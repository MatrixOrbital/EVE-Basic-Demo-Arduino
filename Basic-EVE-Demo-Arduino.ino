#include <stdint.h>       // Find types like "uint8_t"
#include <stdbool.h>      // Find type "bool"
#include <SPI.h>          // Arduino SPI library
#include <SD.h>           // Arduino SD card library

#include "Eve2_81x.h"     // Matrix Orbital Eve2 Library      
#include "Arduino_AL.h"   // Hardware abstraction layer for Arduino

// A global buffer for use with debug messages.  It is always used up immediately so 
// never retains initialized values.  Prevent further allocations of RAM for logging
char LogBuf[LogBufSize];

void MakeScreen_MatrixOrbital(uint8_t DotSize)
{
	Send_CMD(CMD_DLSTART);                   //Start a new display list
 	Send_CMD(VERTEXFORMAT(0));               //setup VERTEX2F to take pixel coordinates
	Send_CMD(CLEAR_COLOR_RGB(0, 0, 0));      //Determine the clear screen color
	Send_CMD(CLEAR(1, 1, 1));	              //Clear the screen and the curren display list
	Send_CMD(COLOR_RGB(26, 26, 192));        // change colour to blue
	Send_CMD(POINT_SIZE(DotSize * 16));      // set point size to DotSize pixels. Points = (pixels x 16)
	Send_CMD(BEGIN(POINTS));                 // start drawing point
	Send_CMD(TAG(1));                        // Tag the blue dot with a touch ID
	Send_CMD(VERTEX2F(DWIDTH / 2, DHEIGHT / 2));     // place blue point
	Send_CMD(END());                         // end drawing point
	Send_CMD(COLOR_RGB(255, 255, 255));      //Change color to white for text
	Cmd_Text(DWIDTH / 2, DHEIGHT / 2, 30, OPT_CENTER, " MATRIX         ORBITAL"); //Write text in the center of the screen
	Send_CMD(DISPLAY());                     //End the display list
	Send_CMD(CMD_SWAP);                      //Swap commands into RAM
	UpdateFIFO();                            // Trigger the CoProcessor to start processing the FIFO
}

void setup()
{
  GlobalInit();
  FT81x_Init();
  MakeScreen_MatrixOrbital(30);
  wr8(REG_PWM_DUTY + RAM_REG,128);
}

void loop()
{
  MainLoop(); // jump to "main()"
}
void MainLoop(void)
{
  while(1)
  {
  }
}

// ************************************************************************************
// Following are wrapper functions for C++ Arduino functions so that they may be      *
// called from outside of C++ files.  These are also your opportunity to use a common *
// name for your hardware functions - no matter the hardware.  In Arduino-world you   *
// interact with hardware using Arduino built-in functions which are all C++ and so   *
// your "abstraction layer" must live in this xxx.ino file where C++ works.           *
//                                                                                    *
// Incidentally, this is also an alternative to ifdef-elif hell.  A different target  *
// processor or compiler will include different files for hardware abstraction, but   *
// the core "library" files remain unaltered - and clean.  Applications built on top  *
// of the libraries need not know which processor or compiler they are running /      *
// compiling on (in general and within reason)                                        *
// ************************************************************************************

void GlobalInit(void)
{
  Serial.begin(115200);
  while (!Serial) {;}                    // wait for serial port to connect.

  // Matrix Orbital Eve display interface initialization
  pinMode(EvePDN_PIN, OUTPUT);            // Pin setup as output for Eve PDN pin.
  digitalWrite(EvePDN_PIN, LOW);          // Apply a resetish condition on Eve
  pinMode(EveChipSelect_PIN, OUTPUT);     // SPI CS Initialization
  digitalWrite(EveChipSelect_PIN, HIGH);  // Deselect Eve
  pinMode(EveAudioEnable_PIN, OUTPUT);    // Audio Enable PIN
  digitalWrite(EveAudioEnable_PIN, LOW);  // Disable Audio
  SPI.begin();                            // Enable SPI
}

// Send a single byte through SPI
void SPI_WriteByte(uint8_t data)
{
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  digitalWrite(EveChipSelect_PIN, LOW);

  SPI.transfer(data);
      
  digitalWrite(EveChipSelect_PIN, HIGH);
  SPI.endTransaction();
}

// Send a series of bytes (contents of a buffer) through SPI
void SPI_WriteBuffer(uint8_t *Buffer, uint32_t Length)
{
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  digitalWrite(EveChipSelect_PIN, LOW);

  SPI.transfer(Buffer, Length);
      
  digitalWrite(EveChipSelect_PIN, HIGH);
  SPI.endTransaction();
}

// Send a byte through SPI as part of a larger transmission.  Does not enable/disable SPI CS
void SPI_Write(uint8_t data)
{
//  Log("W-0x%02x\n", data);
  SPI.transfer(data);
}

// Read a series of bytes from SPI and store them in a buffer
void SPI_ReadBuffer(uint8_t *Buffer, uint32_t Length)
{
  uint8_t a = SPI.transfer(0x00); // dummy read

  while (Length--)
  {
    *(Buffer++) = SPI.transfer(0x00);
  }
}

// Enable SPI by activating chip select line
void SPI_Enable(void)
{
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  digitalWrite(EveChipSelect_PIN, LOW);
}

// Disable SPI by deasserting the chip select line
void SPI_Disable(void)
{
  digitalWrite(EveChipSelect_PIN, HIGH);
  SPI.endTransaction();
}

void Init_Keys(void)
{
  // Pin setup as input for each button pin
  pinMode(Button1_PIN, INPUT);
  pinMode(Button2_PIN, INPUT);
  pinMode(Button3_PIN, INPUT);
}

void Eve_Reset_HW(void)
{
  // Reset Eve
  SetPin(EvePDN_PIN, 0);                    // Set the Eve PDN pin low
  MyDelay(50);                             // delay
  SetPin(EvePDN_PIN, 1);                    // Set the Eve PDN pin high
  MyDelay(100);                            // delay
}

void DebugPrint(char *str)
{
  Serial.print(str);
}

// A millisecond delay wrapper for the Arduino function
void MyDelay(uint32_t DLY)
{
  uint32_t wait;
  wait = millis() + DLY; while(millis() < wait);
}

// An abstracted pin write that may be called from outside this file.
void SetPin(uint8_t pin, bool state)
{
  digitalWrite(pin, state); 
}

// An abstracted pin read that may be called from outside this file.
uint8_t ReadPin(uint8_t pin)
{
  return(digitalRead(pin));
}
