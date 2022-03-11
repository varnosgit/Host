#include "host_control.h"
#include "zones.h"
#include "wireless_host.h"

extern uint8_t pair_request_flag, globalZoneID;
extern const uint8_t numOfMaxZones;
extern systemZone zones[];;
hc_message hc_mesg;
uint8_t hc_sendFlag = 0, hc_recvFlag = 0;



// this 3 below functions must run on the second core
void send_data_to_controller(void)
{
    hc_mesg.begin_validator[0] = 'V';
    hc_mesg.begin_validator[1] = 'A';
    hc_mesg.begin_validator[2] = 'C';
    hc_mesg.end_validator = 'H';
    Serial2.write((const uint8_t *)&hc_mesg, sizeof(hc_mesg));  
    Serial.println("send data to controller");
    hc_sendFlag = 0;
}

bool receive_data_from_controller(void)
{
    if (Serial2.available()) 
    {
      delay(10);  
      char* rxdata = (char*) &hc_mesg;
      Serial2.read(rxdata, sizeof(hc_mesg));
      Serial2.flush();
      Serial.printf("reciever MAC address is: %02X:%02X:%02X:%02X:%02X:%02X\n",
          hc_mesg.reciever_MAC_addr[0], hc_mesg.reciever_MAC_addr[1], hc_mesg.reciever_MAC_addr[2], 
          hc_mesg.reciever_MAC_addr[3], hc_mesg.reciever_MAC_addr[4], hc_mesg.reciever_MAC_addr[5]); 
      Serial.printf("sender MAC address is: %02X:%02X:%02X:%02X:%02X:%02X\n",
          hc_mesg.sender_MAC_addr[0], hc_mesg.sender_MAC_addr[1], hc_mesg.sender_MAC_addr[2], 
          hc_mesg.sender_MAC_addr[3], hc_mesg.sender_MAC_addr[4], hc_mesg.sender_MAC_addr[5]); 
      return true;
    }
    return false;
}

void handle_controller_message(void) 
{
    if (hc_mesg.__hcdata == 0x03) // 3 = cont.: this is a message from a device (sender mac address)
    {
        if (hc_mesg._command == 0x01) //a device requests to be paired
        {
            if (pair_request_flag == 1) // user is searching for a new device
            {
                //////////////////////////////////////////////// save new device on flash & database
                add_device_to_zone(globalZoneID, hc_mesg._sender , hc_mesg.sender_MAC_addr);
                ////////////////////////////////////////////// notify controller and device
                hc_mesg.__hcdata = 4; // 4 = host: pair a device with sender MAC Add
                hc_sendFlag = 1;
                /////////////////////////////////////// notify browser
                char buf[100]; 
                uint8_t* tempo = hc_mesg.sender_MAC_addr;
                String devModelWeb = "./vent.html"; 
                if (hc_mesg._sender == 2) devModelWeb = "./termo.html";
                snprintf(buf, 100, "Add Device, %02X:%02X:%02X:%02X:%02X:%02X,./termo.html,ventName",
                            tempo[0], tempo[1], tempo[2], tempo[3], tempo[4], tempo[5]); 
                String newDev = String(buf);
                Serial.println("");
                Serial.println(newDev);
                notifyClients_txt(newDev);
                pair_request_flag = 0;
                //////////////////////////////////////////////////////
            }
        } 
    }
    hc_recvFlag = 0;
}


////////////////////////////////////////////////////////////////////////////////////
void handle_host_message(char *data, size_t len)
{
    Serial.print("data from browser: ");Serial.printf((char *)data);Serial.println("");

    String mydata = String(data);
    String strs[20];
    int StringCount = 0;
    // interpret data from web
    while (mydata.length() > 0)
    {
        int index = mydata.indexOf(',');
        if (index == -1) // No seperator found
        {
            strs[StringCount++] = mydata;
            break;
        }
        else
        {
            strs[StringCount++] = mydata.substring(0, index);
            mydata = mydata.substring(index+1);
        }
    }
    // print data from web to serial
    for (int i = 0; i < StringCount; i++) {
        Serial.print(i);  Serial.print(": \"");    Serial.print(strs[i]);    Serial.println("\"");  }


    if (strs[0] == "Add New Zone")
    {
        Serial.println("adding new zone...");
        add_new_zone(strs[1], strs[2]);
    }
    if (strs[0] == "Delete Zone")
    {
        Serial.println("deleting a zone...");
        delete_zone(strs[1]);
    }
     if (strs[0] == "Search Device")
    {
        Serial.println("search for paring a new device...");
        // write to controler ???
        //hc_sendFlag = 1;
        pair_request_flag = 1;
        globalZoneID = get_zoneID_by_name(strs[1]);
    }
     if (strs[0] == "Zone Device Names")
    {   
        globalZoneID = get_zoneID_by_name(strs[1]);
        String devices_names;
        devices_names = get_zoen_device_names(globalZoneID);
        Serial.println(devices_names);
        // String devices_names = "Device Names,v1,v2,v3,v5";
        notifyClients_txt(devices_names);
    }
    // hc_mesg.command = 0x03;
    // if (strcmp((char*)data, "auto") == 0) 
    // {
    //     hc_mesg.data[0] = 1;
    // }
    //     if (strcmp((char*)data, "on") == 0) 
    // {
    //     hc_mesg.data[0] = 2;
    // }
    //     if (strcmp((char*)data, "off") == 0) 
    // {
    //     hc_mesg.data[0] = 3;
    // }
    //     if (strcmp((char*)data, "heat") == 0) 
    // {
    //     hc_mesg.data[0] = 4;
    // }
    //     if (strcmp((char*)data, "cool") == 0) 
    // {
    //     hc_mesg.data[0] = 5;
    // }
    // hc_sendFlag = 1;
}