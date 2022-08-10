/**
 * MIT License
 *
 * Copyright (c) 2022 R. Dunbar Poor
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <NDP.h>
#include <NDP_utils.h>
#include <Arduino.h>
#include "TinyML_init.h"
#include "NDP_init.h"
#include "NDP_loadModel.h"
#include "SAMD21_init.h"
#include "SAMD21_lowpower.h"
#include "AudioZero.h"

#define NDP_MICROPHONE 0
#define NDP_SENSOR 1

#define DO_NOT_WAIT_FOR_SERIAL_PORT 0                       // For battery powered autonomus usage, use this option
#define WAIT_FOR_SERIAL_PORT 1                              // For debugging purposes use this option, program will wait until serial port is turned on, Arduino IDE Tools --> Serial Monitor
#define NDP_MICROPHONE 0                                    // Model and this slection MUST match
#define NDP_SENSOR 1                                        // Model and this slection MUST match
#define NUMBER_OF_CLASSIFIERS 3                             // This includes z_openset

int match = 0;                                              // This variable indicates which class has matched

//added by Justin to access the SD card for reading wav files
extern SdFat SD;
File myFile;
// filenames of wave files to play
const char breathe[] = "breathe2.wav";
const char talk[] = "talk5.wav";

void NDP_INT_service(){                                     // This is user's sandbox to do things when a classifier is detected
  switch (match){                                           // match is slighly modified in sequence. When no classifier is asserted after posterior filter then 0 is returned. Classifier0 is retrurnd as 1 and so on
    case 0:
      Serial.println("z_openset");
      delay(1000);
      break;                                                
    case 1:                                                 // Breathe classifier in breathe/speak model
      digitalWrite(LED_GREEN, HIGH);                        // Turning on green light when "Breathe" is detected
      Serial.println("Breathe detected");
      myFile = SD.open("breathe2.wav", FILE_READ);
      AudioZero.begin(22050);
      AudioZero.play(myFile);
      delay(100);
      AudioZero.play(myFile);
      AudioZero.end();
      myFile.close();    
      delay(2000);
      match=0;
      digitalWrite(LED_GREEN, LOW);                         // Green LED is turned off after 2 seconds
      break;
    case 2:                                                 // Speak classifier in breathe/speak model
      digitalWrite(LED_RED, HIGH);                          // Turning on red light when "Speak" is detected
      Serial.println("Speak detected");
      myFile = SD.open(talk, FILE_READ);
      AudioZero.begin(22050);
      AudioZero.play(myFile);
      AudioZero.end();
      myFile.close();   
      delay(2000);                                         
      digitalWrite(LED_RED, LOW);                           // Red LED is turned off after 2 s
      match=0;
      break;
    default:
      break;
  }
}

/**
 * @brief One-time setup, called upon reboot.
 */
void setup(void) {
  SAMD21_init(DO_NOT_WAIT_FOR_SERIAL_PORT);                        // Setting up SAMD21 (0) will not wait for serial port, (1) will wait and RGB LED will be Red
  //SAMD21_init(WAIT_FOR_SERIAL_PORT);                        // Setting up SAMD21 (0) will not wait for serial port, (1) will wait and RGB LED will be Red
  NDP_init("ei_model.bin",NDP_MICROPHONE);                  // Setting up NDP, Stuck Blue LED means model is not read 
  Serial.println(""); 
  delay(2000);                                              // Flushing out buffer
  NDP.poll();                                               // Flushing out buffer
}

void loop() {
    delay(22);
    match = NDP.poll();                                    // comment out this line if using low power version
    //Serial.print("Match :");
    //Serial.println(match);
    NDP_INT_service();                                     // Service the interrupts
}
