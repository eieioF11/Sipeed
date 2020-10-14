#include <Arduino.h>
#include <SD.h>
#include <Sipeed_OV2640.h>
#include <Sipeed_ST7789.h>
//#include "MBNet_1000.h"
#include "Maix_KPU.h"
#include "Maix_Speech_Recognition.h"
#include "voice_model.h"

SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(320, 240, spi_);
//Sipeed_OV2640 camera(224, 224, PIXFORMAT_RGB565);
Sipeed_OV2640 camera(FRAMESIZE_QVGA, PIXFORMAT_RGB565);
KPUClass KPU;
//BNet_1000 mbnet(KPU, lcd, camera);

const char* kmodel_name = "m";

SpeechRecognizer rec;

void setup()
{
    Serial.begin(115200);
    if(!lcd.begin(15000000, COLOR_RED))
        Serial.println("LCD Errorcom6");
    lcd.setTextColor(COLOR_WHITE);
    lcd.setTextSize(2);
    lcd.setCursor(100,100);
    //lcd.setRotation(5);
    if(!camera.begin())
    {
        while(1)lcd.println("Camera Error");
    }
    camera.run(true);
    camera.setInvert(true);
    if (!SD.begin()) 
    {
        lcd.setTextColor(COLOR_WHITE);
        lcd.setCursor(100,100);
        lcd.print("No SD Card");
        delay(500);
    }
    if (!SD.begin()) 
    {
        lcd.setTextColor(COLOR_WHITE);
        lcd.setCursor(100,100);
        lcd.print("No SD Card");
        delay(500);
    }
  /*
    if(KPU.begin() != 0)
    {
        lcd.println("KPU Error");
    }*/
}
/*
#define SWAP_16(x) ((x >> 8 & 0xff) | (x << 8))
uint8_t* rotationimage(int16_t width,int16_t height,float angle,uint8_t* img)
{
    uint8_t *rimg=(uint8_t*)malloc(sizeof(img));
    float sc=sinf(angle*DEG_TO_RAD);
    float cc=cosf(angle*DEG_TO_RAD);
    for(int16_t i=0;i<width;i+=2)
    {
        for(int16_t j=0;j<height;j+=1)
        {
            int x=i*cc-j*sc;
            int y=i*sc+j*cc;
            rimg[y+x*width]=img[i*width+j];
            rimg[y+x*width+1]=img[i*width+j+1];
            rimg[y+x*width+2]=img[i*width+j+2];
        }
    }
    return rimg;
}*/

void loop()
{
    uint8_t*img = camera.snapshot();
    //uint8_t*img=rotationimage(camera.width(),camera.height(),90,img_);
    //if(KPU.forward(img) != 0);
    if(img == nullptr || img==0)
        Serial.printf("snap fail\n");
    else
        lcd.drawImage(0, 0,camera.width(),camera.height(), (uint16_t*)img);
    lcd.setTextColor(COLOR_WHITE);
    lcd.setCursor(0,0);
    lcd.printf("Run!!");
    /*
    if(mbnet.detect() != 0)
    {
        Serial.println("detect object fail");
        return;
    }
    mbnet.show();*/
}

