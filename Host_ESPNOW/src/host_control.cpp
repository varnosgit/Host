#include "host_control.h"

hc_message hc_mesg;
uint8_t hc_sendFlag = 0, hc_recvFlag = 0;

void send_data_to_controller(void)
{
    hc_mesg.begin_validator[0] = 'V';
    hc_mesg.begin_validator[1] = 'A';
    hc_mesg.begin_validator[2] = 'C';
    // hc_mesg.command = 0x02;
    // hc_mesg.data[0] = 1;
    // hc_mesg.data[1] = 1;
    // hc_mesg.data[2] = 56;
    // hc_mesg.data[3] = 1;
    hc_mesg.end_validator = 'H';
    
    hc_sendFlag = 0;
    Serial2.write((const uint8_t *)&hc_mesg, sizeof(hc_mesg));  

    Serial.println("send data to controller");
}

bool receive_data_from_controller(void)
{
        // if (Serial2.available()) 
    // {
    //   Serial2.read(rxdata, 5);
    //   Serial.println(rxdata);
    // }
     // delay(50);
    // byte n = Serial2.available();  //3:
    //  if(n != 0) //4:
    //  {           
    //      byte m = Serial.readBytesUntil('\n', rxdata, 5);  //5:
    //      rxdata[m] = '\0';  //6:
    //      Serial.print(String(rxdata)); Serial.println("rxdata");//7:
    //  }
    return false;
}

void handle_controller_message(void)
{

}

void handle_host_message(uint8_t *data, size_t len)
{
    Serial.print("data from browser: ");
    Serial.printf((char *)data);
    Serial.println("");

    hc_mesg.command = 0x03;
    if (strcmp((char*)data, "auto") == 0) 
    {
        hc_mesg.data[0] = 1;
    }
        if (strcmp((char*)data, "on") == 0) 
    {
        hc_mesg.data[0] = 2;
    }
        if (strcmp((char*)data, "off") == 0) 
    {
        hc_mesg.data[0] = 3;
    }
        if (strcmp((char*)data, "heat") == 0) 
    {
        hc_mesg.data[0] = 4;
    }
        if (strcmp((char*)data, "cool") == 0) 
    {
        hc_mesg.data[0] = 5;
    }
    hc_sendFlag = 1;
}