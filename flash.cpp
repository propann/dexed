#include "flash.h"

void fill_sample_zone_from_flash (const uint16_t entry_number, const uint8_t preset_number, const uint8_t zone_number)
{
  char filename[MAX_FLASH_FILENAME_LEN];
  uint32_t filesize;
  uint16_t f;

  SerialFlash.opendir();

  // search for the right file from its entry_number in the directory
  for (f = 0; f <= entry_number; f++) {
    if (SerialFlash.readdir(filename, sizeof(filename), filesize)) {
//#ifdef DEBUG
//      Serial.print(F("entry #"));
//      Serial.print(f);
//      Serial.print(F(": "));
//      Serial.println(filename);  
//#endif
    }
    else
      break;
  }

  if (f == entry_number + 1) {
#ifdef DEBUG
    Serial.print(F("Flash file found for entry #"));
    Serial.print(entry_number);
    Serial.print(F(": "));
    Serial.println(filename);
#endif
  } else {
#ifdef DEBUG
    Serial.print(F("Flash file not found for entry #"));
    Serial.println(entry_number);
#endif
    strcpy(filename, "*ERROR*");
  }

  // fill the multisample zone informations
  strcpy(msz[preset_number][zone_number].name, filename);
#ifdef DEBUG
    Serial.print(F("MSZ preset #"));
    Serial.print(preset_number);
    Serial.print(F(" - zone #"));
    Serial.print(zone_number);
    Serial.print(F(": "));
    Serial.println(msz[preset_number][zone_number].name);
#endif
}
