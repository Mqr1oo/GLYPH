
void enterDeepSleep8Min(bool showUI) {
    if (showUI) {
    
        display.setRotation(currentScreenRotation); 
        display.fillScreen(GxEPD_WHITE);
        u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
        u8g2Fonts.setFont(u8g2_font_helvB12_tf);
        int sw = u8g2Fonts.getUTF8Width("SHUTDOWN / SLEEP"); 
        u8g2Fonts.setCursor((display.width() - sw)/2, display.height()/2); 
        u8g2Fonts.print("SHUTDOWN / SLEEP");
        display.update(); 
    }

    display.powerDown(); 
    radio.sleep(); 
    
    uint32_t sleepTimeMs = 8 * 60 * 1000;
    
    if(gps_ok) {
        if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            myGNSS.powerOff(sleepTimeMs); 
            xSemaphoreGive(i2cMutex);
        }
    }
    
    esp_sleep_enable_ext1_wakeup(1ULL << BTN_PIN, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_sleep_enable_timer_wakeup((uint64_t)sleepTimeMs * 1000ULL); 

    inDeepSleepMode = true; 
    esp_deep_sleep_start();
}

void evaluateSleep() {
   
    bool manualShutdown = (raw_A && raw_B && raw_C);

    if (manualShutdown) {
        enterDeepSleep8Min(true); 
    }


    if (currentState == PAGE_INIT_LANG || currentState == PAGE_INIT_FREQ || 
        currentState == PAGE_INIT_NAME || currentState == PAGE_INIT_TIME || 
        currentState == PAGE_INIT_COMPASS || isRecording) { 
        sessionActivityTime = millis(); 
        return; 
    }

    if (imu_ok && (millis() - lastPhysicalMovement < 30000)) {
        sessionActivityTime = millis();
        return;
    }

    unsigned long inactive = millis() - sessionActivityTime;
    

    if(inactive > 240000 && currentState != STANDBY_MODE && !isRecording) {
        currentState = STANDBY_MODE; 
        display.setRotation(currentScreenRotation); 
        display.fillScreen(GxEPD_WHITE);
        
        int rectW = 160; 
        int rectH = 50; 
        display.fillRoundRect((display.width() - rectW)/2, (display.height() - rectH)/2, rectW, rectH, 8, GxEPD_BLACK);
        u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK); 
        u8g2Fonts.setFont(u8g2_font_helvB12_tf);
        
        int sw = u8g2Fonts.getUTF8Width(tr_standby[currentLang]); 
        u8g2Fonts.setCursor((display.width() - sw)/2, (display.height() - rectH)/2 + 30); 
        u8g2Fonts.print(tr_standby[currentLang]);
        
        display.update(); 
        display.powerDown();
        
        if(gps_ok) {
            if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                myGNSS.powerSaveMode(true);
                myGNSS.setMeasurementRate(60000); 
                xSemaphoreGive(i2cMutex);
            }
        }
        
        if (deviceConnected) {
            pServer->disconnect(0); 
            delay(50);
        }
        BLEDevice::deinit(true); 
        radio.sleep(); 
        setCpuFrequencyMhz(20); 

 
        while (currentState == STANDBY_MODE) {
            bool wakeUpTriggered = false;

            if (raw_A && raw_B && raw_C) {
                enterDeepSleep8Min(true);
            }

            if (latched_pA || latched_pB || latched_pC) {
                latched_pA = false; latched_pB = false; latched_pC = false;
                wakeUpTriggered = true;
            }


            if (imu_ok && (millis() - lastPhysicalMovement < 2000)) {
                wakeUpTriggered = true;
            }

            if (wakeUpTriggered) {
                currentState = PAGE_MENU_MAIN;
                sessionActivityTime = millis();
                break; 
            }
            
            vTaskDelay(pdMS_TO_TICKS(100));
        }

      
        setCpuFrequencyMhz(80); 
        initBLE(); 
        
        if(gps_ok) { 
            if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                if (currentPowerMode == NORMAL_MODE) {
                    myGNSS.powerSaveMode(false);
                    myGNSS.setMeasurementRate(1000);
                } else { 
                    myGNSS.powerSaveMode(true);
                    myGNSS.setMeasurementRate(10000); 
                }
                xSemaphoreGive(i2cMutex);
            }
        }
        if (currentPowerMode != STEALTH_MODE) {
            radio.standby(); 
            radio.startReceive();
            loraListening = true;
        }

        fullRefreshNeeded = true;
        requestUIUpdate = true;
    }
}

bool quickCheckActivity() {
    bool activityDetected = false;
    unsigned long checkStart = millis();

    while (millis() - checkStart < 4000) { 
        if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(50)) == pdTRUE) {
            

            if (buttons_ok) {
                mButtons.update();
                if (mButtons.isPressed(0) || mButtons.isPressed(1) || mButtons.isPressed(2)) {
                    activityDetected = true;
                }
            }
            
            if (imu_ok && !activityDetected) {
                sensors_event_t a, g, tm;
                imu.getEvent(&a, &g, &tm);
                float acc_mag = sqrt(a.acceleration.x*a.acceleration.x + 
                                     a.acceleration.y*a.acceleration.y + 
                                     a.acceleration.z*a.acceleration.z);
                                     
                if (fabs(acc_mag - 9.8) > 2.0) { 
                    activityDetected = true;
                }
            }
            
            xSemaphoreGive(i2cMutex);
        }
        
        if (activityDetected) break; 
        
        delay(50); 
    }
    
   
    if (!activityDetected) {
        enterDeepSleep8Min(false); 
    }
    

    return true; 
}