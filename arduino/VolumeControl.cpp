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
#include "setup.h"
#include "VolumeControl.h"
#include "buttons.h"
#ifndef _PARALLELLCD_
#include <LiquidCrystal_I2C.h>
#else
#include <LiquidCrystal.h>
#endif
    
      VolumeControl::VolumeControl() {

            volumeR = 0;
            volumeL = 0;
            balanceValue = 0;
            muteActive = false;
            balanceMode = false;
            volumeEncoder = new RotaryEncoder(ENCODER1, ENCODER2, ENCODERSW);

#ifndef _PARALLELLCD_
            lcd = new LiquidCrystal_I2C(LCDI2CADDR, LCDCOLS, LCDROWS);
#else
            lcd = new LiquidCrystal(LCDRS, LCDRW, LCDE,LCDD4, LCDD5, LCDD6, LCDD7);
#endif
                
        } // End of constructor

        


        
        void VolumeControl::writeVolume(unsigned char channel1, unsigned char channel2) {

            Serial.print("Volume Channel 1: ");
            Serial.println(channel1);
            Serial.print("Volume Channel 2: ");
            Serial.println(channel2);  
            if(channel1 <= VOLMAX && channel1 >= 0 && channel2 <= VOLMAX && channel2 >= 0) {
#ifdef __DUALVOLUME
                SPI.beginTransaction (SPISettings (SPIFREQ, MSBFIRST, SPI_MODE0));
                digitalWrite(SS, LOW);
                SPI.transfer(channel1);
                SPI.transfer(channel1);
                digitalWrite(SS, HIGH);
                SPI.endTransaction();
                SPI.beginTransaction (SPISettings (SPIFREQ, MSBFIRST, SPI_MODE0));
                digitalWrite(SS2, LOW);
                SPI.transfer(channel2);
                SPI.transfer(channel2);
                digitalWrite(S2, HIGH);
                SPI.endTransaction();
#else
                SPI.beginTransaction (SPISettings (SPIFREQ, MSBFIRST, SPI_MODE0));
                digitalWrite(SS, LOW);
                SPI.transfer(channel1);
                SPI.transfer(channel2);
                digitalWrite(SS, HIGH);
                SPI.endTransaction();
#endif 
            }
            else {
                Serial.println("ERROR: Volume values OUT OF RANGE.");
            }
   
        } // End of writeVolume()

        void VolumeControl::volumeUp() {

            if(muteActive)
                muteVolume();

            printVolume();

            for(unsigned char i = 0; i <= volumeEncoder->accel; i++)
                if(balanceValue != 0)
                    if(balanceValue < 0) { // Tilted to LEFT, then RIGHT is attenuated.
                        if(volumeL < VOLMAX)
                            volumeL++;
                        changeBalance((char)NULL);
                    }
                    else {  // Tilted to RIGHT, then LEFT is attenuated.
                        if(volumeR < VOLMAX)
                            volumeR++;
                        changeBalance((char)NULL);
                    }
                else {
                    if((volumeR < VOLMAX)) {
                        volumeR++;
                        volumeL++;    
                    }
                }

            volumeEncoder->accel = 0;
            writeVolume(volumeR, volumeL);
            printVolume();
    
        } // End of void volumeUp()

        void VolumeControl::volumeDown() {

           for(unsigned char i = 0; i <= volumeEncoder->accel; i++) {
                if(volumeR > VOLMIN)
                    volumeR--;
                if(volumeL > VOLMIN)
                    volumeL--;  
           }
           
            volumeEncoder->accel = 0;
            if(!muteActive) {
                writeVolume(volumeR, volumeL);
                printVolume();
            }
            
        } // End of void volumeDown()

        void VolumeControl::fadeIn() {

            unsigned char i = 0, j = 0;

            if(balanceValue != 0)
                if(balanceValue > 0) {
                    for(i = 0; i <= volumeR; i++) {
                        if(j < volumeL)
                            j++; 
                        writeVolume(i, j);
                        delay(FADEINDELAY); 
                    }    
                }
                else { // if(balanceValue < 0)
                    for(i = 0; i <= volumeL; i++) {
                        if(j < volumeR)
                            j++;   
                        writeVolume(j, i);
                        delay(FADEINDELAY); 
                    }
                }
           else // if(balanceValue == 0)
              for(i = 0; i <= volumeR; i++) {
                        writeVolume(i, i);
                        delay(FADEINDELAY);
                    }    

        } // End of VolumeControl::fadeIn()

        void VolumeControl::fadeOut() {

            unsigned char i = 0, j = 0;

            if(balanceValue != 0)
                if(balanceValue > 0) {
                    j = volumeL;
                    for(i = volumeR; i > VOLMIN; i--) {
                        if(j > VOLMIN)
                            j--; 
                        writeVolume(i, j);
                        delay(FADEOUTDELAY);
                    }    
                }
                else { // if(balanceValue < 0)
                    j = volumeR;
                    for(i = volumeL; i > VOLMIN; i--) {
                        if(j > VOLMIN)
                            j--; 
                        writeVolume(j, i);       
                        delay(FADEOUTDELAY);
                    }
                }
           else // if(balanceValue == 0)
              for(i = volumeR; i > VOLMIN; i--) {   
                        writeVolume(i, i);       
                        delay(FADEOUTDELAY);
                    }

        } // End of VolumeControl::fadeOut()

        void VolumeControl::setBalanceMode() {

          Serial.println("Ha entrado en setBalanceMode(): ");

            if(!balanceMode) {
                if(!muteActive)  //  While mute is active, adjusting balance makes no sense.
                    if((volumeL > 6) || (volumeR > 6)) {
                        balanceMode = true;  //  If volume is too low, adjusting balance makes no sense.
                        printBalance();
                    }
            }
            else {
                balanceMode = false;
                printVolume();
            }    

             Serial.print("Balance mode = ");
             Serial.println(balanceMode);
             Serial.print("Balance value = ");
             Serial.println(balanceValue);
        
        } // End of setBalanceMode()

      void VolumeControl::changeBalance(char direction) {

           Serial.println("Entra en cahngeBalance()");
           
           if(direction == 'L') {
                if(balanceValue < 0) {
                    if(abs(balanceValue) < BALANCEPOSITIONS)
                        balanceValue--;
                }
                else
                    balanceValue--;
           }
           else
              if(direction == 'R')
                  if(balanceValue < BALANCEPOSITIONS)
                      balanceValue++;
                          

           if(balanceValue != 0) {
                if(balanceValue < 0) {  // Tilt to LEFT, attenuate RIGHT.
                    if((int)(volumeL - (abs(balanceValue) * BALANCESTEP)) > VOLMIN) 
                        if(abs(balanceValue) == BALANCEPOSITIONS)
                            volumeR = VOLMIN;
                        else
                            volumeR = volumeL - (abs(balanceValue) * BALANCESTEP);
                    else
                        volumeR = VOLMIN;
                }
                else {      // Tilt to RIGHT, attenuate LEFT.
                    if((int)(volumeR - (balanceValue * BALANCESTEP)) > VOLMIN)
                        if(abs(balanceValue) == BALANCEPOSITIONS)
                            volumeL = VOLMIN;
                        else
                            volumeL = volumeR - (balanceValue* BALANCESTEP);
                    else
                        volumeL = VOLMIN;
                }
           }
           else  // Balance to center point.
              if(volumeR > volumeL)
                  volumeL = volumeR;
              else
                  volumeR = volumeL;

           Serial.print("Balance value: ");
           Serial.println(balanceValue);
           writeVolume(volumeR, volumeL);

           if(balanceMode)
                printBalance();
        
      } // End of void VolumeControl::changeBalance()


      void VolumeControl::muteVolume() {  

          if(muteActive) {
              muteActive = false;
              printVolume();
              digitalWrite(MUTE, HIGH);
#ifdef _DUALVOLUME_
              digitalWrite(MUTE2, HIGH);
#endif
              // Posible conectar relés de salida si se desconectaron en mute.
              fadeIn();
          }
          else {
              muteActive = true;
              printVolume();
              fadeOut();
              // Desconectar relés de salida. Posible.
              digitalWrite(MUTE, LOW);
#ifdef _DUALVOLUME_
              digitalWrite(MUTE2, LOW);
#endif
              if(balanceMode)  // While mute is active, adjusting balance has no sense. 
                  setBalanceMode();
          }

          Serial.print("Mute active = ");
          Serial.println(muteActive);

    } // End of void VolumeControl::muteVolume

 void VolumeControl::printBalance() { // Prints a bar simultaing a sliding control to set balance.

      lcd->setCursor(0,0);
      lcd->print("B: ");
      for(unsigned int i = 3; i < LCDCOLS; i++) { // Prints the bar and an asterisk marking its center.
          lcd->setCursor(i, 0);
          lcd->print('-');
          lcd->setCursor((unsigned char)((0.5 * (LCDCOLS - 3)) + 3), 0);
          lcd->print('*');
      }
      switch(balanceValue) { // Calculates position of slide control.
            case -15:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) - 3, 0);
                break;
            case -14:
            case -13:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) - 2, 0);                            
                break;                
            case -12:
            case -11:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) - 2, 0);
                break;
            case -10:
            case -9:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) - 1, 0);
                break;
            case -8:
            case -7:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) - 1, 0);
                   break;
            case -6:
            case -5:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)), 0);
                break;
            case -4:
            case -3:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 1, 0);
                break;
            case -2:
            case -1:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 2, 0);
                break;
            case 0:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 3, 0); // Center postion.
                break;
            case 1:
            case 2:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 4, 0);
                break;
            case 3:
            case 4:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 5, 0);
                break;
            case 5:
            case 6:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 6, 0);
                break;
            case 7:
            case 8:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 7, 0);
                break;
            case 9:
            case 10:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 7, 0);
                break;
            case 11:
            case 12:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 8, 0);
                break;
            case 13:
            case 14:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 8, 0);
                break;                    
            case 15:
                lcd->setCursor((unsigned char) (0.5 * (LCDCOLS - 3)) + 9, 0);
                break;
      } // End of switch(balanceValue)
      lcd->print((char)255);  // Prints the slide control.

 }  // End of VolumeControl::printBalance()

 void VolumeControl::printVolume() {

      float volumeLdB, volumeRdB;
      lcd->setCursor(0,0); 
      lcd->print("L: ");
      if(muteActive) {
          lcd->clear();
          printCentered("Mute", 0);
      }
      else {
          //lcd->clear();
          volumeLdB = (31.5 - 0.5 * (255 - volumeL)) + VOLOFFSET;
          volumeRdB = (31.5 - 0.5 * (255 - volumeR)) + VOLOFFSET;
          lcd->print("L:");
          lcd->setCursor(2, 0);
          if(volumeLdB >= 0)
              lcd->print('+');
          lcd->print(volumeLdB);
          lcd->setCursor(7, 0);
          lcd->print("  R:");
          if(volumeRdB >= 0)
              lcd->print('+');
          lcd->print(volumeRdB);
      }
 }  // End of VolumeControl::printVolume()
        
 void VolumeControl::printCentered(char *text, unsigned char row) {

      unsigned char nBlanks, strLength;

      strLength = strlen(text);
  
      if(strLength < LCDCOLS)
          nBlanks = (unsigned char)(LCDCOLS - strLength)/ 2;
      else 
          if(strLength == LCDCOLS)
              nBlanks = 0;
      lcd->setCursor(nBlanks, row);
      lcd->print(text);
            
} // End of void VolumeControl::printCentered()
