 ## Stereo Volume Control using Texas Instruments PGA 2310, PGA 2311, Cirrus Logic CS3310 and compatibles.
 
 ## Classes:
 ### class VolumeControl
 #### Methods:
 * void writeVolume(unsigned char channel1, unsigned char channel2): writes channel1 and channel2 values to volume chip using SPI.
 * volumeUp(): increases volume by 1; the current value, does not write anything to volume chip.
 * volumeDown(): same as prior but decreases volume.
 * fadeIn(): changes volume from 0 to current value, step by step. Writes each step (by calling writeVolume()) to volume chip.
 * fadeOut(): 
