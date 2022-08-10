# darth-vader
## Give voice commands and have Darth Vader speak.  Syntiant TinyML board and Edge Impulse used.

1. Open darth-vader.ino in the Arduino IDE
2. Extract the Arduino-TinyML.zip zip file to the same directory as the .ino sketch
3. In the Arduino IDE, go to File > Preferences and set the sketch directory to the Arduino-TinyML folder location (I recently learned this is where the library files are searched for)
4. Copy the contents of the SD card folder to the root directory of your SD card.  Put the SD card in the Syntiant TinyML board
5. Upload the sketch to the board
6. Set up the hardware: Syntiant pin 5 to LM386 module.  5V power and ground to LM386 module.  USB power to Syntiant board (or battery).
7. Enjoy!
