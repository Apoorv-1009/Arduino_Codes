/*
   AMT22_SPI_Sample_Code.ino
   Company: CUI Inc.
   Author: Jason Kelly
   Version: 2.0.1.0
   Date: August 20, 2019

   This sample code can be used with the Arduino Uno to control the AMT22 encoder.
   It uses SPI to control the encoder and the the Arduino UART to report back to the PC
   via the Arduino Serial Monitor.
   For more information or assistance contact CUI Inc for support.

   After uploading code to Arduino Uno open the open the Serial Monitor under the Tools
   menu and set the baud rate to 115200 to view the serial stream the position from the AMT22.

   Arduino Pin Connections
   SPI Chip Select Enc 0:   Pin  2
   SPI Chip Select Enc 1:   Pin  3
   SPI MOSI                 Pin 11
   SPI MISO                 Pin 12
   SPI SCLK:                Pin 13


   AMT22 Pin Connections
   Vdd (5V):                Pin  1
   SPI SCLK:                Pin  2
   SPI MOSI:                Pin  3
   GND:                     Pin  4
   SPI MISO:                Pin  5
   SPI Chip Select:         Pin  6
*/


/* Include the SPI library for the arduino boards */
#include <SPI.h>

/* Serial rates for UART */
#define BAUDRATE        115200

/* SPI commands */
#define AMT22_NOP       0x00
#define AMT22_RESET     0x60
#define AMT22_ZERO      0x70

/* Define special ascii characters */
#define NEWLINE         0x0A
#define TAB             0x09

/* We will use these define macros so we can write code once compatible with 12 or 14 bit encoders */
#define RES12           12
#define RES14           14

/* SPI pins */
#define ENC_0            2
#define ENC_1            3
#define SPI_MOSI        11
#define SPI_MISO        12
#define SPI_SCLK        13

void setup()
{
  //Set the modes for the SPI IO
  pinMode(SPI_SCLK, OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_MISO, INPUT);
  pinMode(ENC_0, OUTPUT);
  pinMode(ENC_1, OUTPUT);

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);

  //Initialize the UART serial connection for debugging
  Serial.begin(BAUDRATE);

  //Get the CS line high which is the default inactive state
  digitalWrite(ENC_0, HIGH);
  digitalWrite(ENC_1, HIGH);

  //set the clockrate. Uno clock rate is 16Mhz, divider of 32 gives 500 kHz.
  //500 kHz is a good speed for our test environment
  SPI.setClockDivider(SPI_CLOCK_DIV32);    // 500 kHz

  //start SPI bus
  SPI.begin();
}

void loop()
{
  //create a 16 bit variable to hold the encoders position
  uint16_t encoderPosition;
  //let's also create a variable where we can count how many times we've tried to obtain the position in case there are errors
  uint8_t attempts;


  //if you want to set the zero position before beginning uncomment the following function call

  //setZeroSPI(ENC_0);
  
  // Set target
  int target = 8000;

  //Define PID constants
  float kp = 0.07;
  float kd = 0.006;
  float ki = 0.009;

  int error = 0;
  int prev_error = 0;
  int e_int = 0;
  int e_dot = 0;
  
  float initial_time = 0;
  float final_time = 0;
  //once we enter this loop we will run forever
  while (1)
  {
    //set attemps counter at 0 so we can try again if we get bad position
    attempts = 0;
    //this function gets the encoder position and returns it as a uint16_t
    //send the function either res12 or res14 for your encoders resolution
    encoderPosition = getPositionSPI(ENC_0, RES14);

    //if the position returned was 0xFFFF we know that there was an error calculating the checksum
    //make 3 attempts for position. we will pre-increment attempts because we'll use the number later and want an accurate count
    while (encoderPosition == 0xFFFF && ++attempts < 3)
    {
      encoderPosition = getPositionSPI(ENC_0, RES14); //try again
    }

    if (encoderPosition == 0xFFFF) //position is bad, let the user know how many times we tried
    {
      Serial.print("Encoder 0 error. Attempts: ");
      Serial.print(attempts, DEC); //print out the number in decimal format. attempts - 1 is used since we post incremented the loop
//      Serial.write(NEWLINE);
    }
    else //position was good, print to serial stream
    {

//      Serial.print("Encoder 0: ");
//      Serial.print(encoderPosition, DEC); //print the position in decimal format
//      Serial.print("\t");
//      Serial.write(NEWLINE);
    } 
    
    final_time = micros();
    float delta_T = (final_time-initial_time)/(1000000);
//    Serial.print(delta_T);
//    Serial.print("\n");
    ///////////////////////////// Code block for PID /////////////////////////////
    //Error
    error = encoderPosition - target;
//      error = target - encoderPosition; 
//    Serial.println(error);
//    Serial.println(target);
    Serial.print("error: ");
    Serial.print(error);
    Serial.print("\t");
//    Serial.print("\t");

    //Derivative
    e_dot = (error - prev_error)/(delta_T);

    //Integral
    e_int = e_int + error*delta_T;

    //Control Signal
    float u = kp*error + kd*e_dot + ki*e_int;
    
    if(u>0)
      digitalWrite(4, HIGH);

    if(u<0)
      digitalWrite(4, LOW);

//    if(abs(error)< 100)
//      u = 0;

    int u_max = 100;
    u = abs(u);
    if(u > u_max)
      u = u_max;

    Serial.print("u: ");
    Serial.print(u);
//    Serial.print("\t");
    analogWrite(5, u);

    prev_error = error;
    initial_time = micros();
    //////////////////////////////////////////////////////////////////////////////
    
    //For the purpose of this demo we don't need the position returned that quickly so let's wait a half second between reads
    //delay() is in milliseconds
    delay(10);
    Serial.print("\n");
  }
}

