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
  bool isActive = 0;
  uint8_t numOf;
  termoDev termos[10];
  ventDev vents[10];
};

void zone_reload(void);