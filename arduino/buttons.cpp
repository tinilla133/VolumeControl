 /*
 * 
 *  Stereo Volume Control using Texas Instruments PGA 2310, PGA 2311,
 *  Cirrus Logic CS3310 and compatibles.
 *  
 *  (C) 2021 Quality Hi-Fi Works Audio, S. L.
 *  
 *  https://www.qhwaudio.com
 *  
 *  By: Francisco J Vizcaya López  <franciscovl@qhwaudio.com>
 *  
 *  
  */

#include <Arduino.h>
#include "buttons.h"
#include "setup.h"

class PushButton;

        RotaryEncoder::RotaryEncoder(unsigned char a, unsigned char b, unsigned char c) {

            turns = 0;
            accel = 0;
            pin1 = a;
            pin2 = b;
            pushButton = new PushButton(c);
          
        }  // End of constructor

        int RotaryEncoder::rotate() {  // Check for Gray code matching and looks for next bit changes.
          
            unsigned char readPin1, readPin2, accelAux;
            static unsigned char lastRead1 = 0, lastRead2 = 0;
            int direction = 0;
            static int turnsLast = turns;
            static unsigned long int timeAux = millis();

            readPin1 = digitalRead(ENCODER1);
            delay(ENCODERDELAY);
            readPin2 = digitalRead(ENCODER2);

            if(readPin1 != lastRead1) {  
                if(readPin1) {  // CCW  Pin 1 from 0 to 1
                    if(!readPin2) {
                        turns--;
                        direction = -1;
                        #ifdef _DEBUG_
                        Serial.println("CCW<===============");
                        #endif
                    } 
                }
                else {  // CW Pin 1 from 0 to 1
                    if(!readPin2) { // CW
                        turns++;
                        direction = 1;
                        #ifdef _DEBUG_
                        Serial.println("=============>CW");
                        #endif
                    }
                    else { // CCW
                        turns--;
                        direction = -1;
                        #ifdef _DEBUG_
                        Serial.println("<CCW===============");
                        #endif
                    }
                }
            }

             lastRead1 = readPin1;
             lastRead2 = readPin2;

             if((millis() - timeAux) > TIMEFORACCEL) {
                accelAux = abs(turnsLast - turns);
                switch (accelAux) {
                    case 1:
                        accel = 0;
                        break;
                    case 2:
                        accel = 2;
                        break;
                    case 3:
                    case 4:
                        accel = 4;
                        break;
                    case 5:
                    case 7:
                        accel = 6;
                        break;
                    case 8:
                    case 11:
                        accel = 9;
                        break;
                    case 12:
                    case 20:
                        accel = 12;
                        break;
                } 
                turnsLast = turns;
                timeAux = millis();
#ifdef _DEBUG_
                Serial.print("Velocitatix =============> ");
                Serial.println(accel);
#endif
            }

             return(direction);
             
        }

        PushButton::PushButton(unsigned char buttonPin) {

            pin = buttonPin;
            shortPushed = false;
            longPushed = false;
            
        } // End of constructor

        void PushButton::pushed(unsigned int aWhile) {

            unsigned long int currentTime = millis();
            unsigned int timePushed = 0;
            
            if(aWhile < MAXPUSHEDTIME)
                while(digitalRead(pin) == LOW) {
#ifdef _DEBUG_
                    Serial.println(currentTime);
#endif        
                    timePushed = millis() - currentTime;                                                                                          
                    if((millis() - currentTime) > MAXPUSHEDTIME) {
                        timePushed = 0;
                        break;
                    }
                    else
                        if((millis() - currentTime) >= aWhile) {
                            timePushed = aWhile;
                            break;
                        }
                }

                if(!timePushed) {
                    shortPushed = false;
                    longPushed = false;
                }
                else
                    if(timePushed == aWhile) {
                        if(longPushed)
                            longPushed = false;
                        else {
                            shortPushed = false;
                            longPushed = true;
                        }  
                    }
                    else
                        if(timePushed > PUSHDELAY) {
                            if(longPushed)
                                longPushed = false;
                            else {
                                shortPushed = true;
                                longPushed = false;
                            }  
                        }
                
        } // End of void PushButton::pushed()
      
        


        
