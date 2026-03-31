void acquireSD() { 
    digitalWrite(EDP_CS, HIGH); 
    delay(1); 
}

long long getKmlTimestamp(String fname) {
    String nameLower = fname;
    nameLower.toLowerCase();
    int start = nameLower.indexOf("route_");
    if (start < 0 || fname.length() < (start + 22)) return 0; 
    start += 6; 
    long long d   = fname.substring(start + 0,  start + 2).toInt();  
    long long m   = fname.substring(start + 3,  start + 5).toInt();  
    long long y   = fname.substring(start + 6,  start + 10).toInt(); 
    long long h   = fname.substring(start + 11, start + 13).toInt(); 
    long long min = fname.substring(start + 14, start + 16).toInt(); 
    return (y * 100000000LL) + (m * 1000000LL) + (d * 10000LL) + (h * 100) + min;
}

void scanKMLFiles() {
    kmlFileCount = 0; 
    kmlFiles[kmlFileCount++] = "[ NO OVERLAY ]"; 
    
    if (!sdDetected) return; 
    acquireSD(); 
    
    File root = SD.open("/"); 
    if (!root) return;
    
    String tempFiles[MAX_KML_FILES];
    int tempCount = 0;
    
    root.rewindDirectory();
    while (true) {
        File file = root.openNextFile();
        if (!file) break;
        if (!file.isDirectory()) {
            String fname = String(file.name()); 
            String fnameLower = fname; 
            fnameLower.toLowerCase();
            if (fnameLower.endsWith(".kml")) {
                if (tempCount < MAX_KML_FILES - 1) {
                    tempFiles[tempCount++] = fname;
                }
            }
        }
        file.close();
    }
    root.close();

    for (int i = 0; i < tempCount - 1; i++) {
        for (int j = i + 1; j < tempCount; j++) {
            if (getKmlTimestamp(tempFiles[i]) < getKmlTimestamp(tempFiles[j])) {
                String t = tempFiles[i];
                tempFiles[i] = tempFiles[j];
                tempFiles[j] = t;
            }
        }
    }
    
    for (int i = 0; i < tempCount; i++) {
        if (kmlFileCount < MAX_KML_FILES) {
            kmlFiles[kmlFileCount++] = tempFiles[i];
        }
    }
}

void parseKmlToken(String t, float &lastLatParsed, float &lastLonParsed) {
    t.trim();
    if (t.length() > 3) {
        int comma1 = t.indexOf(',');
        if (comma1 > 0) {
            float lon = t.substring(0, comma1).toFloat();
            int comma2 = t.indexOf(',', comma1 + 1);
            float lat = (comma2 > 0) ? t.substring(comma1 + 1, comma2).toFloat() : t.substring(comma1 + 1).toFloat();
            
            if (lon != 0.0 && lat != 0.0) {
                if (lat < kmlCacheMinLat) kmlCacheMinLat = lat; 
                if (lat > kmlCacheMaxLat) kmlCacheMaxLat = lat;
                if (lon < kmlCacheMinLon) kmlCacheMinLon = lon; 
                if (lon > kmlCacheMaxLon) kmlCacheMaxLon = lon;
                
                if (lastLatParsed != -999 && lastLonParsed != -999) {
                    float legDist = calculateDistance(lastLatParsed, lastLonParsed, lat, lon);
                    if (legDist < 50.0) { 
                        kmlCacheDistance += legDist; 
                    }
                }
                lastLatParsed = lat; 
                lastLonParsed = lon;
            }
        }
    }
}

