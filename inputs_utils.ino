void pingActivity() { 
    sessionActivityTime = millis(); 
}

double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0; 
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;
    
    double a = sin(dLat / 2) * sin(dLat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c;
}

double getTraveledDistance() {
    return sessionTotalDistance; 
}

bool processVirtualCommand(String cmd) {
    if (cmd.equalsIgnoreCase("bypass")) {
        currentLang = 1; 
        prefs.putInt("lang", currentLang);

        currentFreq = 868.0;
        prefs.putFloat("freq", currentFreq);
        prefs.putString("version", OS_VERSION);

        radio.begin(currentFreq);
        radio.setSpreadingFactor(11);
        radio.setBandwidth(125.0);   
        radio.setCodingRate(8);        
        radio.setSyncWord(0x12);
        radio.setOutputPower(22);     

        myName = "username";
        prefs.putString("name", myName);

        timeOffset = 2; 
        prefs.putInt("gmt", timeOffset);

        currentState = PAGE_MENU_MAIN;
        fullRefreshNeeded = false;
        requestUIUpdate = true;
        
        notifyPhone("[GLYPH] Setup BYPASSED!");
        return true;
    }

    if (cmd.startsWith("CMD_PWR:")) {
        int mode = cmd.substring(8).toInt();
        if (mode >= 0 && mode <= 2) {
            currentPowerMode = (PowerMode)mode;
            menuSelection = mode;
            
            if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                if (currentPowerMode == NORMAL_MODE) {
                    setCpuFrequencyMhz(80);
                    if(gps_ok) { myGNSS.powerSaveMode(false); myGNSS.setMeasurementRate(1000); }
                } else if (currentPowerMode == ECO_MODE) {
                    if(gps_ok) { myGNSS.powerSaveMode(true); myGNSS.setMeasurementRate(10000); }
                    setCpuFrequencyMhz(40);
                } else if (currentPowerMode == STEALTH_MODE) {
                    if(gps_ok) { myGNSS.powerSaveMode(true); myGNSS.setMeasurementRate(10000); }
                    radio.standby();
                    loraListening = false;
                    setCpuFrequencyMhz(40);
                }
                xSemaphoreGive(i2cMutex);
            }
            
            requestUIUpdate = true;
            fullRefreshNeeded = false;
            notifyPhone("[SYS] Power Mode Updated");
        }
        return true;
    }

    if (cmd.startsWith("CMD_SEC:")) {
        String state = cmd.substring(8);
        secureMode = (state == "ON");
        requestUIUpdate = true;
        fullRefreshNeeded = false;
        notifyPhone("[SYS] Secure Mode: " + state);
        return true;
    }

    if (cmd.startsWith("CMD_TEAM:")) {
        String tName = cmd.substring(9);
        if(tName.length() > 10) tName = tName.substring(0, 10);
        myTeam = tName;
        if(myTeam.length() == 0) myTeam = "ALPHA";
        prefs.putString("team", myTeam);
        requestUIUpdate = true;
        fullRefreshNeeded = false;
        notifyPhone("[SYS] Team updated");
        return true;
    }

    if (cmd == "A@") { virt_pA = true; return true; }
    if (cmd == "B@") { virt_pB = true; return true; }
    if (cmd == "C@") { virt_pC = true; return true; }
    if (cmd == "AL@") { virt_longA = true; return true; }
    if (cmd == "BL@") { virt_longB = true; return true; }
    if (cmd == "CL@") { virt_longC = true; return true; }
    
    if (cmd == "AB@") { virt_comboAB = true; return true; }
    if (cmd == "AC@") { virt_comboAC = true; return true; }
    if (cmd == "BC@") { virt_comboBC = true; return true; }

    bool inSetup = (currentState == PAGE_INIT_LANG || currentState == PAGE_INIT_FREQ || currentState == PAGE_INIT_NAME || currentState == PAGE_INIT_TIME || currentState == PAGE_INIT_COMPASS);

    if (!inSetup && cmd.length() == 2 && cmd[1] == '@') {
        char c = cmd[0];
        if (c >= '1' && c <= '6') {
            if (c == '1') currentState = PAGE_MAP;
            else if (c == '2') { 
                currentState = PAGE_LORA; 
                if(currentPowerMode != STEALTH_MODE) { radio.startReceive(); loraListening = true; } 
            }
            else if (c == '3') currentState = PAGE_CLOCK;
            else if (c == '4') currentState = PAGE_MENU_TEAM;
            else if (c == '5') currentState = PAGE_MENU_POWER;
            else if (c == '6') currentState = PAGE_MENU_LANG;
            
            fullRefreshNeeded = false; 
            requestUIUpdate = true;
            pingActivity();
            return true;
        }
    }
    return false;
}

