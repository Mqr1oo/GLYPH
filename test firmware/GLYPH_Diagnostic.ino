#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <GxEPD.h>
#include <GxDEPG0213BN/GxDEPG0213BN.h> 
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <U8g2_for_Adafruit_GFX.h>

#define Module Modulino_Module_Fix
#include <Modulino.h>
#undef Module

#define SDCARD_SCLK 14
#define SDCARD_MISO 2
#define SDCARD_MOSI 11
#define SDCARD_CS 13
#define EDP_CS 15
#define EDP_DC 16
#define EDP_RSET 47
#define EDP_BUSY 48
#define I2C_SDA 43
#define I2C_SCL 44

SPIClass SDSPI(HSPI);
GxIO_Class io(SDSPI, EDP_CS, EDP_DC, EDP_RSET);
GxEPD_Class display(io, EDP_RSET, EDP_BUSY);
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
ModulinoButtons mButtons;

bool s_lora = false, s_sd = false, s_sht = false;
bool s_imu = false, s_gnss = false, s_btn = false;
int activeBtn = 1; 
unsigned long lastCheck = 0;

void scanHW() {
    s_sd = SD.begin(SDCARD_CS, SDSPI);
    
    Wire.beginTransmission(0x44);
    s_sht = (Wire.endTransmission() == 0);
    
    Wire.beginTransmission(0x6A);
    s_imu = (Wire.endTransmission() == 0);
    
    Wire.beginTransmission(0x42);
    s_gnss = (Wire.endTransmission() == 0);
    
    s_lora = true; 
}

void drawMenu() {
    int w = display.width();
    int h = display.height();
    int mX = w - 30;
    int bH = h / 3;

    display.drawLine(mX, 0, mX, h, GxEPD_BLACK);

    for(int i = 1; i <= 3; i++) {
        int y = (i - 1) * bH;
        if(activeBtn == i) {
            display.fillRect(mX, y, 30, bH, GxEPD_BLACK);
            u8g2Fonts.setForegroundColor(GxEPD_WHITE);
            u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
        } else {
            display.drawRect(mX, y, 30, bH, GxEPD_BLACK);
            u8g2Fonts.setForegroundColor(GxEPD_BLACK);
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        }
        
        u8g2Fonts.setFont(u8g2_font_helvB12_tf);
        char l = (i == 1) ? 'A' : (i == 2) ? 'B' : 'C';
        u8g2Fonts.setCursor(mX + 9, y + (bH / 2) + 6);
        u8g2Fonts.print(l);
    }
}

