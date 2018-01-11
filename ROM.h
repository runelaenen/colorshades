#include <EEPROM.h>

/*
How does our EEPROM look?
Byte 0: keep tracks of wether the EEPROM has been written to (first usage will have empty eeprom) (222 = 'true', this way the change of a trigger by another program which has been on the arduino before will be lower)
Byte 1: Remember the previously active effect
*/

void loadVarsFromROM() {
  if (((int)EEPROM.read(0)) == 222) {
    changeCurrentEffect((int)EEPROM.read(1));
  }
}

void saveVarsToROM() {
  #if CONFIG_DEBUG 
  Serial.println("saveVarsToROM()");
  #endif

  EEPROM.write(0, 222);
  EEPROM.write(1, effectCurrent);
  EEPROM.commit();

  needToUpdateROM = false;
}

/* Gets called every 250ms. */
void updateROM() {
  if(needToUpdateROM){
    saveVarsToROM();
  }
}