void processInitInput(String inputStr) {
    if (currentState == PAGE_INIT_LANG) {
        int l = inputStr.toInt();
        if (l >= 0 && l <= 9) { 
            currentLang = l; 
            prefs.putInt("lang", currentLang); 
            currentState = PAGE_INIT_FREQ; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
            notifyPhone("[GLYPH] Language set!"); 
        }
    }
    else if (currentState == PAGE_INIT_FREQ) {
        if (inputStr.indexOf("868") != -1) {
            currentFreq = 868.0; 
            prefs.putFloat("freq", currentFreq); 
            prefs.putString("version", OS_VERSION); 
            radio.begin(currentFreq); 
            radio.setSpreadingFactor(11); 
            radio.setBandwidth(125.0);
            radio.setCodingRate(8);
            radio.setSyncWord(0x12); 
            radio.setOutputPower(22); 
            currentState = PAGE_INIT_NAME; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
            notifyPhone("[GLYPH] 868MHz Set!"); 
            sendInputPrompt(tr_prompt_name[currentLang]);
        } else if (inputStr.indexOf("915") != -1) {
            currentFreq = 915.0; 
            prefs.putFloat("freq", currentFreq); 
            prefs.putString("version", OS_VERSION); 
            radio.begin(currentFreq); 
            radio.setSpreadingFactor(11); 
            radio.setBandwidth(125.0);
            radio.setCodingRate(8);
            radio.setSyncWord(0x12); 
            radio.setOutputPower(22); 
            currentState = PAGE_INIT_NAME; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
            notifyPhone("[GLYPH] 915MHz Set!"); 
            sendInputPrompt(tr_prompt_name[currentLang]);
        }
    }
    else if (currentState == PAGE_INIT_NAME) {
        myName = inputStr; 
        if(myName.length() > 10) myName = myName.substring(0, 10); 
        prefs.putString("name", myName); 
        currentState = PAGE_INIT_TIME; 
        fullRefreshNeeded = false; 
        requestUIUpdate = true; 
        notifyPhone("[GLYPH] Saved: " + myName); 
        sendInputPrompt(tr_prompt_time[currentLang]);
    }
    else if (currentState == PAGE_INIT_TIME) {
        int offset = inputStr.toInt();
        if (offset >= -12 && offset <= 14) { 
            timeOffset = offset; 
            prefs.putInt("gmt", timeOffset); 
            
            if (mag_ok) {
                currentState = PAGE_INIT_COMPASS;
                calibProgress = 0;
                calibDone = false;
                mag_min_x = 9999.0; mag_max_x = -9999.0;
                mag_min_y = 9999.0; mag_max_y = -9999.0;
                fullRefreshNeeded = true; 
            } else {
                currentState = PAGE_MENU_MAIN; 
                fullRefreshNeeded = true;
            }
            requestUIUpdate = true; 
            notifyPhone("[GLYPH] UTC Set!"); 
        }
    }
}

void checkSerialInput() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n'); 
        input.trim();
        while(input.startsWith("\n") || input.startsWith("\r")) { input.remove(0, 1); }
        while(input.endsWith("\n") || input.endsWith("\r")) { input.remove(input.length()-1); }
        if (input.length() == 0) return; 
        pingActivity();
        
        if (processVirtualCommand(input)) return;

        if (input == "SIMOFF") { 
            simActive = false; 
            hasGpsFix = false; 
            requestUIUpdate = true; 
            fullRefreshNeeded = false; 
            return; 
        }
        
        int commaIdx = input.indexOf(',');
        if (commaIdx > 0 && input.length() < 25 && input.substring(0, commaIdx).toFloat() != 0) {
            float parsedLat = input.substring(0, commaIdx).toFloat(); 
            float parsedLon = input.substring(commaIdx + 1).toFloat();
            if (parsedLat != 0.0 && parsedLon != 0.0) { 
                simActive = true; 
                hasGpsFix = true; 
                lastLat = parsedLat; 
                lastLon = parsedLon; 
                lastSIV = 12; 
                lastAlt = 300.0 + random(-15, 15); 
                requestUIUpdate = true; 
                return; 
            }
        }

        bool inSetup = (currentState == PAGE_INIT_LANG || currentState == PAGE_INIT_FREQ || currentState == PAGE_INIT_NAME || currentState == PAGE_INIT_TIME || currentState == PAGE_INIT_COMPASS);

        if (inSetup) { 
            processInitInput(input); 
        }
        else if (currentState == PAGE_KEYBOARD) { 
            msgDraft = input; 
            if(msgDraft.length() > 30) msgDraft = msgDraft.substring(0, 30); 
            requestUIUpdate = true; 
            fullRefreshNeeded = false; 
        }
        else if (currentState == PAGE_TEAM_NAME_EDIT) { 
            teamDraft = input; 
            if(teamDraft.length() > 10) teamDraft = teamDraft.substring(0, 10); 
            myTeam = teamDraft; 
            if(myTeam.length() == 0) myTeam = "ALPHA"; 
            prefs.putString("team", myTeam); 
            currentState = PAGE_MENU_TEAM; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
        }
        else if (currentPowerMode != STEALTH_MODE) {
            String tmp = msgDraft;
            msgDraft = input;
            if(msgDraft.length() > 30) msgDraft = msgDraft.substring(0, 30);
            executeSendMsg();
            msgDraft = tmp;
        }
    }
}