void loadKMLCache() {
    kmlCacheMinLat = 90; 
    kmlCacheMaxLat = -90; 
    kmlCacheMinLon = 180; 
    kmlCacheMaxLon = -180; 
    kmlCacheDistance = 0.0; 
    kmlCacheValid = false;
    
    if (currentKmlOverlay == "" || currentKmlOverlay == "[ NO OVERLAY ]" || !sdDetected) return;
    
    acquireSD(); 
    String path = currentKmlOverlay;
    if (!path.startsWith("/")) path = "/" + path;
    path.replace("//", "/"); 
    
    File f = SD.open(path); 
    if (!f) return;
    
    bool inCoords = false; 
    String token = ""; 
    float lastLatParsed = -999, lastLonParsed = -999;
    
    while (f.available()) {
        char c = f.read();
        if (c == '<') {
            if (inCoords && token.length() > 0) { 
                parseKmlToken(token, lastLatParsed, lastLonParsed); 
                token = ""; 
            }
            String tag = "<"; 
            while (f.available()) { 
                c = f.read(); 
                tag += c; 
                if (c == '>') break; 
            } 
            if (tag.indexOf("<coordinates>") != -1) {
                inCoords = true;
            } else if (tag.indexOf("</coordinates>") != -1) { 
                inCoords = false; 
                lastLatParsed = -999; 
                lastLonParsed = -999; 
            }
        } else if (inCoords) {
            if (isspace(c)) {
                if (token.length() > 0) { 
                    parseKmlToken(token, lastLatParsed, lastLonParsed); 
                    token = ""; 
                }
            } else {
                if (token.length() < 64) token += c; 
            }
        }
    }
    f.close(); 
    
    if (kmlCacheMinLat != 90 && kmlCacheMaxLat != -90) {
        kmlCacheValid = true;
    }
}

void drawKMLOverlay(String path, double centerLat, double centerLon, float scale, float cosLat, int cx, int cy, int mx, int my, int mw, int mh) {
    if (path == "" || path == "[ NO OVERLAY ]" || !sdDetected) return;
    
    acquireSD(); 
    if (!path.startsWith("/")) path = "/" + path;
    path.replace("//", "/"); 

    File f = SD.open(path); 
    if (!f) return;
    
    bool inCoords = false; 
    String token = ""; 
    bool isFirstPoint = true; 
    int prevX = 0, prevY = 0;
    
    auto parseAndDraw = [&](String t) {
        t.trim();
        int c1 = t.indexOf(',');
        if (c1 > 0) {
            float lon = t.substring(0, c1).toFloat();
            int c2 = t.indexOf(',', c1 + 1);
            float lat = (c2 > 0) ? t.substring(c1 + 1, c2).toFloat() : t.substring(c1 + 1).toFloat();
            
            if (lon != 0.0 && lat != 0.0) {
                long px_raw = cx + (lon - centerLon) * scale * cosLat;
                long py_raw = cy - (lat - centerLat) * scale;
                
                if (px_raw > 30000) px_raw = 30000;
                if (px_raw < -30000) px_raw = -30000;
                if (py_raw > 30000) py_raw = 30000;
                if (py_raw < -30000) py_raw = -30000;
                
                int px = (int)px_raw;
                int py = (int)py_raw;

                if (!isFirstPoint) {
                    display.drawLine(prevX, prevY, px, py, GxEPD_BLACK);
                } else {
                    isFirstPoint = false;
                }
                
                prevX = px; 
                prevY = py;
            }
        }
    };

    while (f.available()) {
        char c = f.read();
        if (c == '<') {
            if (inCoords && token.length() > 0) { parseAndDraw(token); token = ""; }
            String tag = "<"; 
            while (f.available()) { 
                c = f.read();
                tag += c;
                if (c == '>') break;
            } 
            if (tag.indexOf("<coordinates>") != -1) {
                inCoords = true;
            } else if (tag.indexOf("</coordinates>") != -1) { 
                inCoords = false; 
                isFirstPoint = true; 
            }
        } else if (inCoords) {
            if (isspace(c)) {
                if (token.length() > 0) { parseAndDraw(token); token = ""; }
            } else {
                if (token.length() < 64) token += c;
            }
        }
    }
    f.close();
}