/*
   This function gets the absolute position from the AMT22 encoder using the SPI bus. The AMT22 position includes 2 checkbits to use
   for position verification. Both 12-bit and 14-bit encoders transfer position via two bytes, giving 16-bits regardless of resolution.
   For 12-bit encoders the position is left-shifted two bits, leaving the right two bits as zeros. This gives the impression that the encoder
   is actually sending 14-bits, when it is actually sending 12-bit values, where every number is multiplied by 4.
   This function takes the pin number of the desired device as an input
   This funciton expects res12 or res14 to properly format position responses.
   Error values are returned as 0xFFFF
*/
uint16_t getPositionSPI(uint8_t encoder, uint8_t resolution)
{
  uint16_t currentPosition;       //16-bit response from encoder
  bool binaryArray[16];           //after receiving the position we will populate this array and use it for calculating the checksum

  //get first byte which is the high byte, shift it 8 bits. don't release line for the first byte
  currentPosition = spiWriteRead(AMT22_NOP, encoder, false) << 8;

  //this is the time required between bytes as specified in the datasheet.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3);

  //OR the low byte with the currentPosition variable. release line after second byte
  currentPosition |= spiWriteRead(AMT22_NOP, encoder, true);

  //run through the 16 bits of position and put each bit into a slot in the array so we can do the checksum calculation
  for (int i = 0; i < 16; i++) binaryArray[i] = (0x01) & (currentPosition >> (i));

  //using the equation on the datasheet we can calculate the checksums and then make sure they match what the encoder sent
  if ((binaryArray[15] == !(binaryArray[13] ^ binaryArray[11] ^ binaryArray[9] ^ binaryArray[7] ^ binaryArray[5] ^ binaryArray[3] ^ binaryArray[1]))
      && (binaryArray[14] == !(binaryArray[12] ^ binaryArray[10] ^ binaryArray[8] ^ binaryArray[6] ^ binaryArray[4] ^ binaryArray[2] ^ binaryArray[0])))
  {
    //we got back a good position, so just mask away the checkbits
    currentPosition &= 0x3FFF;
  }
  else
  {
    currentPosition = 0xFFFF; //bad position
  }

  //If the resolution is 12-bits, and wasn't 0xFFFF, then shift position, otherwise do nothing
  if ((resolution == RES12) && (currentPosition != 0xFFFF)) currentPosition = currentPosition >> 2;

  return currentPosition;
}

/*
   This function does the SPI transfer. sendByte is the byte to transmit.
   Use releaseLine to let the spiWriteRead function know if it should release
   the chip select line after transfer.
   This function takes the pin number of the desired device as an input
   The received data is returned.
*/
uint8_t spiWriteRead(uint8_t sendByte, uint8_t encoder, uint8_t releaseLine)
{
  if(sendByte == AMT22_ZERO)
  {
    Serial.print("Set zero active \n");
  }
  //holder for the received over SPI
  uint8_t data;

  //set cs low, cs may already be low but there's no issue calling it again except for extra time
  setCSLine(encoder , LOW);

  //There is a minimum time requirement after CS goes low before data can be clocked out of the encoder.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3);

  //send the command
  data = SPI.transfer(sendByte);
  delayMicroseconds(3); //There is also a minimum time after clocking that CS should remain asserted before we release it
  setCSLine(encoder, releaseLine); //if releaseLine is high set it high else it stays low

  return data;
}

/*
   This function sets the state of the SPI line. It isn't necessary but makes the code more readable than having digitalWrite everywhere
   This function takes the pin number of the desired device as an input
*/
void setCSLine (uint8_t encoder, uint8_t csLine)
{
  digitalWrite(encoder, csLine);
}

/*
   The AMT22 bus allows for extended commands. The first byte is 0x00 like a normal position transfer, but the
   second byte is the command.
   This function takes the pin number of the desired device as an input
*/
void setZeroSPI(uint8_t encoder)
{
  spiWriteRead(AMT22_NOP, encoder, false);

  //this is the time required between bytes as specified in the datasheet.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3);

  spiWriteRead(AMT22_ZERO, encoder, true);
  delay(250); //250 millisecond delay to allow the encoder to reset
}

/*
   The AMT22 bus allows for extended commands. The first byte is 0x00 like a normal position transfer, but the
   second byte is the command.
   This function takes the pin number of the desired device as an input
*/
void resetAMT22(uint8_t encoder)
{
  spiWriteRead(AMT22_NOP, encoder, false);

  //this is the time required between bytes as specified in the datasheet.
  //We will implement that time delay here, however the arduino is not the fastest device so the delay
  //is likely inherantly there already
  delayMicroseconds(3);

  spiWriteRead(AMT22_RESET, encoder, true);

  delay(250); //250 second delay to allow the encoder to start back up
}
