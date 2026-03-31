

void getAESKey(byte* key) {
    String k = (myTeam.length() > 0) ? myTeam : "ALPHA";
    mbedtls_md_context_t ctx;
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const unsigned char *)k.c_str(), k.length());
    mbedtls_md_finish(&ctx, key);
    mbedtls_md_free(&ctx);
}


String bufferToHex(byte* data, int len) {
    String res = "";
    for (int i = 0; i < len; i++) {
        if (data[i] < 0x10) res += "0";
        res += String(data[i], HEX);
    }
    return res;
}

void hexToBuffer(String hex, byte* output) {
    for (int i = 0; i < hex.length(); i += 2) {
        output[i / 2] = (byte)strtol(hex.substring(i, i + 2).c_str(), NULL, 16);
    }
}

String cryptMsg(String input) {
    byte key[32];
    getAESKey(key);

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, key, 256);

    byte iv[16];
    for (int i = 0; i < 16; i++) iv[i] = esp_random() % 256; 

    int inputLen = input.length();
    int padLen = 16 - (inputLen % 16);
    int paddedLen = inputLen + padLen;

    byte* paddedInput = new byte[paddedLen];
    memcpy(paddedInput, input.c_str(), inputLen);
    for (int i = 0; i < padLen; i++) paddedInput[inputLen + i] = padLen; 

    byte* output = new byte[paddedLen];
    byte iv_copy[16];
    memcpy(iv_copy, iv, 16); 
    
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, paddedLen, iv_copy, paddedInput, output);
    String result = bufferToHex(iv, 16) + bufferToHex(output, paddedLen);

    delete[] paddedInput;
    delete[] output;
    mbedtls_aes_free(&aes);

    return result;
}

String decryptMsg(String input) {

    if (input.length() < 64 || input.length() % 2 != 0) return "";

    byte key[32];
    getAESKey(key);

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_dec(&aes, key, 256);
    
    byte iv[16];
    hexToBuffer(input.substring(0, 32), iv);

    int cipherLen = (input.length() - 32) / 2;
    byte* ciphertext = new byte[cipherLen];
    hexToBuffer(input.substring(32), ciphertext);

    byte* output = new byte[cipherLen];
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, cipherLen, iv, ciphertext, output);

    int padLen = output[cipherLen - 1];
    String result = "";
    if (padLen > 0 && padLen <= 16) {
        for (int i = 0; i < cipherLen - padLen; i++) {
            result += (char)output[i];
        }
    }
    
    delete[] ciphertext;
    delete[] output;
    mbedtls_aes_free(&aes);

    return result;
}

void executeSendMsg() {
    if (msgDraft.length() == 0 || currentPowerMode == STEALTH_MODE) return;

    String fullMsg = myName + ": " + msgDraft; 
    String msgData = fullMsg + "|" + String(lastLat, 5) + "," + String(lastLon, 5);

    String msgToSend = "";
    if (secureMode) {
        msgToSend += (char)0xFF; 
        msgToSend += cryptMsg(msgData);
    } else {
        msgToSend = msgData;
    }

    radio.standby(); 
    radio.setSpreadingFactor(11); 
    radio.transmit(msgToSend); 

    String timeStr = getSystemTimeStr();
    String screenMsg = ">> [" + timeStr + "] " + fullMsg;
    for(int i = MAX_LORA_MSGS - 1; i > 0; i--) {
        loraHistory[i] = loraHistory[i-1]; 
    }
    loraHistory[0] = screenMsg; 
    if (loraMsgCount < MAX_LORA_MSGS) loraMsgCount++;
    notifyPhone("[TX]: " + fullMsg); 

    radio.startReceive(); 
    loraListening = true; 
    
    msgDraft = "";
    currentState = PAGE_LORA;
    fullRefreshNeeded = false; 
    requestUIUpdate = true; 
    pingActivity();
}