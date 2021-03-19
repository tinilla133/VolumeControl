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

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "setup.h"

class PushButton;

class RotaryEncoder {

    private:
        unsigned char pin1, pin2, pinSw;
        
    public:

        long int turns, lastTurns;
        unsigned int accel;
        PushButton *pushButton;

        RotaryEncoder(unsigned char a, unsigned char b, unsigned char c);
        int rotate();  // Returns -1 case of CCW rotation and 1 case of CW.

};  // End of class RotaryEncoder

class PushButton {

    private:

        unsigned char pin;

    public:

        bool shortPushed, longPushed;
        PushButton(unsigned char buttonPin);
        void pushed(unsigned int aWhile);

}; // End of class pushButton

#endif
