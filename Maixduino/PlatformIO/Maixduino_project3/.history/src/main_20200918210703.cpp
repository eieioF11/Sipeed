#include <Arduino.h>
#include <SD.h>
#include <Sipeed_OV2640.h>
#include <Sipeed_ST7789.h>
#include <Maix_KPU.h>
#include "Maix_Speech_Recognition.h"
#include "region_layer.h"


SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(320, 240, spi_);
//Sipeed_OV2640 camera(224,224, PIXFORMAT_RGB565);
//Sipeed_OV2640 camera(28,28, PIXFORMAT_RGB565);
//Sipeed_OV2640 camera(FRAMESIZE_QVGA, PIXFORMAT_RGB565);

KPUClass KPU;

//const char* kmodel_name = "detect";//モデル名
const char* kmodel_name = "mnist";//モデル名

SpeechRecognizer rec;


void setup()
{
    Serial.begin(115200);
    if(!lcd.begin(15000000, COLOR_RED))
        Serial.println("LCD Errorcom6");
    lcd.setTextColor(COLOR_WHITE);
    lcd.setTextSize(2);
    lcd.setCursor(100,100);
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

    File myFile = SD.open(kmodel_name);
    if (!myFile)
        return;
    uint32_t fSize = myFile.size();
    lcd.setCursor(100,100);
    lcd.print("Loading ... ");
    uint8_t *model = (uint8_t*)malloc(fSize);//メモリー上にモデルデータのサイズ分の領域確保
    if(!model)
    {
        lcd.setCursor(100,140);
        lcd.print("Memmory not enough ... ");
        delay(500);
        return;
    }
    long ret = myFile.read(model, fSize);//モデルデータ読み取り
    myFile.close();
    if(ret != fSize)
    {
        lcd.setCursor(100,140);
        lcd.print("ret != fSize");
        free(model);
        model = nullptr;
        delay(500);
        return;
    }
    if(KPU.begin(model) != 0)
    {
        free(model);
        model = nullptr;
        lcd.println("KPU Error");
        delay(500);
        return;
    }
    lcd.setCursor(0,0);
    lcd.println("\nInitialize success!");
    delay(500);
}

size_t count;
float* result;
void loop()
{
    //lcd.fillScreen(COLOR_RED);
    uint8_t*img = camera.snapshot();
    if(img == nullptr || img==0)
        Serial.printf("snap fail\n");
    else
        lcd.drawImage(0, 0,camera.width(),camera.height(), (uint16_t*)img);
    lcd.setTextColor(COLOR_WHITE);
    lcd.setCursor(0,240-20);
    uint8_t* img888 = camera.getRGB888();
    if(KPU.forward(img888) != 0)
    {
        lcd.printf("KPU Error!!");
        return;
    }
    while( !KPU.isForwardOk() );
    if(KPU.getResult((uint8_t**)&result, &count) != 0)
    {
        lcd.printf("getResult Error!!");
        return;
    }
    lcd.setTextSize(1);
    lcd.setCursor(0,0);
    for(int i=0; i<count; i++)
        lcd.printf("%.1f/",result[i]);
    lcd.setTextSize(2);
    lcd.setCursor(0,240-20);
    lcd.printf("Run!!");
}
