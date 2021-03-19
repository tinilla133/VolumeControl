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
 *  Features:
 *    + Balance control.
 *    + Mute function.
 *    + Fade in and fade out.
 *    + Output results to an LCD display with Hitachi HD4478 controller or compatible.
 *    
 *  General description of working:
 *    + Rotary encoder controls volume and balance.
 *    + When you do a long push on push button, program will enter in 
 *       "balance mode". While in this mode rotary encoder will control 
 *       balance instead volume.
 *    + While in balance mode, if you push push button, it will output this mode.
 *    + While in "nomal mode", if you push push button shortly, it will perform muting
 *      funcion with fade out.
 *    + While in "mute mode", if you push push putton shortly it will out muting with fade in.
 *    + While in "mute mode", if you rotate clockwise, it will output muting and 
 *      lift volume.
 *  
 *  This software is distributed under terms and conditions of the 
 *   
 *   GNU LESSER GENERAL PUBLIC LICENSE Version 3 (GNU LGPLv3)
 *   
 *   which are distributed among with this software and can be found at
 *   GNU site:
 *   https://www.gnu.org/licenses/lgpl-3.0.html
 *  
 */  

#include "setup.h"
#include "VolumeControl.h"
#include "buttons.h"
#ifdef _PARALLELLCD_
#include <LiquidCrystal.h>
#else
#include <LiquidCrystal_I2C.h>
#endif
#ifdef _SOFTWARE_IRQ_
#include <TimerOne.h>
#endif


VolumeControl VolumeControl;

unsigned long int lastTurns;
int rotateDirection = 0;

void ISRVolume();

void setup() {
  
    pinMode(ENCODER1, INPUT_PULLUP);
    pinMode(ENCODER2, INPUT_PULLUP);
    pinMode(ENCODERSW, INPUT_PULLUP);
    pinMode(SS, OUTPUT);
    digitalWrite(SS, HIGH);
    pinMode(MUTE, OUTPUT);
    digitalWrite(MUTE, LOW);
#ifdef _DUALVOLUME_
    pinMode(SS2, OUTPUT);
    digitalWrite(SS2, HIGH);
    pinMode(MUTE2, OUTPUT);
    digitalWrite(MUTE2, LOW);
#endif

#ifndef _SOFTWARE_IRQ_
    attachInterrupt(digitalPinToInterrupt(ENCODER1), ISRVolume, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER2), ISRVolume, CHANGE);
#else
    TimerRTC.initialize(20000);
    Timer1.initialize(20000);
    Timer1.disablePwm(ENCODER2);
    TimerRTC.attachInterrupt(ISRVolume);
    Timer1.attachInterrupt(ISRVolume);
#endif

  Serial.begin(SERIAL_BRATE);
  
#ifndef _PARALLELLCD_
  VolumeControl.lcd->init();
#else
  VolumeControl.lcd->begin(LCDCOLS, LCDROWS);
#endif
  VolumeControl.lcd->clear();
#ifndef _PARALLELLCD_
  VolumeControl.lcd->backlight();
#endif
  SPI.begin();
  digitalWrite(SS, LOW);
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(SS, HIGH);
#ifdef _DUALVOLUME_
  digitalWrite(SS2, LOW);
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(SS2, HIGH);
#endif

  VolumeControl.printVolume();
  
}

void ISRVolume() {

     lastTurns = VolumeControl.volumeEncoder->turns;
     rotateDirection = VolumeControl.volumeEncoder->rotate();
    
}

void loop() {

    unsigned long int auxTurns;
        
    VolumeControl.volumeEncoder->pushButton->pushed(2000); // We measure time pushbutton is pushed.
    if(VolumeControl.volumeEncoder->pushButton->longPushed)
        VolumeControl.setBalanceMode();
    else
        if(VolumeControl.volumeEncoder->pushButton->shortPushed)
            if(VolumeControl.balanceMode)
                   VolumeControl.setBalanceMode();
            else
                   VolumeControl.muteVolume();

      if(rotateDirection != 0) { // Encoder is rotating.
          if(rotateDirection < 0) { // Counter clockwise rotation.
              rotateDirection = 0;
              if(VolumeControl.balanceMode) 
                  VolumeControl.changeBalance('L');
              else 
                  VolumeControl.volumeDown();
          }
          if(rotateDirection > 0) {// Clockwise direction
              rotateDirection = 0;
              if(VolumeControl.balanceMode)
                   VolumeControl.changeBalance('R');
              else
                   VolumeControl.volumeUp();
          }
      }
          
} // End of void loop()
