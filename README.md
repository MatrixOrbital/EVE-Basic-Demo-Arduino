**Basic EVE Arduino UNO Example Code**

Written for an [**EVE2**](https://www.matrixorbital.com/ftdi-eve/eve-ft812) or [**EVE3**](https://www.matrixorbital.com/ftdi-eve/eve-bt815-bt816) or [**EVE4**](https://www.matrixorbital.com/ftdi-eve/eve-bt817-bt818) running on an Arduino UNO.

This code when executed will display MATRIX ORBITAL text and a blue circle.
![alt text](https://raw.githubusercontent.com/MatrixOrbital/Basic-EVE-Demo/master/Screens/Basic-EVE-Demo-2.jpg)

This code will illustrate:
* EVE initilization
* Set colour
* Draw blue dot
* Write text

*please note, this example does not utilize touch*

**IMPORTANT**

1. Download and rename the directory from Basic-EVE-Demo-Arduino-master to Basic-EVE-Demo-Arduino

2. The example is configured for a 5.0", to change the display used edit:

: Eve2_81x.h --> // Panel specific setup parameters, SELECT YOUR DISPLAY

:: //User selectable configurations.
:: //#define EVE2_70
:: #define EVE2_50
:: //#define EVE2_43
:: //#define EVE2_38
:: //#define EVE2_35
:: //#define EVE2_29

The settings work for EVE2, EVE3 and EVE4 displays.

------------------------------------------------------------------

**Hardware Requirements:**

- Designed for Matrix Orbital EVE2/3/4 SPI TFT Displays
  https://www.matrixorbital.com/ftdi-eve  
![alt text](https://www.lcdforums.com/forums/download/file.php?id=1433)
  
  
-  Scoodo EVE TFT SHIELD for Arduino
  https://www.matrixorbital.com/eve2-shield  
![alt text](https://www.lcdforums.com/forums/download/file.php?id=1432)

-  Arduino UNO, external power will be required most likely



**LIBRARY**
- This code makes use of the Matrix Orbital EVE2 Library found here: 

  https://github.com/MatrixOrbital/EVE2-Library

  - While a copy of the library files (Eve2_81x.c and Eve2_81x.h) is included here, it will not be updated. For the most up to date files please use the files from the link above.

 **EVE SPI TFT connected to a Scoodo SHIELD**
 
![alt text](https://www.lcdforums.com/forums/download/file.php?id=1430)
  
  **Support Forums**
  
  http://www.lcdforums.com/forums/viewforum.php?f=45
