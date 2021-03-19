 ## Stereo Volume Control 
 
 Using Texas Instruments PGA 2310, PGA 2311, Cirrus Logic CS3310 and compatibles and Arduino.
 
 ## Working:
 It can be seen working in working.mp4 video.
	
 In general:
	
 * Rotary encoder controls volume and balance.
 * When you do a long push on push button, program will enter in "balance mode". While in this mode rotary encoder will control balance instead volume.
 * While in balance mode, if you push push button, it will output this mode.
 * While in "nomal mode", if you push push button shortly, it will perform muting funcion with fade out.
 * While in "mute mode", if you push push putton shortly it will out muting with fade in.
 * While in "mute mode", if you rotate clockwise, it will output muting and lift volume.
 
	
 ## Configuration:
 Configuration can be done in setup.h file.

Each parameter is described by a comment in code.
	
Details as balance positions, maximum volume (if you dont want to use full 31.5 dB of gain of chips), size of ecah balance position, slopes of fade in and fade out curves, acceleration and sensitivity of rotary encoder to suit different encoders behavior.
	
It also can be configured to use one chip per channel in a dual configuration and to use parallel or I2S display.
 
 ## Classes:
 ### class VolumeControl
 #### Methods:
 * void writeVolume(unsigned char channel1, unsigned char channel2): writes channel1 and channel2 values to volume chip using SPI.
 * volumeUp(): increases volume by 1; the current value, does not write anything to volume chip.
 * volumeDown(): same as prior but decreases volume.
 * fadeIn(): changes volume from 0 to current value, step by step. Writes each step (by calling writeVolume()) to volume chip.
 * fadeOut(): same as prior but changes from current value to 0.
 * setBalanceMode(): toggles working mode of rotary encoder from volume to balance and vice versa.
 * changeBalance(char direction): change current balance one step to left chanel if 'L' is passed as parameter or to right if 'R'. It calls writeVolume() to write values to volume chip.
 * muteVolume(): mutes output.
 * printVolume(): prints current volume to display.
 * printBalance(): prints balance graphic to display. (See video for more information.)
 * printCentered(char *text, unsigned char row): print some text centered in a row of display.
 ### class PushButton
 #### Methods:
 * pushed(unsigned int aWhile): used to determine if a pushbutton is pushed at least time in microsecond indicated by parameter aWhile. it modifies boolean variables shortPushed and longPushed to check kind of pushing.
 ### class RotaryEncoder
 #### Methods:
 * rotate(): returns -1 when rotary encoder rotates counter clockwise and 1 case of it rotates clockwise. 
