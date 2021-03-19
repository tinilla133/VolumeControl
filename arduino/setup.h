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

#ifndef _SETUP_H_
#define _SETUP_H_

#define SERIAL_BRATE 115200
// #define _DUALVOLUME_  // If amplifier uses one volume chip per channel. For example, in a balanced volume control. Comment for using single chip for two channels.
//#define _PARALLELLCD_ // If LCD wants to be controlled parallel-ish. Comment for serial I2C display.

#define SHORT_PUSH_TIME 600 // Time in milliseconds to consider a short push on pushbutton.
#define LONG_PUSH_TIME 2000 // Time in milliseconds to consider a long push on pushbutton.
#define MAXPUSHEDTIME 10000   // Max time a button can be pushed. Longer time pulsation is discarded.
#define PUSHDELAY 30     // Delay to avoid false pulsation or too short time for triggering a function from a push button.
#define ENCODERDELAY 20   // Delay to avoid or reduce false lectures from rotary ecoder.
#define TIMEFORACCEL 100    // Time in ms for sampling the rotary encoder acceleration.
#define SPIFREQ 200000  // Frequency (in Hz) of SPI bus transmissions.
#define VOLMIN 0          // Minimum volume value for TI PGA volume chip.
#define VOLMAX 192        // Maximum volume value for TI PGA volume chip. If you want to use full in-chip gain set it to 255 if not gain is defined in datasheets as G = 31.5 - 1/2 * (255 - N)) dB 
#define VOLOFFSET 20      // Fixed amplifier output in dB used for displaying the right volume value relative to 0 dB.
#define BALANCEPOSITIONS 15   // Number of balance positions (or steps) for each channel.
#define BALANCESTEP 4   // Value of each step (or position) of attenuation with using balance referred to integers used by PGA.  [Vol(dB) = 2 * vol(N), because G = 31.5 - 1/2 * (255 - N)) dB] 
#define FADEOUTDELAY 1  // Slope of fade out curve.
#define FADEINDELAY 5   // Slope of fade in curve.
#define LCDROWS 2     // Number of rows of LCD.
#define LCDCOLS 16    // Number of columns of LCD.
#define LCDI2CADDR 0x3F // Address of LCD I2C interface.

// Pins configuration
// Volume chip SCLK and MOSI must be connection acording SPI pins of your Arduino board.
// I2C Display pins must be connected according to SDA and SCL pins of your Arduino board.
#define ENCODER1 2      // Rotary encoder for volume. Output 1. 
#define ENCODER2 3      // Rotary encoder for volume. Output 2. 
#define ENCODERSW 6    // Pushbutton for toggling balance adjustment. Switch of volume encoder used.
#define SS 53          // CS for volume chip.
#define MUTE 47        // MUTE for volume chip.
// Pins definition for dual chip volume control.
#ifdef _DUALVOLUME_
#define SS2 48
#define MUTE2 49
#endif  // _DUALVOLUME_
// Pins definition for parallel LCD.
#ifdef _PARALLELLCD_
#define _LCDRS 30
#define _LCDRW 31
#define _LCDE 32
#define _LCDD4 33
#define _LCDD5 34
#define _LCDD6 35
#define _LCDD7 36
#define _LCDBACK 37
#endif // _PARALLELLCD_


#endif // _SETUP_H_
