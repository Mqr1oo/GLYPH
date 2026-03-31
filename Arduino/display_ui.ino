void drawSidebarBtn(int cx, int y, String btn, String action) {
    u8g2Fonts.setFont(u8g2_font_5x7_tf);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    int btnW = u8g2Fonts.getUTF8Width(btn.c_str());
    int actW = u8g2Fonts.getUTF8Width(action.c_str());
    u8g2Fonts.setCursor(cx - btnW/2, y);
    u8g2Fonts.print(btn);
    if (action.length() > 0) {
        u8g2Fonts.setCursor(cx - actW/2, y + 8);
        u8g2Fonts.print(action);
    }
}

void drawHeader(String title, bool isMap = false) {
    if (!isMap) {
        display.fillScreen(GxEPD_WHITE); 
    }
    
    int scrW = display.width(); 
    String timeStr = getSystemTimeStr(); 
    
    int p = (int)((batVoltage - 3.1) / 1.1 * 100.0);
    p = constrain(p, 0, 100);
    String batStr = String(p) + "%";

    if (!sdDetected && !isMap) {
        if (title == "GLYPH") title = "GLYPH - NO SD";
        else title += " (NO SD)";
    }

    if (isMap) {
        u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
        u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        
        int timeW = u8g2Fonts.getUTF8Width(timeStr.c_str());
        display.fillRect(4, 2, timeW + 4, 14, GxEPD_WHITE); 
        u8g2Fonts.setCursor(6, 14); 
        u8g2Fonts.print(timeStr.c_str());

        u8g2Fonts.setFont(u8g2_font_helvB08_tf); 
        int batW = u8g2Fonts.getUTF8Width(batStr.c_str());
        display.fillRect(scrW - batW - 4, 2, batW + 4, 12, GxEPD_WHITE);
        u8g2Fonts.setCursor(scrW - batW - 2, 12);
        u8g2Fonts.print(batStr);
    } else {
        display.fillRoundRect(0, 0, scrW, 20, 0, GxEPD_BLACK); 
        u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
        
        if (title.startsWith("GLYPH")) { 
            if (!sdDetected) u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
            else u8g2Fonts.setFont(u8g2_font_helvB12_tf); 
            u8g2Fonts.setCursor(6, 16); 
        } else { 
            u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
            u8g2Fonts.setCursor(6, 15); 
        }
        u8g2Fonts.print(title);
        
        u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
        int timeW = u8g2Fonts.getUTF8Width(timeStr.c_str()); 
        int batW = u8g2Fonts.getUTF8Width(batStr.c_str());
        u8g2Fonts.setCursor(scrW - batW - 6, 15); 
        u8g2Fonts.print(batStr);
        u8g2Fonts.setCursor(scrW - batW - timeW - 14, 15); 
        u8g2Fonts.print(timeStr.c_str()); 
    }
}