void logSystemData(String sysLog) {
    if (!sdDetected || !isRecording) return; 
    acquireSD(); 
    char dateStr[16] = "00_00_0000";
    char timeStr[16] = "00:00:00";
    
    if ((gps_ok || simActive) && gpsTimeValid) {
        snprintf(dateStr, sizeof(dateStr), "%02d_%02d_%04d", gpsDay, gpsMonth, gpsYear);
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", (gpsHour + timeOffset + 24) % 24, gpsMinute % 60, gpsSecond);
    }
    
    String csvPath = "/sys_" + String(dateStr) + ".csv";
    File f = SD.open(csvPath, FILE_APPEND);
    if (f) {
        f.printf("%s,%s,%.7f,%.7f,%.1f,%.1f,%.2f,%.2f,%.2f,%s\n", dateStr, timeStr, lastLat, lastLon, currentTemp, currentHum, currentAx, currentAy, currentAz, sysLog.c_str()); 
        f.flush(); 
        f.close();
    }
}

void logGPS(double lat, double lon, float alt) {
    if (lastAccuracy > 100.0) return;

    if (lastLoggedLat != 0.0 && lastLoggedLon != 0.0) { 
        double dKm = calculateDistance(lastLoggedLat, lastLoggedLon, lat, lon); 
        if (dKm * 1000.0 < 0.2) return; 
        
        if (isRecording) {
            sessionTotalDistance += dKm; 
        }
         requestUIUpdate = true; 
    }
    lastLoggedLat = lat; lastLoggedLon = lon;
    

    if (breadcrumbIdx < MAX_BREADCRUMBS) {
        breadcrumbs[breadcrumbIdx++] = {(float)lat, (float)lon}; 
    } else {
    
        for (int i = 1; i < MAX_BREADCRUMBS; i++) {
            breadcrumbs[i-1] = breadcrumbs[i];
        }
        breadcrumbs[MAX_BREADCRUMBS - 1] = {(float)lat, (float)lon};
    }

  
    if (isRecording && sdDetected && strlen(currentRecordDate) > 0) {
        acquireSD(); 
        String kmlPath = "/route_" + String(currentRecordDate) + ".kml";
        File f = SD.open(kmlPath, "r+"); 
        if (f) { 
            String footer = "</coordinates>";
            long fileSize = f.size();
            long searchPos = (fileSize > 256) ? fileSize - 256 : 0;
            f.seek(searchPos);
            
            String tail = "";
            while(f.available()) tail += (char)f.read();
            
            int tagIdx = tail.lastIndexOf(footer);
            if (tagIdx != -1) {
                f.seek(searchPos + tagIdx);
                f.print(String(lon, 7) + "," + String(lat, 7) + "," + String(alt, 1) + "\n"); 
                f.print(KML_CLOSING); 
            }
            f.close(); 
        }
    }
}

void logLoraMessage(String msg, bool isEncrypted) {
    if (!sdDetected) return; 
    acquireSD();
    char dateStr[16] = "00_00_0000";
    if ((gps_ok || simActive) && gpsTimeValid) { 
        snprintf(dateStr, sizeof(dateStr), "%02d_%02d_%04d", gpsDay, gpsMonth, gpsYear); 
    }
    String path = isEncrypted ? "/sec_" + String(dateStr) + ".txt" : "/pub_" + String(dateStr) + ".txt";
    File f = SD.open(path, FILE_APPEND); 
    if (f) { 
        f.println(msg); 
        f.flush(); 
        f.close(); 
    }
}

void checkSDHotplug() {
    acquireSD();
    if (sdDetected) { 
        File root = SD.open("/"); 
        if (root) { 
            root.close(); 
        } else { 
            SD.end(); 
            sdDetected = false; 
            return; 
        } 
    }
    if (!sdDetected) { 
        if (SD.begin(SDCARD_CS, SDSPI, 2000000)) {
            sdDetected = true;
        }
    }
}