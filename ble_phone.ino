
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        BLEDevice::startAdvertising(); 
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        String rxValue = pCharacteristic->getValue().c_str();
        if (rxValue.length() > 0) {
            portENTER_CRITICAL(&bleMux);
            bleRxBuffer = rxValue;
            bleRxBuffer.trim(); 
            bleDataReceived = true;
            portEXIT_CRITICAL(&bleMux);
        }
    }
};

void initBLE() {
    BLEDevice::init("GLYPH");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pTxCharacteristic = pService->createCharacteristic(
                            CHARACTERISTIC_UUID_TX,
                            BLECharacteristic::PROPERTY_NOTIFY
                        );
    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
                                                CHARACTERISTIC_UUID_RX,
                                                BLECharacteristic::PROPERTY_WRITE
                                            );
    pRxCharacteristic->setCallbacks(new MyCallbacks());
    pService->start();
    pServer->getAdvertising()->start();
}

void notifyPhone(String msg) { 
    if (deviceConnected && pTxCharacteristic) {
        pTxCharacteristic->setValue(msg.c_str());
        pTxCharacteristic->notify();
    }
}

void sendTelemetryBLE() {
    static unsigned long lastBleTelemetry = 0;
    
    if (deviceConnected && millis() - lastBleTelemetry > 5000) {
        lastBleTelemetry = millis();
        
        int p = constrain((int)((batVoltage - 3.1) / 1.1 * 100.0), 0, 100);
        notifyPhone("SYS_BATT:" + String(p));
        notifyPhone("SYS_SATS:" + String(lastSIV));

        if (hasGpsFix || simActive) {
            notifyPhone("SYS_GPS:" + String(lastLat, 6) + "," + String(lastLon, 6));
        }

        notifyPhone("SYS_ENV:" + String(currentTemp, 1) + "," + String(currentHum, 1));
        
        if (gpsTimeValid) {
            char tBuf[16];
            snprintf(tBuf, sizeof(tBuf), "%02d:%02d", (gpsHour + timeOffset + 24) % 24, gpsMinute);
            notifyPhone("SYS_TIME:" + String(tBuf));
        }

        notifyPhone(isRecording ? "SYS_REC:1" : "SYS_REC:0");
    }
}

void sendInputPrompt(String msg) { 
    notifyPhone(">> " + msg); 
}

void checkBLEInput() {
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = true;
        phoneConnectedTime = millis();
        introMode = true;
        promptNeedsResend = true;

        notifyPhone(isRecording ? "SYS_REC:1" : "SYS_REC:0");
    }
    
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); 
        pServer->startAdvertising(); 
        oldDeviceConnected = false;
        introMode = false;
        promptNeedsResend = false;
    }

    if (introMode && (millis() - phoneConnectedTime > 10000)) {
        introMode = false;
    }

    if (bleDataReceived) {
        String btMsg;
        portENTER_CRITICAL(&bleMux);
        btMsg = bleRxBuffer;
        bleRxBuffer = ""; 
        bleDataReceived = false;
        portEXIT_CRITICAL(&bleMux);

        if (introMode) introMode = false; 
        if (btMsg == "INPUT") return;
        
        pingActivity();
        
        if (!processVirtualCommand(btMsg)) {
            bool inSetup = (currentState == PAGE_INIT_LANG || currentState == PAGE_INIT_FREQ || currentState == PAGE_INIT_NAME || currentState == PAGE_INIT_TIME || currentState == PAGE_INIT_COMPASS);
            
            if (inSetup) {
                processInitInput(btMsg);
            }
            else if (currentState == PAGE_KEYBOARD) { 
                msgDraft = btMsg; 
                if(msgDraft.length() > 30) msgDraft = msgDraft.substring(0, 30); 
                requestUIUpdate = true; 
                fullRefreshNeeded = false; 
            }
            else if (currentState == PAGE_TEAM_NAME_EDIT) {
                teamDraft = btMsg; 
                if(teamDraft.length() > 10) teamDraft = teamDraft.substring(0, 10); 
                myTeam = teamDraft; 
                if(myTeam.length() == 0) myTeam = "ALPHA"; 
                prefs.putString("team", myTeam); 
                currentState = PAGE_MENU_TEAM; 
                fullRefreshNeeded = true; 
                requestUIUpdate = true; 
            }
            else if (currentPowerMode != STEALTH_MODE) {
                String tmp = msgDraft;
                msgDraft = btMsg;
                if(msgDraft.length() > 30) msgDraft = msgDraft.substring(0, 30);
                executeSendMsg();
                msgDraft = tmp;
            }
        }
    }
}