void renderMap() {
    display.fillScreen(GxEPD_WHITE);
    
    int scrW = display.width(); 
    int scrH = display.height(); 
    int rightPanelW = 30;
    int contentW = scrW - rightPanelW;
    int cx = contentW + rightPanelW / 2;
    
    display.drawLine(contentW, 0, contentW, scrH, GxEPD_BLACK);
    display.drawLine(contentW, 20, scrW, 20, GxEPD_BLACK);

    for(int i=10; i<contentW; i+=40) {
        for(int j=10; j<scrH; j+=40) {
            display.drawPixel(i,j, GxEPD_BLACK);
        }
    }

    int mapCX = contentW / 2;
    int mapCY = scrH / 2;
    int userX = mapCX;
    int userY = mapCY;

    // --- REPARATIE KML: Incarcam cache-ul KML inainte de a desena ---
    static String lastRenderedKml = "";
    if (currentKmlOverlay != lastRenderedKml) {
        lastRenderedKml = currentKmlOverlay;
        kmlCacheValid = false;
    }
    if (currentKmlOverlay != "" && !kmlCacheValid) {
        loadKMLCache();
    }

    // Acum verificam corect daca avem motiv sa desenam harta
    bool hasMapContent = hasGpsFix || breadcrumbIdx > 0 || simActive || (currentKmlOverlay != "" && kmlCacheValid);

    if (hasMapContent) {
        double centerLat = lastLat;
        double centerLon = lastLon;
        double scale = 1000.0;
        
        // Daca nu avem GPS dar avem KML, centram harta pe fisierul KML
        if (!(hasGpsFix || simActive) && currentKmlOverlay != "" && kmlCacheValid) {
            centerLat = (kmlCacheMinLat + kmlCacheMaxLat) / 2.0;
            centerLon = (kmlCacheMinLon + kmlCacheMaxLon) / 2.0;
        }

        if (mapZoomLevel == 0) { 
            double minLat = centerLat, maxLat = centerLat;
            double minLon = centerLon, maxLon = centerLon;
            
            if (hasGpsFix || breadcrumbIdx > 0) {
                minLat = lastLat; maxLat = lastLat;
                minLon = lastLon; maxLon = lastLon;
                for(int i=0; i < breadcrumbIdx; i++){
                    if (breadcrumbs[i].lat < minLat) minLat = breadcrumbs[i].lat;
                    if (breadcrumbs[i].lat > maxLat) maxLat = breadcrumbs[i].lat;
                    if (breadcrumbs[i].lon < minLon) minLon = breadcrumbs[i].lon;
                    if (breadcrumbs[i].lon > maxLon) maxLon = breadcrumbs[i].lon;
                }
            } else if (currentKmlOverlay != "" && kmlCacheValid) {
                minLat = kmlCacheMinLat; maxLat = kmlCacheMaxLat;
                minLon = kmlCacheMinLon; maxLon = kmlCacheMaxLon;
            }

            if (currentKmlOverlay != "" && kmlCacheValid) {
                if (kmlCacheMinLat < minLat) minLat = kmlCacheMinLat;
                if (kmlCacheMaxLat > maxLat) maxLat = kmlCacheMaxLat;
                if (kmlCacheMinLon < minLon) minLon = kmlCacheMinLon;
                if (kmlCacheMaxLon > maxLon) maxLon = kmlCacheMaxLon;
            }
            double dLat = maxLat - minLat;
            double dLon = maxLon - minLon;
            dLat *= 1.2; dLon *= 1.2; 
            if (dLat < 0.0005) dLat = 0.0005; 
            if (dLon < 0.0005) dLon = 0.0005;
            centerLat = (minLat + maxLat) / 2.0;
            centerLon = (minLon + maxLon) / 2.0;
            double cosL = cos(centerLat * PI / 180.0);
            double scaleLat = (scrH / 2.0 - 15) / (dLat / 2.0);
            double scaleLon = (contentW / 2.0 - 15) / ((dLon / 2.0) * cosL);
            scale = min(scaleLat, scaleLon);

        } else if (mapZoomLevel == 1) { 
            if (hasGpsFix || simActive) {
                centerLat = lastLat;
                centerLon = lastLon;
            } else if (currentKmlOverlay != "" && kmlCacheValid) {
                centerLat = (kmlCacheMinLat + kmlCacheMaxLat) / 2.0;
                centerLon = (kmlCacheMinLon + kmlCacheMaxLon) / 2.0;
            }

            // ZOOM LA ~10 METRI (Raza) IN JURUL TAU (0.00018 grade = ~20 metri cadru total)
            double dLat = 0.00018; 
            double dLon = 0.00018; 
            
            double cosL = cos(centerLat * PI / 180.0);
            double scaleLat = (scrH / 2.0 - 15) / (dLat / 2.0);
            double scaleLon = (contentW / 2.0 - 15) / ((dLon / 2.0) * cosL);
            scale = min(scaleLat, scaleLon);
        }

        double cosLat = cos(centerLat * PI / 180.0);
        mapCX = contentW / 2;
        mapCY = scrH / 2;

        // Deseneaza overlay-ul de pe SD
        if(currentKmlOverlay != "" && sdDetected) {
            drawKMLOverlay(currentKmlOverlay, centerLat, centerLon, scale, cosLat, mapCX, mapCY, 0, 0, contentW, scrH);
        }

        // NOU: Deseneaza traseul tau curent DIRECT de pe cardul SD, ca sa nu dispara niciodata linia!
        if (isRecording && sdDetected && strlen(currentRecordDate) > 0) {
            String activeKml = "/route_" + String(currentRecordDate) + ".kml";
            if (activeKml != currentKmlOverlay) {
                drawKMLOverlay(activeKml, centerLat, centerLon, scale, cosLat, mapCX, mapCY, 0, 0, contentW, scrH);
            }
        }

        // Deseneaza ruta inregistrata curent (breadcrumbs)
        if (breadcrumbIdx > 1) {
            for(int i=0; i < breadcrumbIdx - 1; i++){
                int x1 = mapCX + (breadcrumbs[i].lon - centerLon) * scale * cosLat; 
                int y1 = mapCY - (breadcrumbs[i].lat - centerLat) * scale;
                int x2 = mapCX + (breadcrumbs[i+1].lon - centerLon) * scale * cosLat; 
                int y2 = mapCY - (breadcrumbs[i+1].lat - centerLat) * scale;
                display.drawLine(x1, y1, x2, y2, GxEPD_BLACK); 
            }
        }
        userX = mapCX + (lastLon - centerLon) * scale * cosLat;
        userY = mapCY - (lastLat - centerLat) * scale;
        
        if (hasGpsFix || simActive) {
            display.fillCircle(userX, userY, 4, GxEPD_BLACK); 
            display.fillCircle(userX, userY, 2, GxEPD_WHITE); 
        }
    }

    if(isRecording) {
        display.fillRoundRect(4, scrH - 24, 48, 20, 4, GxEPD_BLACK);
        display.fillCircle(14, scrH - 14, 4, GxEPD_WHITE);
        u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
        u8g2Fonts.setFont(u8g2_font_helvB08_tf); 
        u8g2Fonts.setCursor(22, scrH - 10); 
        u8g2Fonts.print("REC");
    }

    if (!(hasGpsFix || simActive)) {
        u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        u8g2Fonts.setFont(u8g2_font_helvB10_tf);
        int tw = u8g2Fonts.getUTF8Width(tr_gps[currentLang]); 
        display.fillRect((contentW - tw)/2 - 2, 23, tw + 4, 16, GxEPD_WHITE);
        u8g2Fonts.setCursor((contentW - tw)/2, 35); 
        u8g2Fonts.print(tr_gps[currentLang]); 
    }

    display.fillRect(contentW, 0, rightPanelW, scrH, GxEPD_WHITE);
    display.drawLine(contentW, 0, contentW, scrH, GxEPD_BLACK);
    display.drawLine(contentW, 20, scrW, 20, GxEPD_BLACK);

    drawHeader("", true); 

    double displayDistKm = isRecording ? sessionTotalDistance : ((currentKmlOverlay != "" && kmlCacheValid) ? kmlCacheDistance : sessionTotalDistance);
    
    auto formatDist = [](double dKm) -> String {
        if (dKm <= 0.001) return "0m";
        if (dKm < 1.0) return String((int)(dKm * 1000.0)) + "m";
        return String(dKm, 2) + "km";
    };
    String distStr = formatDist(displayDistKm);
    
    u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
    int dw = u8g2Fonts.getUTF8Width(distStr.c_str()); 
    
    display.fillRect(contentW - dw - 8, scrH - 15, dw + 4, 15, GxEPD_WHITE); 
    u8g2Fonts.setCursor(contentW - dw - 6, scrH - 8); 
    u8g2Fonts.print(distStr.c_str());

    String zoomTxt = (mapZoomLevel == 0) ? "FIT" : "ZOOM";
    drawSidebarBtn(cx, 30, "A", "EXT");
    drawSidebarBtn(cx, 58, "C", "KML");
    drawSidebarBtn(cx, 86, "B", zoomTxt);
    drawSidebarBtn(cx, 110, "AB", "REC");
}

