from modules import ws2812
from fpioa_manager import *
fm.register(board_info.CONNEXT_A)
class_ws2812 = ws2812(board_info.CONNEXT_A,130)
r=0
dir = True
while True:
    if dir:
        r += 1
    else:
        r -= 1
    if r>=255:
        r = 255
        dir = False
    elif r<0:
        r = 0
        dir = True
    for i in range(130):
        a = class_ws2812.set_led(i,(0,0,r))
    a=class_ws2812.display()
