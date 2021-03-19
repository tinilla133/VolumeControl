# VolumeControl

Stereo Volume Control using Texas Instruments PGA 2310, PGA 2311,
   Cirrus Logic CS3310 and compatibles and Arduino.
   
   (C) 2021 Quality Hi-Fi Works Audio, S. L.
   
   https://www.qhwaudio.com
   
   By: Francisco J Vizcaya López  \<franciscovl@qhwaudio.com\>
   
   ### Features:
   * Balance control.
   * Mute function.
   * Fade in and fade out.
   * Output results to an LCD display with Hitachi HD4478 controller or compatible.
     
   ### General description of working:
   * Rotary encoder controls volume and balance.
   * When you do a long push on push button, program will enter in 
        "balance mode". While in this mode rotary encoder will control 
        balance instead volume.
   * While in balance mode, if you push push button, it will output this mode.
   * While in "nomal mode", if you push push button shortly, it will perform muting
       funcion with fade out.
   * While in "mute mode", if you push push putton shortly it will out muting with fade in.
   * While in "mute mode", if you rotate clockwise, it will output muting and 
       lift volume.
   
   ### License:
   
   This software is distributed under terms and conditions of the
   
   GNU LESSER GENERAL PUBLIC LICENSE Version 3 (GNU LGPLv3)
   
   which are distributed among with this software and can be found at GNU site:
   https://www.gnu.org/licenses/lgpl-3.0.html
   
   