void renderKmlList() {
    drawHeader("EXPLORED", false);
    int scrW = display.width(); 
    int scrH = display.height();
    int rightPanelW = 30;
    int contentW = scrW - rightPanelW;
    int cx = contentW + rightPanelW / 2;

    display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);
    u8g2Fonts.setFont(u8g2_font_6x10_tf);

    int startY = 35; 
    int maxVisible = 6; 
    int startIdx = 0;
    
    // Corectare UI pentru cazul în care fișierele nu sunt găsite / SD-ul a fost scos
    if (kmlFileCount == 0) {
        display.fillRoundRect(4, startY - 10, contentW - 8, 14, 2, GxEPD_BLACK);
        u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
        u8g2Fonts.setCursor(8, startY); 
        u8g2Fonts.print("< NO OVERLAY >");
        
        u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        u8g2Fonts.setCursor(8, startY + 25);
        if (!sdDetected) {
            u8g2Fonts.print("SD CARD ERROR!");
        } else {
            u8g2Fonts.print("NO KML FOUND!");
        }
        kmlSelectionIndex = 0; // Forțăm indexul valid
    } else {
        if (kmlSelectionIndex > maxVisible - 1) {
            startIdx = kmlSelectionIndex - (maxVisible - 1);
        }

        for(int i = 0; i < maxVisible; i++) {
            int idx = startIdx + i; 
            if (idx >= kmlFileCount) break;
            int y = startY + (i * 16);
            if (idx == kmlSelectionIndex) {
                display.fillRoundRect(4, y - 10, contentW - 8, 14, 2, GxEPD_BLACK);
                u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
                u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
            } else {
                u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
                u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
            }
            String fName = kmlFiles[idx];
            if(fName.startsWith("/")) fName = fName.substring(1); 
            fName.replace(".kml", ""); 
            u8g2Fonts.setCursor(8, y); 
            u8g2Fonts.print(fName.c_str()); 
        }
    }

    drawSidebarBtn(cx, 30, "A", "UP");
    drawSidebarBtn(cx, 58, "C", "DWN");
    drawSidebarBtn(cx, 86, "B", "SEL");
    drawSidebarBtn(cx, 110, "hA", "EXT");
}

