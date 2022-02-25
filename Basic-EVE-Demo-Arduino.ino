//A Basic EVE Arduino UNO Example Code example
//
//By Matrix Orbital
//
//WEB
//https://www.matrixorbital.com
//SUPPORT
//https://www.lcdforums.com/forums/viewforum.php?f=45


#include <stdint.h>       // Find types like "uint8_t"
#include <SPI.h>          // Arduino SPI library
#include "Eve2_81x.h"     // Matrix Orbital EVE Library      
#include "Arduino_AL.h"   // Hardware abstraction layer for Arduino


void MakeScreen_MatrixOrbital(uint8_t R, uint8_t G,uint8_t B) //Fuction that will draw a circle and display text
{
  Send_CMD(CMD_DLSTART);                          //Start a new display list
  Send_CMD(VERTEXFORMAT(0));                      //Setup VERTEX2F to take pixel coordinates
  Send_CMD(CLEAR_COLOR_RGB(0, 0, 0));             //Determine the clear screen color
  Send_CMD(CLEAR(1, 1, 1));                       //Clear the screen and the curren display list
  Send_CMD(COLOR_RGB(R, G, B));                    //Change colour to blue
  Send_CMD(POINT_SIZE(32 * 16));                  //Set point size to DotSize pixels. Points = (pixels x 16)
  Send_CMD(BEGIN(POINTS));                        //Start drawing point
  Send_CMD(TAG(1));                               //Tag the blue dot with a touch ID
  Send_CMD(VERTEX2F(DWIDTH / 2, DHEIGHT / 2));    //Place blue point
  Send_CMD(END());                                //End drawing point
  Send_CMD(COLOR_RGB(255, 255, 255));             //Change color to white for text
  Cmd_Text(DWIDTH / 2, DHEIGHT / 2, 30, OPT_CENTER, " MATRIX         ORBITAL"); //Write text in the center of the screen
  Send_CMD(DISPLAY());                            //End the display list
  Send_CMD(CMD_SWAP);                             //Swap commands into RAM
  UpdateFIFO();                                   //Trigger the CoProcessor to start processing the FIFO
}



void setup()  //run once, setup
{
  GlobalInit();                                   //EVE display interface initialization
  FT81x_Init();                                   //Reset and initialize the EVE
  wr8(REG_PWM_DUTY + RAM_REG,128);                //Set the backlight to full brightness
}

void loop() //main loop
{
  MakeScreen_MatrixOrbital(0, 0, 255);            //Draw a blue circle
  delay (1000);                                   //Delay for 1000ms
  MakeScreen_MatrixOrbital(255, 0, 0);            //Draw a red circle
  delay (1000);                                   //Delay for 1000ms
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

// Matrix Orbital EVE display interface initialization
void GlobalInit(void)
{
  pinMode(EvePDN_PIN, OUTPUT);            // Pin setup as output for EVE PDN pin
  digitalWrite(EvePDN_PIN, LOW);          // Apply a resetish condition on EVE
  pinMode(EveChipSelect_PIN, OUTPUT);     // SPI CS Initialization
  digitalWrite(EveChipSelect_PIN, HIGH);  // Deselect EVE
  pinMode(EveAudioEnable_PIN, OUTPUT);    // Audio Enable pin
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
