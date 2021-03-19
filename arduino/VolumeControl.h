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

 #ifndef _VolumeControl_H_
 #define _VolumeControl_H_
 
 #include "setup.h"
 #include "buttons.h"
 #include <SPI.h>
 #ifndef _PARALLELLCD_
 #include <LiquidCrystal_I2C.h>
 #else
 #include <LiquidCrystal.h>
 #endif


class RotaryEncoder;
#ifndef _PARALLELLCD_
class LiquidCrystal_I2C;
#else
class LiquidCrystal;
#endif
class IRrecvPCI;
class IRdecode;

class VolumeControl {

    private: 
    
        unsigned char volumeR, volumeL;
        int balanceValue; 
        bool muteActive; 

    public:

        RotaryEncoder *volumeEncoder;
#ifndef _PARALLELLCD_
        LiquidCrystal_I2C *lcd;
#else
        LiquidCrystal *lcd;
#endif
        bool balanceMode, menuMode;

        VolumeControl();
        void writeVolume(unsigned char channel1, unsigned char channel2);
        void volumeUp();
        void volumeDown();
        void fadeIn();
        void fadeOut();
        void setBalanceMode();
        void changeBalance(char direction);
        void muteVolume();
        void printVolume();
        void printBalance();
        void printCentered(char *text, unsigned char row);
        
}; // End of class VolumeControl

#endif