void renderClock() {
    drawHeader(tr_sensors_title[currentLang], false); 
    int scrW = display.width(); 
    int scrH = display.height();
    int rightPanelW = 30;
    int contentW = scrW - rightPanelW;
    int cx = contentW + rightPanelW / 2;
    
    display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);

    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    
    String timeStr = getSystemTimeStr(); 
    char fullDate[32] = "--/--/--";
    
    if((gps_ok || simActive) && gpsTimeValid){
        snprintf(fullDate, sizeof(fullDate), "%02d/%02d/%02d", gpsDay, gpsMonth, gpsYear % 100);
    }
    
    int startY = 48; 
    u8g2Fonts.setFont(u8g2_font_logisoso24_tn); 
    u8g2Fonts.setCursor((contentW - u8g2Fonts.getUTF8Width(timeStr.c_str())) / 2, startY); 
    u8g2Fonts.print(timeStr.c_str());
    
    startY += 20; 
    u8g2Fonts.setFont(u8g2_font_6x10_tf); 
    char dateInfo[64];
    snprintf(dateInfo, sizeof(dateInfo), "DATE: %s", fullDate);
    u8g2Fonts.setCursor((contentW - u8g2Fonts.getUTF8Width(dateInfo)) / 2, startY); 
    u8g2Fonts.print(dateInfo);
    
    startY += 14; 
    char gpsInfo[64]; 
    snprintf(gpsInfo, sizeof(gpsInfo), "LAT: %.4f | LON: %.4f", lastLat, lastLon); 
    u8g2Fonts.setCursor((contentW - u8g2Fonts.getUTF8Width(gpsInfo)) / 2, startY); 
    u8g2Fonts.print(gpsInfo);

    startY += 14; 
    char altInfo[64];
    snprintf(altInfo, sizeof(altInfo), "ALT: %.1fm | SATS: %d", lastAlt, lastSIV);
    u8g2Fonts.setCursor((contentW - u8g2Fonts.getUTF8Width(altInfo)) / 2, startY);
    u8g2Fonts.print(altInfo);
    
    startY += 14; 
    char envInfo[64]; 
    float tempF = (currentTemp * 9.0 / 5.0) + 32.0; 
    snprintf(envInfo, sizeof(envInfo), "T: %.1fC/%.1fF | H: %.1f%%", currentTemp, tempF, currentHum); 
    u8g2Fonts.setCursor((contentW - u8g2Fonts.getUTF8Width(envInfo)) / 2, startY); 
    u8g2Fonts.print(envInfo);

    drawSidebarBtn(cx, 60, "A", "EXT");
}

void renderLoRa() {
    drawHeader(secureMode ? tr_secure_rx[currentLang] : tr_public_rx[currentLang], false);
    int scrW = display.width();
    int scrH = display.height();
    int rightPanelW = 30;
    int contentW = scrW - rightPanelW;
    int cx = contentW + rightPanelW / 2;

    display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    
    if(loraMsgCount == 0) { 
        u8g2Fonts.setFont(u8g2_font_helvB08_tf);
        int tw = u8g2Fonts.getUTF8Width(tr_no_msg[currentLang]);
        u8g2Fonts.setCursor((contentW - tw)/2, 70); 
        u8g2Fonts.print(tr_no_msg[currentLang]); 
    } else {
        u8g2Fonts.setFont(u8g2_font_6x10_tf);
        int yPos = 25;
        for(int i = 0; i < loraMsgCount; i++) {
            if (yPos > scrH - 5) break;
            String text = loraHistory[i];
            int startY = yPos;
            while(text.length() > 0 && yPos < scrH) {
                int splitIdx = text.length();
                while(splitIdx > 0 && u8g2Fonts.getUTF8Width(text.substring(0, splitIdx).c_str()) > contentW - 16) {
                    splitIdx--;
                }
                if (splitIdx < text.length()) {
                    int lastSpace = text.substring(0, splitIdx).lastIndexOf(' ');
                    if (lastSpace > 0) splitIdx = lastSpace;
                }
                if (splitIdx == 0) splitIdx = 1;
                String line = text.substring(0, splitIdx);
                u8g2Fonts.setCursor(8, yPos + 10);
                u8g2Fonts.print(line.c_str());
                yPos += 14; 
                text = text.substring(splitIdx);
                text.trim();
            }
            display.drawRoundRect(4, startY, contentW - 8, yPos - startY + 2, 2, GxEPD_BLACK);
            yPos += 8; 
        }
    }
    
    drawSidebarBtn(cx, 30, "A", "EXT");
    drawSidebarBtn(cx, 58, "B", "WRT");
    drawSidebarBtn(cx, 86, "C", "MOD");
    drawSidebarBtn(cx, 110, "MOD:", secureMode ? "SEC" : "PUB");
}

