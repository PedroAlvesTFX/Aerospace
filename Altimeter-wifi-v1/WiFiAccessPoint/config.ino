
void save_config() {
    Serial.println("Save Config:" + resetTime);
    resetTime.toCharArray(user_config.date, 25);
//    char date[4][25]    long startPressure[4]   float startAltitude[4]  long MinPressure
   user_config.startPressure = pressureM;
   user_config.startAltitude = altitudeM;
   user_config.MinPressure   = pressureM;
   user_config.MaxAltitude   = altitudeM;
   user_config.temperature   = temperatureM;
   apogee = 0;
   startAltitude = altitudeM;
   startPressure = pressureM;
   EEPROM.put(0, user_config);
   EEPROM.commit();
}

void update_altitude() {
   Serial.println("Update Altitude:" );
   user_config.MinPressure = MinPressure;
   user_config.MaxAltitude = MaxAltitude;
   user_config.temperature = temperatureM;
   EEPROM.put(0, user_config);
   EEPROM.commit();
}


void load_config() {
   EEPROM.get(0, user_config);
   MinPressure = user_config.MinPressure;
   MaxAltitude = user_config.MaxAltitude;
   if (String(user_config.date) == "") {
        user_config.OverShootReference = 20;
        user_config.UnderShootApogee   = 10;
   }
   resetTime.toCharArray(user_config.date, 25);
   Serial.println("Load Config:" + resetTime);
}
