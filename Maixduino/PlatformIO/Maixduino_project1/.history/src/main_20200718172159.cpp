#include <Arduino.h>
#include <SD.h>
#include <Sipeed_OV2640.h>
#include <Sipeed_ST7789.h>
#include "MBNet_1000.h"
#include "Maix_KPU.h"
#include "Maix_Speech_Recognition.h"


SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(320, 240, spi_);
//Sipeed_OV2640 camera(224, 224, PIXFORMAT_RGB565);
Sipeed_OV2640 camera(FRAMESIZE_QVGA, PIXFORMAT_RGB565);
KPUClass KPU;
BNet_1000 mbnet(KPU, lcd, camera);

const char* kmodel_name = "m";


void setup()
{
    Serial.begin(115200);
    if(!lcd.begin(15000000, COLOR_RED))
        Serial.println("LCD Errorcom6");
    lcd.setTextColor(COLOR_WHITE);
    lcd.setTextSize(2);
    lcd.setCursor(100,100);

    camera.setInvert(true);
}
