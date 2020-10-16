import audio
import gc
import image
import lcd
import sensor
import sys
import time
import uos
import os
from fpioa_manager import *
from machine import I2C
from Maix import I2S, GPIO
from fpioa_manager import fm
from board import board_info

def play_sound(filename):
    try:
        player = audio.Audio(path = filename)
        player.volume(20)
        wav_info = player.play_process(wav_dev)
        wav_dev.channel_config(wav_dev.CHANNEL_1, I2S.TRANSMITTER,resolution = I2S.RESOLUTION_16_BIT, align_mode = I2S.STANDARD_MODE)
        wav_dev.set_sample_rate(wav_info[1])
        spk_sd.value(1)
        while True:
            ret = player.play()
            if ret == None:
                break
            elif ret==0:
                break
        player.finish()
        spk_sd.value(0)
    except:
        pass

fm.register(board_info.BUTTON_A, fm.fpioa.GPIO1)
but_a=GPIO(GPIO.GPIO1, GPIO.IN, GPIO.PULL_UP)
fm.register(board_info.BUTTON_B, fm.fpioa.GPIO2)
but_b = GPIO(GPIO.GPIO2, GPIO.IN, GPIO.PULL_UP)
is_button_a = 0
is_button_b = 0

lcd.init()
lcd.rotation(2)

try:
    from pmu import axp192
    pmu = axp192()
    pmu.enablePMICSleepMode(True)
except:
    pass

fm.register(board_info.SPK_SD, fm.fpioa.GPIO0)
spk_sd=GPIO(GPIO.GPIO0, GPIO.OUT)
spk_sd.value(1) #Enable the SPK output

fm.register(board_info.SPK_DIN,fm.fpioa.I2S0_OUT_D1)
fm.register(board_info.SPK_BCLK,fm.fpioa.I2S0_SCLK)
fm.register(board_info.SPK_LRCLK,fm.fpioa.I2S0_WS)

wav_dev = I2S(I2S.DEVICE_0)

#button
fm.register(board_info.BUTTON_A, fm.fpioa.GPIO1)
but_a=GPIO(GPIO.GPIO1, GPIO.IN, GPIO.PULL_UP) #PULL_UP is required here!

fm.register(board_info.BUTTON_B, fm.fpioa.GPIO2)
but_b = GPIO(GPIO.GPIO2, GPIO.IN, GPIO.PULL_UP) #PULL_UP is required here!
#LED
fm.register(board_info.LED_W, fm.fpioa.GPIO3)
led_w = GPIO(GPIO.GPIO3, GPIO.OUT)
led_w.value(1) # LED is Active Low

fm.register(board_info.LED_R, fm.fpioa.GPIO4)
led_r = GPIO(GPIO.GPIO4, GPIO.OUT)
led_r.value(1) # LED is Active Low

fm.register(board_info.LED_G, fm.fpioa.GPIO5)
led_g = GPIO(GPIO.GPIO5, GPIO.OUT)
led_g.value(1) # LED is Active Low

fm.register(board_info.LED_B, fm.fpioa.GPIO6)
led_b = GPIO(GPIO.GPIO6, GPIO.OUT)
led_b.value(1) # LED is Active Low

#camera
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.run(1)

path = "/sd/image/"
ext=".jpg"
cnt=0
img_read = image.Image()

#os.mkdir("save")
print(os.listdir())

FLASH=False
flag=False

try:
    while True:
        if is_button_b == 1:
            if cnt == 0:
                lcd.draw_string(0,28,"No image!",lcd.WHITE,lcd.BLACK)
            else:
                lcd.display(img_read)
        else :
            img=sensor.snapshot()
            lcd.display(img)

        if but_a.value() == 0 and but_b.value() == 0:
            if FLASH and flag == False:
                FLASH=False
            else:
                FLASH=True
            flag=True
        elif but_a.value() == 0 and is_button_a == 0:
            print("save image")
            cnt+=1
            fname=path+str(cnt)+ext
            print(fname)
            img.save(fname, quality=95)
            play_sound("/sd/kacha.wav")
            is_button_a=1
        else :
            flag=False

        if but_a.value() == 1 and is_button_a == 1:
            is_button_a=0

        if but_b.value() == 0 and is_button_b == 0:
            if cnt !=0:
                fname=path+str(cnt)+ext
                print(fname)
                img_read = image.Image(fname)
            is_button_b=1

        if but_b.value() == 1 and is_button_b == 1:
            is_button_b=0

        if FLASH:
            led_w.value(1)
            led_r.value(0)
            led_g.value(0)
            led_b.value(1)
        else:
            led_w.value(1)
            led_r.value(1)
            led_g.value(1)
            led_b.value(1)


except KeyboardInterrupt:
    pass
