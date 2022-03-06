#include "zones.h"
#include <Effortless_SPIFFS.h>

const uint8_t numOfMaxZones = 20;
struct systemZone zones[numOfMaxZones];
struct systemZone* zonesAddr = &zones[0];
eSPIFFS fileSystem;

//////////////////////////////////// privet functions
char* readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\n", path);
    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("Failed to open file for reading");
        return "FAIL";
    }
    file.read((uint8_t *)&zones[0], sizeof(zones));
    delayMicroseconds(100);
    file.close(); 
}
////////////
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    file.write((uint8_t *)message, sizeof(zones));
    file.close();
}
//////////////////////////////////////////////////////

void zone_reload(void)
{   
    ///////////////////////////////////////////////////// flash zone file read
    //writeFile(SPIFFS, "/zones.txt", (char *)zonesAddr);
    readFile(SPIFFS, "/zones.txt");

    if (strcmp(zones[0].name, "Main Room") != 0) //no main room available
    {
        zones[0].isActive = true;    
        strcpy(zones[0].name, "Main Room");
        strcpy(zones[0].icon, "ssss Room");
        writeFile(SPIFFS, "/zones.txt", (char *)zonesAddr);
    }
}

String get_zone_names(void)
{
    String __zone_name_commands = "Zone Names";
    for(int i=0; i<numOfMaxZones; i++)
    {
        if (zones[i].isActive)
            __zone_name_commands += String(",") + String(zones[i].name);
    } 
   // __zone_name_commands += String(zones[0].name) +  String(",") + String(zones[1].name);
    return __zone_name_commands;
}

uint8_t add_new_zone(String name, String icon)
{   
    char temp[16];
    for (int i=0;i<numOfMaxZones; i++)
        {
            if (zones[i].isActive == false) //available empty zone
            {
                zones[i].isActive = true;
                name.toCharArray(temp, 16);
                strcpy(zones[i].name, temp);
                icon.toCharArray(temp, 16);
                strcpy(zones[i].icon, temp);
                writeFile(SPIFFS, "/zones.txt", (char *)zonesAddr);
                return i;
            }
        }
    return 0;    
}

uint8_t delete_zone(String name)
{
    char temp[16];
    name.toCharArray(temp, 16);
    for (int i=0;i<numOfMaxZones; i++)
        {
            if (strcmp(zones[i].name, temp) == 0)  //equal names
            {   
                Serial.println("A zone has been deleted succesfully!");
                zones[i].isActive = false;
                writeFile(SPIFFS, "/zones.txt", (char *)zonesAddr);
                return i;
            }
        }
    return 0;    
}


