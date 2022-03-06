#include <Arduino.h>


typedef struct ventDev { //16 character
  uint8_t batteryState;
  uint8_t doorState;
}ventDev;

typedef struct termoDev { //16 character
  uint8_t batteryState;
  uint8_t setPoint;
  uint8_t temperature;
}termoDev;

typedef struct systemZone { //16 character
  char name[16]= "no Name";
  char icon[16]= "no icon";
  bool isActive = 0;
  uint8_t numOf;
  termoDev termos[10];
  ventDev vents[10];
};

void zone_reload(void);
String get_zone_names(void);
uint8_t add_new_zone(String name, String icon);
uint8_t delete_zone(String name);
