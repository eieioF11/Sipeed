from Maix import GPIO, I2S, AUDIO, FFT
import image, lcd, math
from board import board_info
from fpioa_manager import fm
import KPU as kpu

sample_rate = 11025
sample_points = 1024
fft_points = 512
hist_x_num = 128

lcd.init()
# close WiFi
fm.register(8,  fm.fpioa.GPIO0)
wifi_en=GPIO(GPIO.GPIO0,GPIO.OUT)
wifi_en.value(0)
fm.register(20,fm.fpioa.I2S0_IN_D0)
fm.register(19,fm.fpioa.I2S0_WS)    # 30 on dock/bit Board
fm.register(18,fm.fpioa.I2S0_SCLK)  # 32 on dock/bit Board

rx = I2S(I2S.DEVICE_0)
rx.channel_config(rx.CHANNEL_0, rx.RECEIVER, align_mode = I2S.STANDARD_MODE)
rx.set_sample_rate(sample_rate)
img = image.Image(size=(256,240))
img=img.to_grayscale()

while True:
    audio = rx.record(sample_points)
    fft_res = FFT.run(audio.to_bytes(),fft_points)
    fft_amp = FFT.amplitude(fft_res)
    img_tmp = img.cut(0,0,256,239)
    a = img.draw_image(img_tmp, 0,1)
    for i in range(hist_x_num):
        img[i*2] = fft_amp[i]
        img[i*2+1] = fft_amp[i]
    del(a)
    del(img_tmp)
    imgc = img.to_rainbow(1)
    lcd.display(imgc)
    del(imgc)
    fft_amp.clear()