void renderKeyboard() {
    drawHeader(tr_compose[currentLang], false);
    int scrW = display.width();
    int scrH = display.height();
    int rightPanelW = 30;
    int contentW = scrW - rightPanelW;
    int cx = contentW + rightPanelW / 2;

    display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);
    display.drawRoundRect(6, 26, contentW - 12, 24, 4, GxEPD_BLACK);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    
    if (msgDraft.length() > 15) u8g2Fonts.setFont(u8g2_font_5x7_tf);
    else u8g2Fonts.setFont(u8g2_font_helvB10_tf);
    
    u8g2Fonts.setCursor(10, 42); 
    u8g2Fonts.print((msgDraft + "_").c_str());

    u8g2Fonts.setFont(u8g2_font_helvB12_tf); 
    int boxSize = 22, spacing = 28, centerY = 62;
    for(int i = -2; i <= 2; i++) {
        int idx = (kbCursor + i + (int)strlen(kbChars)) % (int)strlen(kbChars);
        int boxX = (contentW/2) + (i * spacing) - (boxSize/2);
        char str[2] = {kbChars[idx], '\0'};
        int charW = u8g2Fonts.getUTF8Width(str);
        int charX = boxX + (boxSize - charW) / 2;
        int charY = centerY + 16; 
        if(i == 0) { 
            display.fillRoundRect(boxX, centerY, boxSize, boxSize, 4, GxEPD_BLACK); 
            u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
            u8g2Fonts.setBackgroundColor(GxEPD_BLACK); 
        } else { 
            u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
            display.drawRoundRect(boxX, centerY, boxSize, boxSize, 4, GxEPD_BLACK);
        }
        u8g2Fonts.setCursor(charX, charY); 
        u8g2Fonts.print(str);
    }
    
    u8g2Fonts.setFont(u8g2_font_5x7_tf); 
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    int mw2 = u8g2Fonts.getUTF8Width(tr_use_phone[currentLang]); 
    u8g2Fonts.setCursor((contentW - mw2)/2, 100); 
    u8g2Fonts.print(tr_use_phone[currentLang]);
    
    drawSidebarBtn(cx, 30, "A/C", "MOV");
    drawSidebarBtn(cx, 50, "B", "ADD");
    drawSidebarBtn(cx, 70, "hB", "DEL");
    drawSidebarBtn(cx, 90, "hC", "SND");
    drawSidebarBtn(cx, 110, "hA", "EXT");
}

void renderMenuTeam() {
    drawHeader(tr_team_dash[currentLang], false);
    int scrW = display.width();
    int scrH = display.height();
    int rightPanelW = 30;
    int contentW = scrW - rightPanelW;
    int cx = contentW + rightPanelW / 2;

    display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setFont(u8g2_font_helvB10_tf);
    
    String teamLabel = String(tr_team[currentLang]) + ": " + myTeam;
    u8g2Fonts.setCursor(4, 35); 
    u8g2Fonts.print(teamLabel.c_str());
    display.drawLine(4, 42, contentW - 4, 42, GxEPD_BLACK);
    
    u8g2Fonts.setFont(u8g2_font_6x10_tf); 
    int yPos = 56;
    if (teammateCount == 0) {
        int tw = u8g2Fonts.getUTF8Width(tr_no_team[currentLang]);
        u8g2Fonts.setCursor((contentW - tw)/2, 75); 
        u8g2Fonts.print(tr_no_team[currentLang]);
    } else {
        for (int i = 0; i < teammateCount; i++) {
            float dist = calculateDistance(lastLat, lastLon, teammates[i].lat, teammates[i].lon);
            String distStr = (dist < 1.0) ? String(dist * 1000, 0) + "m" : String(dist, 1) + "km";
            unsigned long passed = millis() - teammates[i].lastSeen;
            int mins = passed / 60000; 
            String timeStr = (mins < 1) ? "NOW" : String(mins) + "m";
            u8g2Fonts.setCursor(4, yPos); 
            u8g2Fonts.print(teammates[i].name.c_str());
            String rightText = distStr + "|" + timeStr;
            int rw = u8g2Fonts.getUTF8Width(rightText.c_str());
            u8g2Fonts.setCursor(contentW - rw - 4, yPos); 
            u8g2Fonts.print(rightText.c_str());
            yPos += 16; 
            if (yPos > 100) break; 
        }
    }
    
    drawSidebarBtn(cx, 40, "A", "EXT");
    drawSidebarBtn(cx, 75, "B", "EDIT");
}

