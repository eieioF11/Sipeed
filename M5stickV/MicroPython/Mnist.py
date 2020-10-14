import sensor,image,lcd
import KPU as kpu

lcd.init()
lcd.rotation(2)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_windowing((224, 224))	#set to 224x224 input
task = kpu.load(0x200000) #MNIST
sensor.run(1)
while True:
    img = sensor.snapshot()
    lcd.display(img)
    img1 = img.to_grayscale()
    img2=img1.resize(28,28)         #resize to mnist input 28x28
    a=img2.invert()					#invert picture as mnist need
    a=img2.strech_char(1)           #preprocessing pictures, eliminate dark corner
    a=img2.pix_to_ai();				#generate data for ai
    fmap=kpu.forward(task,img2)     #run neural network model
    plist=fmap[:]                   #get result (10 digit's probability)
    pmax=max(plist)                 #get max probability
    if pmax<1.000:
        #lcd.draw_string(0,0,"%.3f"%(pmax),lcd.WHITE,lcd.BLACK)  #show result
        max_index=plist.index(pmax)     #get the digit
        lcd.draw_string(0,28,"%d: %.3f"%(max_index,pmax),lcd.WHITE,lcd.BLACK)  #show result