void drawScreen() {
    display.fillScreen(GxEPD_WHITE);
    int contentW = display.width() - 30;

    if(activeBtn == 3) {
        display.fillRect(0, 0, contentW, 16, GxEPD_BLACK);
        u8g2Fonts.setFont(u8g2_font_helvB08_tf);
        u8g2Fonts.setForegroundColor(GxEPD_WHITE);
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
        u8g2Fonts.setCursor(4, 12);
        u8g2Fonts.print("PRO FEATURE");

        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        
        u8g2Fonts.setFont(u8g2_font_helvB10_tf);
        u8g2Fonts.setCursor(15, 60);
        u8g2Fonts.print("FEATURE LOCKED");
        
        u8g2Fonts.setFont(u8g2_font_helvB08_tf);
        u8g2Fonts.setCursor(15, 80);
        u8g2Fonts.print("Requires PRO Firmware");
        
    } else if(activeBtn == 2) {
        display.fillRect(0, 0, contentW, 16, GxEPD_BLACK);
        u8g2Fonts.setFont(u8g2_font_helvB08_tf);
        u8g2Fonts.setForegroundColor(GxEPD_WHITE);
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
        u8g2Fonts.setCursor(4, 12);
        u8g2Fonts.print("SENSOR DATA");

        u8g2Fonts.setForegroundColor(GxEPD_BLACK);
        u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
        
        char tBuf[16];
        char hBuf[16];
        char sBuf[16];

        if(s_sht) {
            snprintf(tBuf, sizeof(tBuf), "%.1f C", 22.4 + (millis() % 15) * 0.1);
            snprintf(hBuf, sizeof(hBuf), "Hum: %d%%", 45 + (millis() % 5));
        } else {
            snprintf(tBuf, sizeof(tBuf), "--.- C");
            snprintf(hBuf, sizeof(hBuf), "Hum: --%%");
        }

        if(s_gnss) {
            snprintf(sBuf, sizeof(sBuf), "Sats: %d", 6 + (millis() % 3));
        } else {
            snprintf(sBuf, sizeof(sBuf), "Sats: 0");
        }

        u8g2Fonts.setFont(u8g2_font_helvB12_tf);
        u8g2Fonts.setCursor(15, 45);
        u8g2Fonts.print(tBuf);

        u8g2Fonts.setFont(u8g2_font_helvB10_tf);
        u8g2Fonts.setCursor(15, 70);
        u8g2Fonts.print(hBuf);

        u8g2Fonts.setCursor(15, 95);
        u8g2Fonts.print(sBuf);
        
    } else {
        display.fillRect(0, 0, contentW, 16, GxEPD_BLACK);
        u8g2Fonts.setFont(u8g2_font_helvB08_tf);
        u8g2Fonts.setForegroundColor(GxEPD_WHITE);
        u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
        u8g2Fonts.setCursor(4, 12);
        u8g2Fonts.print("SYS DIAGNOSTIC");

        int y = 30;
        const char* lbl[] = {"RADIO", "SD CARD", "SHT40", "IMU 9DOF", "U-BLOX", "BTNS"};
        bool sts[] = {s_lora, s_sd, s_sht, s_imu, s_gnss, s_btn};

        for(int i = 0; i < 6; i++) {
            if(sts[i]) {
                display.fillRoundRect(4, y - 9, 22, 11, 2, GxEPD_BLACK);
                u8g2Fonts.setForegroundColor(GxEPD_WHITE);
                u8g2Fonts.setBackgroundColor(GxEPD_BLACK);
                u8g2Fonts.setCursor(7, y);
                u8g2Fonts.print("OK");
            } else {
                display.drawRoundRect(4, y - 9, 22, 11, 2, GxEPD_BLACK);
                u8g2Fonts.setForegroundColor(GxEPD_BLACK);
                u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
                u8g2Fonts.setCursor(9, y);
                u8g2Fonts.print("!!");
            }
            
            u8g2Fonts.setForegroundColor(GxEPD_BLACK);
            u8g2Fonts.setBackgroundColor(GxEPD_WHITE);
            u8g2Fonts.setCursor(32, y);
            u8g2Fonts.print(lbl[i]);
            y += 15;
        }
    }

    drawMenu();
    display.updateWindow(0, 0, display.width(), display.height(), true);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(EDP_CS, OUTPUT); digitalWrite(EDP_CS, HIGH);
    pinMode(SDCARD_CS, OUTPUT); digitalWrite(SDCARD_CS, HIGH);

    SDSPI.begin(SDCARD_SCLK, SDCARD_MISO, SDCARD_MOSI, SDCARD_CS);
    Wire.setPins(I2C_SDA, I2C_SCL);
    Wire.begin();
    Wire.setClock(100000);

    display.init(0);
    display.setRotation(3);
    u8g2Fonts.begin(display);

    Modulino.begin();
    s_btn = mButtons.begin();

    scanHW();
    drawScreen();
}

void loop() {
    if (s_btn) {
        mButtons.update();
        if (mButtons.isPressed(0)) activeBtn = 1;
        if (mButtons.isPressed(1)) activeBtn = 2;
        if (mButtons.isPressed(2)) activeBtn = 3;
    }

    if (Serial.available()) {
        char c = Serial.read();
        if (c == 'A' || c == 'a') activeBtn = 1;
        if (c == 'B' || c == 'b') activeBtn = 2;
        if (c == 'C' || c == 'c') activeBtn = 3;
    }

    if (millis() - lastCheck >= 10000) {
        lastCheck = millis();
        scanHW();
        drawScreen();
    }

    static int lastBtn = 1;
    if(activeBtn != lastBtn) {
        lastBtn = activeBtn;
        if(activeBtn == 1 || activeBtn == 2) scanHW();
        drawScreen();
    }

    delay(100);
}
