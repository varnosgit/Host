#include "zones.h"

struct systemZone zones[10];
void zone_reload(void)
{
    strcpy(zones[0].name, "main Room");
    strcpy(zones[1].name, "sleep room"); 
}