void handleButtons() {
    bool cA = raw_A;
    bool cB = raw_B;
    bool cC = raw_C;
    
    if (virt_pA) { latched_pA = true; virt_pA = false; }
    if (virt_pB) { latched_pB = true; virt_pB = false; }
    if (virt_pC) { latched_pC = true; virt_pC = false; }

    bool pA = latched_pA; latched_pA = false;
    bool pB = latched_pB; latched_pB = false;
    bool pC = latched_pC; latched_pC = false;
    
    bool inSetup = (currentState == PAGE_INIT_LANG || currentState == PAGE_INIT_FREQ || currentState == PAGE_INIT_NAME || currentState == PAGE_INIT_TIME || currentState == PAGE_INIT_COMPASS);

    static unsigned long comboABStart = 0; 
    static bool abTriggered = false;
    
    if (virt_comboAB || (cA && cB && !cC)) {
        
        pA = false; pB = false; pC = false;
        latched_pA = false; latched_pB = false; latched_pC = false;

        if (inSetup) {
             comboABStart = 0; 
        } else {
            if (virt_comboAB) {
                abTriggered = true; 
            } else if (comboABStart == 0) {
                comboABStart = millis();
            } 
            
            if (virt_comboAB || (millis() - comboABStart > 1000 && !abTriggered)) {
                abTriggered = true; 
                virt_comboAB = false;
                
                isRecording = !isRecording;
                notifyPhone(isRecording ? "SYS_REC:1" : "SYS_REC:0");
                if(isRecording) {
                    breadcrumbIdx = 0;          
                    sessionTotalDistance = 0.0; 
                    lastLoggedLat = 0.0; 
                    lastLoggedLon = 0.0; 

                    char baseDate[32];
                    if((gps_ok || simActive) && gpsTimeValid) {
                        snprintf(baseDate, sizeof(baseDate), "%02d_%02d_%04d_%02d-%02d", 
                            gpsDay, gpsMonth, gpsYear, 
                            (gpsHour + timeOffset + 24) % 24, gpsMinute % 60);
                    } else {
                        snprintf(baseDate, sizeof(baseDate), "OFFLINE_%lu", millis()/1000);
                    }

                    if (sdDetected) {
                            acquireSD();
                            

                            String finalName = baseDate;
                            int fileIndex = 1;
                            
                            while (SD.exists(("/route_" + finalName + ".kml").c_str())) {
                                finalName = String(baseDate) + "-" + String(fileIndex);
                                fileIndex++;
                            }
                            
      
                            strncpy(currentRecordDate, finalName.c_str(), sizeof(currentRecordDate));


                            String kmlPath = "/route_" + String(currentRecordDate) + ".kml";
                            File f = SD.open(kmlPath, FILE_WRITE);
                            if (f) {
                                f.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
                                f.println("<kml xmlns=\"http://www.opengis.net/kml/2.2\">");
                                f.println("  <Document>");
                                f.println("    <name>GLYPH " + String(currentRecordDate) + "</name>");
                                f.print("    <Placemark><LineString><coordinates>\n");
                                f.print(KML_CLOSING); 
                                f.close();
                            }

                            String csvPath = "/system_" + String(currentRecordDate) + ".csv";
                            File f2 = SD.open(csvPath, FILE_WRITE);
                            if (f2) {
                                f2.println("Date,Time,Lat,Lon,Temp,Hum,Ax,Ay,Az,Log");
                                f2.close();
                            }
                        }else {
                            strncpy(currentRecordDate, baseDate, sizeof(currentRecordDate));
                        }

                } else {
                    if (sdDetected) {
                        acquireSD();
                        String kmlPath = "/route_" + String(currentRecordDate) + ".kml";
                        
                        char stopSuffix[32] = "";
                        if((gps_ok || simActive) && gpsTimeValid) {
                            snprintf(stopSuffix, sizeof(stopSuffix), "_to_%02d-%02d", 
                                (gpsHour + timeOffset + 24) % 24, gpsMinute % 60);
                        } else {
                            snprintf(stopSuffix, sizeof(stopSuffix), "_to_STOP");
                        }

                        String newKmlPath = "/route_" + String(currentRecordDate) + String(stopSuffix) + ".kml";
                        String oldCsvPath = "/system_" + String(currentRecordDate) + ".csv";
                        String newCsvPath = "/system_" + String(currentRecordDate) + String(stopSuffix) + ".csv";

                        SD.rename(kmlPath.c_str(), newKmlPath.c_str());
                        SD.rename(oldCsvPath.c_str(), newCsvPath.c_str());
                    }
                }
                
                display.setRotation(currentScreenRotation); 
                display.fillScreen(GxEPD_WHITE); 
                display.fillRect(0, 0, display.width(), 20, GxEPD_BLACK);
                u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
                u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
                u8g2Fonts.setFont(u8g2_font_helvB12_tf);
                
                String recTxt = isRecording ? tr_route_start[currentLang] : tr_route_stop[currentLang];
                u8g2Fonts.setCursor((display.width() - u8g2Fonts.getUTF8Width(recTxt.c_str()))/2, 60); 
                u8g2Fonts.print(recTxt.c_str());
                display.updateWindow(0, 0, display.width(), display.height(), true); 
                
                delay(500); 
                
                requestUIUpdate = true; 
                fullRefreshNeeded = false;
                pingActivity();
            }
        }
        return; 
    } else { 
        comboABStart = 0; 
        abTriggered = false; 
    }

    static unsigned long comboBCStart = 0;
    static bool bcTriggered = false;
    if (virt_comboBC || (cB && cC && !cA)) {
        pA = false; pB = false; pC = false;
        latched_pA = false; latched_pB = false; latched_pC = false;

        if (virt_comboBC) { bcTriggered = true; } 
        else if (comboBCStart == 0) { comboBCStart = millis(); } 
        
        if (virt_comboBC || (millis() - comboBCStart > 1000 && !bcTriggered)) {
            bcTriggered = true;
            virt_comboBC = false;
            
            notifyPhone("[SYS] SOS BROADCASTING!");

            display.setRotation(currentScreenRotation); 
            display.fillScreen(GxEPD_WHITE); 
            u8g2Fonts.setFont(u8g2_font_helvB12_tf);
            u8g2Fonts.setCursor((display.width() - u8g2Fonts.getUTF8Width("SOS ACTIVE"))/2, 50); 
            u8g2Fonts.print("SOS ACTIVE");
            display.updateWindow(0, 0, display.width(), display.height(), true); 
            if (currentPowerMode != STEALTH_MODE) {
                String fullMsg = myName + " SOS! LAT:" + String(lastLat, 5) + " LON:" + String(lastLon, 5);
                radio.setSpreadingFactor(12);
                unsigned long sosTimer = millis();
                while (millis() - sosTimer < 10000) {
                    radio.transmit(fullMsg + "|" + String(lastLat, 5) + "," + String(lastLon, 5));
                    delay(150); 
                }
                radio.startReceive(); loraListening = true;
            }
            
            requestUIUpdate = true; fullRefreshNeeded = false; pingActivity();
        }
        return; 
    } else { comboBCStart = 0; bcTriggered = false; }

    static unsigned long comboACStart = 0;
    static bool acTriggered = false;
    if (virt_comboAC || (cA && cC && !cB)) {
        
        pA = false; pB = false; pC = false;
        latched_pA = false; latched_pB = false; latched_pC = false;

        if (inSetup) {
             comboACStart = 0; 
        } else {
            if (virt_comboAC) {
                acTriggered = true;
            } else if (comboACStart == 0) {
                comboACStart = millis();
            } 
            
            if (virt_comboAC || (millis() - comboACStart > 1000 && !acTriggered)) {
                acTriggered = true;
                virt_comboAC = false;
                
                display.setRotation(currentScreenRotation); 
                display.fillScreen(GxEPD_WHITE); 
                display.fillRect(0, 0, display.width(), 20, GxEPD_BLACK);
                u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
                u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
                u8g2Fonts.setFont(u8g2_font_helvB12_tf);
                String sosTxt = "BROADCASTING SOS";
                String sosTxt2 = "FOR 10 SECONDS...";
                u8g2Fonts.setCursor((display.width() - u8g2Fonts.getUTF8Width(sosTxt.c_str()))/2, 50); 
                u8g2Fonts.print(sosTxt.c_str());
                u8g2Fonts.setFont(u8g2_font_helvB10_tf);
                u8g2Fonts.setCursor((display.width() - u8g2Fonts.getUTF8Width(sosTxt2.c_str()))/2, 80); 
                u8g2Fonts.print(sosTxt2.c_str());
                display.updateWindow(0, 0, display.width(), display.height(), true); 
                
                if (currentPowerMode != STEALTH_MODE) {
                    String fullMsg = myName + " SOS! LAT:" + String(lastLat, 5) + " LON:" + String(lastLon, 5);
                    String msgData = fullMsg + "|" + String(lastLat, 5) + "," + String(lastLon, 5);
                    String msgToSend = msgData;

                    radio.setSpreadingFactor(11);
                    
                    unsigned long sosTimer = millis();
                    int txCount = 0;
                    while (millis() - sosTimer < 10000) {
                        radio.standby();
                        radio.transmit(msgToSend);
                        txCount++;
                        delay(100); 
                    }

                    String timeStr = "--:--";
                    if((gps_ok || simActive) && gpsTimeValid) {
                        char tBuf[16];
                        snprintf(tBuf, sizeof(tBuf), "%02d:%02d", (gpsHour + timeOffset + 24) % 24, gpsMinute % 60);
                        timeStr = String(tBuf);
                    }
                    String screenMsg = ">> [" + timeStr + "] " + fullMsg + " (x" + String(txCount) + ")";
                    for(int i = MAX_LORA_MSGS - 1; i > 0; i--) {
                        loraHistory[i] = loraHistory[i-1];
                    }
                    loraHistory[0] = screenMsg;
                    if (loraMsgCount < MAX_LORA_MSGS) loraMsgCount++;

                    notifyPhone("[TX SOS]: " + fullMsg);
                    radio.startReceive();
                    loraListening = true;
                }

                requestUIUpdate = true; 
                fullRefreshNeeded = false;
                pingActivity();
            }
        }
        return; 
    } else {
        comboACStart = 0; 
        acTriggered = false;
    }

    static unsigned long holdMapExitA = 0;
    static bool handledLongA = false;
    if (currentState == PAGE_KML_LIST) {
        if (cA || virt_longA) {
            if (virt_longA) {
                handledLongA = true;
                currentState = PAGE_MAP; 
                fullRefreshNeeded = false; 
                requestUIUpdate = true; 
                pingActivity();
                virt_longA = false;
            } else {
                if (holdMapExitA == 0) { 
                    holdMapExitA = millis(); 
                    handledLongA = false; 
                } else if (!handledLongA && millis() - holdMapExitA > 5000) { 
                    handledLongA = true; 
                    currentState = PAGE_MAP; 
                    fullRefreshNeeded = false; 
                    requestUIUpdate = true; 
                    pingActivity();
                }
            }
        } else {
            if (holdMapExitA > 0 && !handledLongA) { 
                if (kmlSelectionIndex > 0) kmlSelectionIndex--; 
                else kmlSelectionIndex = kmlFileCount - 1; 
                requestUIUpdate = true;
                fullRefreshNeeded = false; 
                pingActivity(); 
            }
            holdMapExitA = 0; 
            handledLongA = false;
        }
        
        if (pC) { 
            if (kmlSelectionIndex < kmlFileCount - 1) kmlSelectionIndex++; 
            else kmlSelectionIndex = 0; 
            requestUIUpdate = true;
            fullRefreshNeeded = false; 
            pingActivity(); 
        }
        if (pB) {
            if (kmlSelectionIndex == 0) { 
                currentKmlOverlay = ""; 
                kmlCacheValid = false; 
            } else { 
                currentKmlOverlay = "/" + kmlFiles[kmlSelectionIndex]; 
                loadKMLCache(); 
            }
            currentState = PAGE_MAP; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
            pingActivity();
        }
        return; 
    }

    static unsigned long bTimer = 0; 
    static bool bLongPressed = false;
    static unsigned long lastDeleteTime = 0;

    if ((cB && !pB) || virt_longB) {
        if (virt_longB) {
            bLongPressed = true; 
            if (currentState == PAGE_KEYBOARD && msgDraft.length() > 0) msgDraft.remove(msgDraft.length()-1);
            else if (currentState == PAGE_TEAM_NAME_EDIT && teamDraft.length() > 0) teamDraft.remove(teamDraft.length()-1);
            requestUIUpdate = true; 
            fullRefreshNeeded = false; 
            pingActivity(); 
            virt_longB = false;
        } else {
            if (bTimer == 0) { 
                bTimer = millis(); 
                bLongPressed = false; 
            } else if (millis() - bTimer > 600) { 
                bLongPressed = true; 
                
                if ((currentState == PAGE_KEYBOARD || currentState == PAGE_TEAM_NAME_EDIT) && (millis() - lastDeleteTime > 150)) {
                    lastDeleteTime = millis();
                    if (currentState == PAGE_KEYBOARD && msgDraft.length() > 0) msgDraft.remove(msgDraft.length()-1);
                    else if (currentState == PAGE_TEAM_NAME_EDIT && teamDraft.length() > 0) teamDraft.remove(teamDraft.length()-1);
                    requestUIUpdate = true; 
                    fullRefreshNeeded = false; 
                    pingActivity();
                }
            }
        }
    } else if (!cB) { 
        bTimer = 0; 
        bLongPressed = false; 
        lastDeleteTime = 0;
    }

    static unsigned long aTimer = 0;
    if ((cA && !pA) || virt_longA) {
        if (virt_longA) {
            if (currentState == PAGE_KEYBOARD) { 
                msgDraft = ""; currentState = PAGE_LORA; fullRefreshNeeded = false; requestUIUpdate = true; pingActivity(); 
            } else if (currentState == PAGE_TEAM_NAME_EDIT) { 
                currentState = PAGE_MENU_TEAM; fullRefreshNeeded = false; requestUIUpdate = true; pingActivity(); 
            }
            virt_longA = false;
        } else {
            if(aTimer == 0) aTimer = millis();
            if(millis() - aTimer > 800) { 
                if (currentState == PAGE_KEYBOARD) { 
                    msgDraft = ""; 
                    currentState = PAGE_LORA; 
                    fullRefreshNeeded = false; 
                    requestUIUpdate = true; 
                    aTimer = 0; 
                    pingActivity(); 
                } else if (currentState == PAGE_TEAM_NAME_EDIT) { 
                    currentState = PAGE_MENU_TEAM; 
                    fullRefreshNeeded = false; 
                    requestUIUpdate = true; 
                    aTimer = 0; 
                    pingActivity(); 
                }
            }
        }
    } else if (!cA) {
        aTimer = 0;
    }

    static unsigned long cTimer = 0;
    if ((cC && !pC) || virt_longC) {
        if (virt_longC) {
            if (currentState == PAGE_KEYBOARD) { 
                executeSendMsg(); 
            } else if (currentState == PAGE_TEAM_NAME_EDIT) { 
                myTeam = teamDraft; 
                if(myTeam.length() == 0) myTeam = "ALPHA"; 
                prefs.putString("team", myTeam); 
                currentState = PAGE_MENU_TEAM; 
                fullRefreshNeeded = false; 
                requestUIUpdate = true; 
                pingActivity(); 
            }
            virt_longC = false;
        } else {
            if(cTimer == 0) cTimer = millis();
            if(millis() - cTimer > 800) { 
                if (currentState == PAGE_KEYBOARD) { 
                    cTimer = 0; 
                    executeSendMsg();
                } else if (currentState == PAGE_TEAM_NAME_EDIT) { 
                    myTeam = teamDraft; 
                    if(myTeam.length() == 0) myTeam = "ALPHA"; 
                    prefs.putString("team", myTeam); 
                    currentState = PAGE_MENU_TEAM; 
                    fullRefreshNeeded = false; 
                    requestUIUpdate = true; 
                    cTimer = 0; 
                    pingActivity(); 
                }
            }
        }
    } else if (!cC) {
        cTimer = 0;
    }

    if (!pA && !pB && !pC) return;
    
    pingActivity(); 
    
    if (currentState == STANDBY_MODE) { 
        currentState = PAGE_MENU_MAIN; 
        fullRefreshNeeded = false; 
        requestUIUpdate = true; 
        return; 
    }

    if (currentState == PAGE_INIT_LANG) {
        if (pA) { if (tempLangSelection > 0) tempLangSelection--; else tempLangSelection = 9; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pC) { if (tempLangSelection < 9) tempLangSelection++; else tempLangSelection = 0; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pB) { 
            currentLang = tempLangSelection; 
            prefs.putInt("lang", currentLang); 
            currentState = PAGE_INIT_FREQ; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
        }
    }
    else if (currentState == PAGE_INIT_FREQ) {
        if (pA) { currentFreq = 868.0; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pC) { currentFreq = 915.0; requestUIUpdate = true; fullRefreshNeeded = false; }
        else if (pB) { 
            prefs.putFloat("freq", currentFreq); 
            prefs.putString("version", OS_VERSION); 
            radio.begin(currentFreq); 
            radio.setSpreadingFactor(9); 
            radio.setSyncWord(0x12); 
            radio.setOutputPower(17); 
            currentState = PAGE_INIT_NAME; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
            sendInputPrompt(tr_prompt_name[currentLang]); 
        }
    }
    else if (currentState == PAGE_INIT_TIME) {
        if (pA) { if (tempTimeSelection > -12) tempTimeSelection--; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pC) { if (tempTimeSelection < 14) tempTimeSelection++; requestUIUpdate = true; fullRefreshNeeded = false; }
        else if (pB) { 
            timeOffset = tempTimeSelection; 
            prefs.putInt("gmt", timeOffset); 
            
            if (mag_ok) {
                currentState = PAGE_INIT_COMPASS;
                calibProgress = 0;
                calibDone = false;
                mag_min_x = 9999.0; mag_max_x = -9999.0;
                mag_min_y = 9999.0; mag_max_y = -9999.0;
                fullRefreshNeeded = false; 
            } else {
                currentState = PAGE_MENU_MAIN; 
                fullRefreshNeeded = false; 
            }
            
            requestUIUpdate = true; 
        }
    }
    else if (currentState == PAGE_INIT_COMPASS) {
        if (pA || pB || pC) { 
            currentState = PAGE_MENU_MAIN; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
        }
    }
    else if (currentState == PAGE_MENU_MAIN) {
        if (pA) { if (mainMenuItem > 0) mainMenuItem--; else mainMenuItem = 6; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pC) { if (mainMenuItem < 6) mainMenuItem++; else mainMenuItem = 0; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pB) { 
            if (mainMenuItem == 0) currentState = PAGE_MAP; 
            else if (mainMenuItem == 1) { 
                currentState = PAGE_LORA; 
                if(currentPowerMode!=STEALTH_MODE){
                    radio.startReceive(); 
                    loraListening = true;
                } 
            } 
            else if (mainMenuItem == 2) currentState = PAGE_CLOCK; 
            else if (mainMenuItem == 3) { currentState = PAGE_MENU_TEAM; } 
            else if (mainMenuItem == 4) currentState = PAGE_MENU_POWER; 
            else if (mainMenuItem == 5) currentState = PAGE_MENU_LANG;
            else if (mainMenuItem == 6) { currentState = PAGE_MENU_TIME; tempTimeSelection = timeOffset; }
            fullRefreshNeeded = false; 
            requestUIUpdate = true;
        }
    }
    else if (currentState == PAGE_MAP) {
        if (pA) { currentState = PAGE_MENU_MAIN; fullRefreshNeeded = false; requestUIUpdate = true; } 
        if (pC) { scanKMLFiles(); currentState = PAGE_KML_LIST; fullRefreshNeeded = false; requestUIUpdate = true; }
        if (pB) { mapZoomLevel = (mapZoomLevel + 1) % 3; fullRefreshNeeded = false; requestUIUpdate = true; }
    }
    else if (currentState == PAGE_CLOCK) { 
        if (pA) { currentState = PAGE_MENU_MAIN; fullRefreshNeeded = false; requestUIUpdate = true; } 
    }
    else if (currentState == PAGE_LORA) {
        if (pB) { currentState = PAGE_KEYBOARD; fullRefreshNeeded = false; requestUIUpdate = true; sendInputPrompt(tr_prompt_msg[currentLang]); }
        if (pA) { currentState = PAGE_MENU_MAIN; fullRefreshNeeded = false; requestUIUpdate = true; }
        if (pC) { secureMode = !secureMode; fullRefreshNeeded = false; requestUIUpdate = true; }
    }
    else if (currentState == PAGE_KEYBOARD) {
        if (pA) { kbCursor = (kbCursor > 0) ? kbCursor - 1 : strlen(kbChars) - 1; requestUIUpdate = true; fullRefreshNeeded = false; }
        if (pC) { kbCursor = (kbCursor < strlen(kbChars) - 1) ? kbCursor + 1 : 0; requestUIUpdate = true; fullRefreshNeeded = false; }
        if (pB && !bLongPressed) { 
            char selected = kbChars[kbCursor]; 
            if(selected == '<') { 
                if(msgDraft.length() > 0) msgDraft.remove(msgDraft.length()-1); 
            } else if(msgDraft.length() < 30) {
                msgDraft += selected; 
            }
            requestUIUpdate = true; 
            fullRefreshNeeded = false; 
        }
    }
    else if (currentState == PAGE_MENU_TEAM) {
        if (pA) { currentState = PAGE_MENU_MAIN; fullRefreshNeeded = false; requestUIUpdate = true; }
        if (pB) { 
            currentState = PAGE_TEAM_NAME_EDIT; 
            teamDraft = myTeam; 
            kbCursor = 0; 
            sendInputPrompt(tr_prompt_team[currentLang]); 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
        }
    }
    else if (currentState == PAGE_TEAM_NAME_EDIT) {
        if (pA) { kbCursor = (kbCursor > 0) ? kbCursor - 1 : strlen(kbChars) - 1; requestUIUpdate = true; fullRefreshNeeded = false; }
        if (pC) { kbCursor = (kbCursor < strlen(kbChars) - 1) ? kbCursor + 1 : 0; requestUIUpdate = true; fullRefreshNeeded = false; }
        if (pB && !bLongPressed) { 
            char selected = kbChars[kbCursor]; 
            if(selected == '<') { 
                if(teamDraft.length() > 0) teamDraft.remove(teamDraft.length()-1); 
            } else if(teamDraft.length() < 10) {
                teamDraft += selected; 
            }
            requestUIUpdate = true; 
            fullRefreshNeeded = false; 
        }
    }
    else if (currentState == PAGE_MENU_POWER) {
        if (pA) { if (menuSelection > 0) menuSelection--; else menuSelection = 2; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pC) { if (menuSelection < 2) menuSelection++; else menuSelection = 0; requestUIUpdate = true; fullRefreshNeeded = false; }
        else if (pB) { 
            currentPowerMode = (PowerMode)menuSelection; 
            
            if (xSemaphoreTake(i2cMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                if (currentPowerMode == NORMAL_MODE) { 
                    setCpuFrequencyMhz(80); 
                    myGNSS.powerSaveMode(false); 
                    myGNSS.setMeasurementRate(1000); 
                }
                else if (currentPowerMode == ECO_MODE) { 
                    myGNSS.powerSaveMode(true); 
                    myGNSS.setMeasurementRate(10000); 
                    setCpuFrequencyMhz(40); 
                }
                else if (currentPowerMode == STEALTH_MODE) { 
                    myGNSS.powerSaveMode(true); 
                    myGNSS.setMeasurementRate(10000); 
                    radio.standby(); 
                    loraListening = false; 
                    setCpuFrequencyMhz(40); 
                }
                xSemaphoreGive(i2cMutex);
            }
            
            currentState = PAGE_MENU_MAIN; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
        }
    }
    else if (currentState == PAGE_MENU_LANG) {
        if (pA) { if (tempLangSelection > 0) tempLangSelection--; else tempLangSelection = 9; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pC) { if (tempLangSelection < 9) tempLangSelection++; else tempLangSelection = 0; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pB) { 
            currentLang = tempLangSelection; 
            prefs.putInt("lang", currentLang); 
            currentState = PAGE_MENU_MAIN; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
        }
    }
    else if (currentState == PAGE_MENU_TIME) {

        if (pA) { if (tempTimeSelection > -12) tempTimeSelection--; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pC) { if (tempTimeSelection < 14) tempTimeSelection++; requestUIUpdate = true; fullRefreshNeeded = false; } 
        else if (pB) { 
            timeOffset = tempTimeSelection; 
            prefs.putInt("gmt", timeOffset); 
            currentState = PAGE_MENU_MAIN; 
            fullRefreshNeeded = false; 
            requestUIUpdate = true; 
        }
    }
}