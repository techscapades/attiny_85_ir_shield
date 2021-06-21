# attiny_85_ir_shield
attiny 85 ir shield PCB + code 

This project uses this library: https://github.com/techscapades/Adafruit-Trinket-USB 
make sure you download it and place it into your 'libraries' folder in arduino.



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
