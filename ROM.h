#include <EEPROM.h>

/*
How does our EEPROM look?
Byte 0: Remember the previously active effect
*/

void loadVarsFromROM() {
  changeCurrentEffect((int)EEPROM.read(0));
}

void saveVarsToROM() {
  #if CONFIG_DEBUG 
  Serial.println("saveVarsToROM()");
  #endif

  EEPROM.write(0, effectCurrent);
  EEPROM.commit();

  needToUpdateROM = false;
}

/* Gets called every 250ms. */
void updateROM() {
  if(needToUpdateROM){
    saveVarsToROM();
  }
}

