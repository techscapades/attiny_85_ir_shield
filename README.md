# attiny_85_ir_shield
attiny 85 ir shield PCB + code + how to use code

![attiny85_side1](https://user-images.githubusercontent.com/76870663/122782881-a99d4e80-d2e3-11eb-9753-2a9bdd976d89.jpg)
![attiny85_side2](https://user-images.githubusercontent.com/76870663/122782990-c174d280-d2e3-11eb-9bce-d40834fc5eb4.jpg)


**BILL OF MATERIALS**
1. AT TINY 85 (https://www.aliexpress.com/item/606895785.html) //I recommend getting from alibaba instead
2. VS1838B (https://www.aliexpress.com/item/32452383491.html) //**pinout**: https://buyhere22.com/image/cache/data/products/236/236159/ir-vs1838b-pinout-800x800.png
3. Right angle push button switch (https://www.aliexpress.com/item/4000864377668.html) //I got the 6x6x5 version
4. Toggle switch (https://www.aliexpress.com/item/32989785276.html?algo_pvid=2afdb9c4-8a46-46d0-b99b-37b43d753335&algo_exp_id=2afdb9c4-8a46-46d0-b99b-37b43d753335-2)
   //I bent the legs at 90 degrees before soldering
5. IR Shield (gerber files in repo, use JLCPCB to manufacture cause theyre really fast)
6. Male and female 2.54mm header pins

**Notes:**
1. I chose to solder the right angle push button switch on the bottom side of the PCB 
   because its easier to press, but you can do it on the top as well
2. The toggle switch allows you to toggle between pogramming mode and normal operation 
   mode so you dont have to keep plugging and unplugging your attiny 85,
   [] Toggle switch back --> programming mode (can also be used to disable the at tiny 85)
   [] Toggle switch front --> operation mode
3. ### The PCB design and Gerber files are copyrighted 

**Arduino IDE pre-requisites**
1.This project uses this library: https://github.com/techscapades/Adafruit-Trinket-USB 
make sure you download it and place it into your 'libraries' folder in arduino.
Ensure

**How to use the code**
1. By default "#define DEBUG 1" is stated, this is used to find out IR codes from IR remotes
   ### ENSURE ITS SET TO 0 AFTER YOU'RE DONE OR IR CODES WILL BE PRINTED OUT
2. Open notepad
3. Point you IR remote at the reciever and record what IR code is shown after you press the button
4. #define your_button your_button_IR_code
5. In void Action() function, add "case your_button:" and then choose what keycode you 
   want it to execute from: https://github.com/adafruit/Adafruit-Trinket-USB/blob/master/TrinketHidCombo/TrinketHidCombo.h
   note that you can also choose to execute a bunch of keycodes as well, explore :D
6. In runPushButtonCmd() function, run a command or list of commands when the side push button is pressed, I use it
   to automate the series of keypresses I execute the most 
   ### YOU MAY NEED TO INCLUE DELAYS INBETWEEN KEYPRESSES IF YOUR SYSTEM DOESN'T DETECT THEM, USE "ms_delay(250)" for example

**If you want to change the name of the device, go to:**
1. Arduino>libraries>TrinketHidCombo
2. open "usbconfig.h" file in notepad
3. comment out "#define USB_CFG_VENDOR_NAME" and "define USB_CFG_VENDOR_NAME_LEN"
4. Change "#define USB_CFG_DEVICE_NAME" to what ever you want, putting wach character in single quotes and spacing them with a comma
   example:  #define USB_CFG_DEVICE_NAME     'K', 'e', 'y', 'b', 'o', 'a', 'r', 'd', '-', 'M', 'o', 'u', 's', 'e', ' ', ' C', 'o', 'm', 'b','o'
5. Change "#define USB_CFG_DEVICE_NAME_LEN" to match the number of characters in #define USB_CFG_DEVICE_NAME
   example: #define USB_CFG_DEVICE_NAME_LEN 20 // 20 corresponds to the number to of chars in 4. example, note that spaces are counted as 1 char
6. Save it and exit, then compile the code to reflect changes
7. Go to devices manager to check if device name changed. 