void renderTeamNameEdit() {
    drawHeader(tr_team_name[currentLang], false);
    int scrW = display.width();
    int scrH = display.height();
    int rightPanelW = 30, contentW = scrW - rightPanelW, cx = contentW + rightPanelW / 2;

    display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);
    display.drawRoundRect(6, 26, contentW - 12, 24, 4, GxEPD_BLACK);
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
    u8g2Fonts.setCursor(10, 42); 
    u8g2Fonts.print((teamDraft + "_").c_str());

    u8g2Fonts.setFont(u8g2_font_helvB12_tf); 
    int boxSize = 22, spacing = 28, centerY = 62;
    for(int i = -2; i <= 2; i++) {
        int idx = (kbCursor + i + (int)strlen(kbChars)) % (int)strlen(kbChars);
        int boxX = (contentW/2) + (i * spacing) - (boxSize/2); 
        char str[2] = {kbChars[idx], '\0'};
        int charW = u8g2Fonts.getUTF8Width(str); 
        int charX = boxX + (boxSize - charW) / 2; 
        int charY = centerY + 16; 
        if(i == 0) { 
            display.fillRoundRect(boxX, centerY, boxSize, boxSize, 4, GxEPD_BLACK); 
            u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
            u8g2Fonts.setBackgroundColor(GxEPD_BLACK); 
        } else { 
            u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
            display.drawRoundRect(boxX, centerY, boxSize, boxSize, 4, GxEPD_BLACK); 
        }
        u8g2Fonts.setCursor(charX, charY); 
        u8g2Fonts.print(str);
    }
    
    u8g2Fonts.setFont(u8g2_font_5x7_tf); 
    int mw2 = u8g2Fonts.getUTF8Width(tr_use_phone[currentLang]); 
    u8g2Fonts.setCursor((contentW - mw2)/2, 100); 
    u8g2Fonts.print(tr_use_phone[currentLang]);
    
    drawSidebarBtn(cx, 30, "A/C", "MOV");
    drawSidebarBtn(cx, 50, "B", "ADD");
    drawSidebarBtn(cx, 70, "hB", "DEL");
    drawSidebarBtn(cx, 90, "hC", "SAV");
    drawSidebarBtn(cx, 110, "hA", "EXT");
}

void renderMenuLang(bool initMode = false) {
    if (!initMode) drawHeader(tr_lang[currentLang], false);
    else { 
        display.fillScreen(GxEPD_WHITE); 
        u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
        u8g2Fonts.setFont(u8g2_font_helvB12_tf); 
        u8g2Fonts.setCursor((display.width() - u8g2Fonts.getUTF8Width(tr_welcome[currentLang]))/2, 20); 
        u8g2Fonts.print(tr_welcome[currentLang]); 
    } 
    
    int scrW = display.width(), scrH = display.height();
    int rightPanelW = initMode ? 0 : 30;
    int contentW = scrW - rightPanelW;

    if (!initMode) display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);
    int startY = initMode ? 25 : 35; 
    
    u8g2Fonts.setFont(u8g2_font_helvB12_tf); 
    display.fillRoundRect(4, startY + 10, contentW - 8, 40, 4, GxEPD_BLACK);
    u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
    u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
    
    if (initMode) {
        u8g2Fonts.setCursor(14, startY + 36); u8g2Fonts.print("<"); 
        u8g2Fonts.setCursor(contentW - 24, startY + 36); u8g2Fonts.print(">");
    }
    
    int textW = u8g2Fonts.getUTF8Width(langNames[tempLangSelection]); 
    u8g2Fonts.setCursor((contentW - textW) / 2, startY + 36); 
    u8g2Fonts.print(langNames[tempLangSelection]);
    
    if (initMode) {
        u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
        u8g2Fonts.setFont(u8g2_font_helvB08_tf); 
        String hint = "A: <-  |  C: ->";
        u8g2Fonts.setCursor((contentW - u8g2Fonts.getUTF8Width(hint.c_str()))/2, scrH - 28); 
        u8g2Fonts.print(hint.c_str());
        
        int boxW = u8g2Fonts.getUTF8Width("B: OK") + 16;
        display.fillRoundRect((contentW - boxW)/2, scrH - 25, boxW, 14, 4, GxEPD_BLACK); 
        u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
        u8g2Fonts.setCursor((contentW - u8g2Fonts.getUTF8Width("B: OK"))/2, scrH - 14); 
        u8g2Fonts.print("B: OK");
    } else {
        int cx = contentW + rightPanelW / 2;
        drawSidebarBtn(cx, 40, "A", "UP");
        drawSidebarBtn(cx, 75, "C", "DWN");
        drawSidebarBtn(cx, 110, "B", "SEL");
    }
}

