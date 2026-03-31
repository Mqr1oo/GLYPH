void hardwareCore0Task(void * pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    bool lastA = false, lastB = false, lastC = false;
    unsigned long lastSlowSensorTick = 0;
    
    for(;;) {

        if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(5)) == pdTRUE) {
            
            if (buttons_ok) {
                mButtons.update();
                raw_A = mButtons.isPressed(0);
                raw_B = mButtons.isPressed(1);
                raw_C = mButtons.isPressed(2);

                if (raw_A && !lastA) { latched_pA = true; lastPhysicalMovement = millis(); }
                if (raw_B && !lastB) { latched_pB = true; lastPhysicalMovement = millis(); }
                if (raw_C && !lastC) { latched_pC = true; lastPhysicalMovement = millis(); }

                lastA = raw_A; 
                lastB = raw_B; 
                lastC = raw_C;
            }

            if (imu_ok) {
                sensors_event_t a, g, tm;
                imu.getEvent(&a, &g, &tm);
                currentAx = a.acceleration.x;
                currentAy = a.acceleration.y;
                currentAz = a.acceleration.z;
                float acc_mag = sqrt(currentAx*currentAx + currentAy*currentAy + currentAz*currentAz);
                if (fabs(acc_mag - 9.8) > 1.2) {
                    lastPhysicalMovement = millis(); 
                }
            }

            if (mag_ok) {
                processCompassData();
            }
            
      
            xSemaphoreGive(i2cMutex);
        }


        if (millis() - lastSlowSensorTick > 1000) {
            lastSlowSensorTick = millis();

    
            if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(30)) == pdTRUE) {
                
                if (gps_ok && currentPowerMode != STEALTH_MODE) {
                    
                    if (myGNSS.getPVT(100)) {
                        hasGpsFix = myGNSS.getGnssFixOk();
                        gpsTimeValid = myGNSS.getTimeValid();
                        
                        if (gpsTimeValid) {
                            gpsYear = myGNSS.getYear();
                            gpsMonth = myGNSS.getMonth();
                            gpsDay = myGNSS.getDay();
                            gpsHour = myGNSS.getHour();
                            gpsMinute = myGNSS.getMinute();
                            gpsSecond = myGNSS.getSecond();
                        }

                        if (hasGpsFix) {
                            lastLat = (double)myGNSS.getLatitude() / 1e7;
                            lastLon = (double)myGNSS.getLongitude() / 1e7;
                            lastAlt = myGNSS.getAltitudeMSL() / 1e3;
                            lastSIV = myGNSS.getSIV();
                            lastAccuracy = myGNSS.getPositionAccuracy() / 1000.0;
                            new_gps_data = true; 
                        }
                    }
                }

                if (sht_ok) {
                    sensors_event_t he, te;
                    sht40.getEvent(&he, &te);
                    currentTemp = te.temperature;
                    currentHum = he.relative_humidity;
                }
                
   
                xSemaphoreGive(i2cMutex);
            }
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(33)); //DO NOT MODIFY
    }
}