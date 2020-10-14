#include <Arduino.h>
#include <SD.h>
#include <Sipeed_OV2640.h>
#include <Sipeed_ST7789.h>
#include "MBNet_1000.h"
#include "Maix_KPU.h"
#include "Maix_Speech_Recognition.h"
#include "voice_model.h"

SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(320, 240, spi_);
//Sipeed_OV2640 camera(224, 224, PIXFORMAT_RGB565);
Sipeed_OV2640 camera(FRAMESIZE_QVGA, PIXFORMAT_RGB565);
KPUClass KPU;
MBNet_1000 mbnet(KPU, lcd, camera);

const char* kmodel_name = "m";

SpeechRecognizer rec;

void setup()
{
    Serial.begin(115200);
    if(!lcd.begin(15000000, COLOR_RED))
        Serial.println("LCD Errorcom6");
    lcd.setTextColor(COLOR_WHITE);
    lcd.setTextSize(2);
    //lcd.setCursor(100,100);
    //lcd.setRotation(5);
    if(!camera.begin())
    {
        while(1)lcd.println("Camera Error");
    }
    camera.run(true);
    camera.setInvert(false);
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
}

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

/*
void setup()
{
    Serial.begin(115200);
    if(!lcd.begin(15000000, COLOR_RED))
        Serial.println("LCD Errorcom6");
    lcd.setTextColor(COLOR_WHITE);
    lcd.setTextSize(2);
    lcd.setCursor(100,100);
    lcd.setRotation(5);
    /*if (!SD.begin()) 
    {
        lcd.setTextColor(COLOR_WHITE);
        lcd.setCursor(100,100);
        lcd.print("No SD Card");
        delay(500);
    }
    /*rec.begin();
    lcd.println("init model...");
    rec.addVoiceModel(0, 0, red_0, fram_num_red_0); 
    rec.addVoiceModel(0, 1, red_1, fram_num_red_1); 
    rec.addVoiceModel(0, 2, red_2, fram_num_red_2); 
    rec.addVoiceModel(0, 3, red_3, fram_num_red_3); 
    rec.addVoiceModel(1, 0, green_0, fram_num_green_0);     
    rec.addVoiceModel(1, 1, green_1, fram_num_green_1);     
    rec.addVoiceModel(1, 2, green_2, fram_num_green_2);     
    rec.addVoiceModel(1, 3, green_3, fram_num_green_3);     
    rec.addVoiceModel(2, 0, blue_0, fram_num_blue_0);   
    rec.addVoiceModel(2, 1, blue_1, fram_num_blue_1);   
    rec.addVoiceModel(2, 2, blue_2, fram_num_blue_2);   
    rec.addVoiceModel(2, 3, blue_3, fram_num_blue_3);   
    rec.addVoiceModel(3, 0, turnoff_0, fram_num_turnoff_0);  
    rec.addVoiceModel(3, 1, turnoff_1, fram_num_turnoff_1);  
    rec.addVoiceModel(3, 2, turnoff_2, fram_num_turnoff_2);  
    rec.addVoiceModel(3, 3, turnoff_3, fram_num_turnoff_3);  
    lcd.fillScreen(COLOR_RED);
    lcd.println("init model ok!");*/
    /*
    Serial.println("init mobile net, load kmodel from SD card, it may takes a long time");
    if( mbnet.begin(kmodel_name) != 0)
    {
        Serial.println("mobile net init fail");
        while(1);
    }
    lcd.setTextColor(COLOR_DARKGREY);
}

void loop()
{
    int res;
    
    res = rec.recognize();
    Serial.printf("res : %d ", res);
    if (res > 0)
    {
        switch (res)
        {
        case 1:
            lcd.fillScreen(COLOR_RED);
            Serial.println(" red ");
            break;
        case 2:
            lcd.fillScreen(COLOR_GREEN);
            Serial.println(" green ");

            break;
        case 3:
            lcd.fillScreen(COLOR_BLUE);
            Serial.println(" blue ");                
            break;
        case 4:
            lcd.fillScreen(COLOR_BLACK);
            Serial.println(" turnoff ");
        default:
            break;
        }
    }
    else
    {
        lcd.fillScreen(COLOR_WHITE);
        Serial.println("recognize failed.");
    }
    delay(1000);
}
*/