void renderMenuMain() {
    drawHeader("GLYPH", false); 
    int scrW = display.width(), contentW = scrW; 
    bool isPortrait = (scrW < 200);
    String teamLabel = String(tr_team[currentLang]) + ": " + myTeam;
    
    const char* titles[] = { tr_map[currentLang], tr_radio[currentLang], tr_clock[currentLang], teamLabel.c_str(), tr_power[currentLang], tr_lang[currentLang], tr_set_utc[currentLang] };
    int totalItems = 7;
    
    int maxVisible = isPortrait ? 6 : 3;
    
    int startIdx = mainMenuItem - 1; 
    if (startIdx < 0) startIdx = 0; 
    if (startIdx > totalItems - maxVisible) startIdx = totalItems - maxVisible; 

    for (int i = 0; i < maxVisible; i++) {
        int itemIdx = startIdx + i; 
        if (itemIdx >= totalItems) break; 
        
        int cy = 28 + (i * 28);
        if (mainMenuItem == itemIdx) { 
            display.fillRoundRect(4, cy, contentW - 8, 24, 4, GxEPD_BLACK); 
            u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
            u8g2Fonts.setBackgroundColor(GxEPD_BLACK); 
        } else { 
            display.drawRoundRect(4, cy, contentW - 8, 24, 4, GxEPD_BLACK); 
            u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
        }
        u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
        String menuText = String(itemIdx + 1) + ". " + String(titles[itemIdx]); 
        u8g2Fonts.setCursor(10, cy + 17); 
        u8g2Fonts.print(menuText.c_str());
    }
}

void renderMenuPower() {
    drawHeader(tr_power[currentLang], false); 
    int scrW = display.width(), scrH = display.height();
    int rightPanelW = 30, contentW = scrW - rightPanelW, cx = contentW + rightPanelW / 2;

    display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);
    const char* labels[] = {"NORMAL", "ECO", "STEALTH"}; 
    const char* desc[] = {"BLE+GPS", "GPS 10s", "NO RADIO"}; 
    int itemH = 26, startY = 30, spacing = 32;

    for (int i = 0; i < 3; i++) {
        int cy = startY + (i * spacing);
        if (menuSelection == i) { 
            display.fillRoundRect(4, cy, contentW - 8, itemH, 4, GxEPD_BLACK); 
            u8g2Fonts.setForegroundColor(GxEPD_WHITE); 
            u8g2Fonts.setBackgroundColor(GxEPD_BLACK); 
        } else { 
            display.drawRoundRect(4, cy, contentW - 8, itemH, 4, GxEPD_BLACK); 
            u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
        }
        u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
        u8g2Fonts.setCursor(10, cy + 18); 
        u8g2Fonts.print(labels[i]); 
        u8g2Fonts.setFont(u8g2_font_5x7_tf); 
        int dw = u8g2Fonts.getUTF8Width(desc[i]); 
        u8g2Fonts.setCursor(contentW - 6 - dw, cy + 17); 
        u8g2Fonts.print(desc[i]); 
    }
    drawSidebarBtn(cx, 40, "A", "UP");
    drawSidebarBtn(cx, 75, "C", "DWN");
    drawSidebarBtn(cx, 110, "B", "SEL");
}

void renderInitFreq() {
    display.fillScreen(GxEPD_WHITE); 
    int scrW = display.width(), scrH = display.height();
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    int tw = u8g2Fonts.getUTF8Width(tr_sel_region[currentLang]); 
    u8g2Fonts.setCursor((scrW - tw)/2, 30); u8g2Fonts.print(tr_sel_region[currentLang]);
    
    int boxW = 100, boxH = 50;
    if(currentFreq < 900) display.fillRoundRect(20, 45, boxW, boxH, 8, GxEPD_BLACK);
    else display.drawRoundRect(20, 45, boxW, boxH, 8, GxEPD_BLACK);
    if(currentFreq > 900) display.fillRoundRect(130, 45, boxW, boxH, 8, GxEPD_BLACK); 
    else display.drawRoundRect(130, 45, boxW, boxH, 8, GxEPD_BLACK);
    
    u8g2Fonts.setForegroundColor(currentFreq < 900 ? GxEPD_WHITE : GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(currentFreq < 900 ? GxEPD_BLACK : GxEPD_WHITE);
    u8g2Fonts.setCursor(20 + (boxW - u8g2Fonts.getUTF8Width("868MHz"))/2, 77); u8g2Fonts.print("868MHz");
    
    u8g2Fonts.setForegroundColor(currentFreq > 900 ? GxEPD_WHITE : GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(currentFreq > 900 ? GxEPD_BLACK : GxEPD_WHITE);
    u8g2Fonts.setCursor(130 + (boxW - u8g2Fonts.getUTF8Width("915MHz"))/2, 77); u8g2Fonts.print("915MHz");
    
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
    u8g2Fonts.setFont(u8g2_font_helvB08_tf); 
    String hint = "A: <-  |  C: ->  |  B: OK";
    u8g2Fonts.setCursor((scrW - u8g2Fonts.getUTF8Width(hint.c_str()))/2, scrH - 18); 
    u8g2Fonts.print(hint.c_str());
}

void renderInitName() {
    display.fillScreen(GxEPD_WHITE); 
    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    int tw = u8g2Fonts.getUTF8Width(tr_set_username[currentLang]); 
    u8g2Fonts.setCursor((display.width() - tw)/2, 30); u8g2Fonts.print(tr_set_username[currentLang]);
    display.fillRoundRect(20, 45, display.width() - 40, 55, 6, GxEPD_BLACK); 
    u8g2Fonts.setForegroundColor(GxEPD_WHITE); u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
    u8g2Fonts.setFont(u8g2_font_helvB10_tf); 
    u8g2Fonts.setCursor((display.width() - u8g2Fonts.getUTF8Width(tr_use_phone_only[currentLang]))/2, 68); u8g2Fonts.print(tr_use_phone_only[currentLang]);
    u8g2Fonts.setCursor((display.width() - u8g2Fonts.getUTF8Width(tr_or_serial[currentLang]))/2, 88); u8g2Fonts.print(tr_or_serial[currentLang]);
}

void renderMenuTime(bool initMode = false) {
    if (!initMode) drawHeader(tr_set_utc[currentLang], false);
    else display.fillScreen(GxEPD_WHITE); 

    int scrW = display.width(), scrH = display.height();
    int rightPanelW = initMode ? 0 : 30;
    int contentW = scrW - rightPanelW;
    int cx = contentW + rightPanelW / 2;

    if (!initMode) display.drawLine(contentW, 20, contentW, scrH, GxEPD_BLACK);

    u8g2Fonts.setForegroundColor(GxEPD_BLACK); 
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
    u8g2Fonts.setFont(u8g2_font_helvB12_tf);
    
    if (initMode) {
        int tw = u8g2Fonts.getUTF8Width(tr_set_utc[currentLang]); 
        u8g2Fonts.setCursor((scrW - tw)/2, 35); u8g2Fonts.print(tr_set_utc[currentLang]);
    }

    int boxY = initMode ? 50 : 45;
    display.fillRoundRect((contentW - 80)/2, boxY, 80, 40, 8, GxEPD_BLACK); 
    
    u8g2Fonts.setForegroundColor(GxEPD_WHITE); u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
    String offsetStr = "UTC " + String(tempTimeSelection > 0 ? "+" : "") + String(tempTimeSelection);
    u8g2Fonts.setCursor((contentW - u8g2Fonts.getUTF8Width(offsetStr.c_str()))/2, boxY + 26); 
    u8g2Fonts.print(offsetStr.c_str());
    
    if (initMode) {
        u8g2Fonts.setForegroundColor(GxEPD_BLACK); u8g2Fonts.setBackgroundColor(GxEPD_WHITE); 
        u8g2Fonts.setFont(u8g2_font_helvB08_tf);
        String hint = "A: <-  |  C: ->  |  B: OK";
        u8g2Fonts.setCursor((scrW - u8g2Fonts.getUTF8Width(hint.c_str()))/2, scrH - 18); 
        u8g2Fonts.print(hint.c_str());
    } else {
        drawSidebarBtn(cx, 40, "A", "-1h");
        drawSidebarBtn(cx, 75, "C", "+1h");
        drawSidebarBtn(cx, 110, "B", "OK");
    }
}

void updateUI() {
    if (currentState == STANDBY_MODE) return; 

    unsigned long currentMillis = millis();
    bool periodicRefresh = (currentMillis - lastFullRefreshTime > 900000); 

    switch (currentState) {
        case PAGE_MAP: renderMap(); break;
        case PAGE_LORA: renderLoRa(); break;
        case PAGE_CLOCK: renderClock(); break;
        case PAGE_MENU_MAIN: renderMenuMain(); break;
        case PAGE_INIT_LANG: renderMenuLang(true); break;
        case PAGE_INIT_FREQ: renderInitFreq(); break;
        case PAGE_INIT_NAME: renderInitName(); break;
        case PAGE_INIT_TIME: renderMenuTime(true); break; 
        case PAGE_INIT_COMPASS: renderMenuMain(); break; 
        case PAGE_KML_LIST: renderKmlList(); break;
        case PAGE_KEYBOARD: renderKeyboard(); break;
        case PAGE_MENU_TEAM: renderMenuTeam(); break;
        case PAGE_TEAM_NAME_EDIT: renderTeamNameEdit(); break;
        case PAGE_MENU_POWER: renderMenuPower(); break;
        case PAGE_MENU_LANG: renderMenuLang(false); break;
        case PAGE_MENU_TIME: renderMenuTime(false); break;
        default: break; 
    }
    
     if (fullRefreshNeeded || periodicRefresh) { 
        display.update(); 
        fullRefreshNeeded = false; 
        lastFullRefreshTime = currentMillis;
    } else { 
        display.updateWindow(0, 0, display.width(), display.height(), true); 
    }
}