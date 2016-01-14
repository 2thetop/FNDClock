//
// FNDClock.ino
// by Sung-ho Lee (NULL_POINTER)
// sh1217sh@gmail.com
// 
// =================================================================
// The MIT License 
// 
// Copyright (c) 2016 Sung-ho Lee
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// =================================================================
//
// ====== Hardware configuration ======
// RTC: DS1307
// FND: S-5462CSR2/C
//
// Pin 2 ~ Pin 8: A ~ G of FND via current limiting resistors
// Pin 9 ~ Pin 12: DIG.1 ~ DIG.4 of FND via NPN transistors
// I hard-wired the seperator colon of FND to the power line,
// but you can always modify this code if you want :)
//
// ====== Setting time ======
// Send 3 bytes of data via serial(baudrate: 9600)
// Order: second, minute, hour
// Format: BCD
// 

#include <Wire.h>

// BOF preprocessor bug prevent
#if 1
__asm volatile ("nop");
#endif

// Decides wheather numbers should be straight or upside-down. Uncomment to display flipped numbers.
//#define FND_UPSIDE_DOWN

const byte RTC_ADDRESS = B1101000;
#ifdef FND_UPSIDE_DOWN
// Flipped numbers
const boolean FND[10][7] =
  {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,0,1,1,0,1,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,1,0,1}, // 4
    {1,1,0,1,1,0,1}, // 5
    {1,1,0,1,1,1,1}, // 6
    {0,1,1,1,1,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,1,0,1}  // 9
  };
#else
// Straight numbers
const boolean FND[10][7] =
  {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,1,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
  };
#endif

byte time[3] = {0, 0, 0}; // Second, minute, hour
unsigned long lastMillis = 0;
int numberToDisplay;
int displayMem[4];

void setup() {
  // All pins should be in output mode.
  for (int i=2; i<=13; i++) {
    pinMode(i, OUTPUT);
  }

  // Begin communicating with devices.
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (abs(currentMillis - lastMillis) >= 500) {
    lastMillis = currentMillis;
    getTime();
    timeToNumber();
    splitNumber();
  }
  displayNumber();
}

void serialEvent() {
  // Recieve time data via serial connection.
  // Data formatted in BCD.
  // Order: second, minute, hour
  byte newTime[3] = {0, 0, 0};
  Serial.readBytes(newTime, 3);
  setTimeWithBCD(newTime[0], newTime[1], newTime[2]);
}

/***** Format Conversion *****/
byte decimalToBCD (byte decimal) {
  byte firstDigit = decimal / 10;
  byte secondDigit = decimal - (firstDigit * 10);
  return (firstDigit << 4) | secondDigit;
}

byte BCDToDecimal (byte BCD) {
  byte firstDigit = BCD >> 4;
  byte secondDigit = BCD & B1111;
  return (firstDigit * 10) + secondDigit;
}
/*****************************/

/***** RTC *****/
void getTime() {
  // Set resistor pointer to 0x00
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x00); // Set starting point for reading data.
  Wire.endTransmission();

  // Get actual data
  Wire.requestFrom(RTC_ADDRESS, 3); // Request 3 bytes of data from 0x00 to 0x02 which will contain second, minute and hour value.
  while (Wire.available() == 0); // Wait for the data.
  for (int i = 0; Wire.available(); i++) {
    if (i > 2) break;
    time[i] = BCDToDecimal(Wire.read());
  }
}

void setTimeWithBCD(byte second, byte minute, byte hour) {
  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x00);
  Wire.write(second);
  Wire.write(minute);
  Wire.write(hour);
  Wire.endTransmission();
}

void setTimeWithDecimal(byte second, byte minute, byte hour) {
  setTimeWithBCD(decimalToBCD(second), decimalToBCD(minute), decimalToBCD(hour));
}

/*
 * Use this function if you need.
void setClockHalt(boolean halt) {
  // Clock halt bit is on the 'second' value.
  byte data = time[0];

  // Set clock halt bit.
  // 1 will halt the clock.
  if (halt) {
    data = data | B10000000;
  }
  else {
    data = data | B00000000;
  }

  Wire.beginTransmission(RTC_ADDRESS);
  Wire.write(0x00);
  Wire.write(data);
  Wire.endTransmission();
}
*/
/***************/

/***** Display *****/
void timeToNumber() {
  // Get ready to display the time.
  numberToDisplay = (time[2] * 100) + time[1];
}

void splitNumber() {
  displayMem[0] = numberToDisplay / 1000;
  displayMem[1] = (numberToDisplay - (displayMem[0] * 1000)) / 100;
  displayMem[2] = (numberToDisplay - (displayMem[0] * 1000) - (displayMem[1] * 100)) / 10;
  displayMem[3] = (numberToDisplay - (displayMem[0] * 1000) - (displayMem[1] * 100) - (displayMem[2] * 10));
}

void displayNumber() {
  // Multiplex
  for (int i=0; i<=3; i++) {
    // Start with no output.
    for (int j=2; j<=12; j++){
      digitalWrite(j, LOW);
    }
    
    // Turn on the only transistor we want.
    digitalWrite(i+9, HIGH);

    // Display the number.
    for (int j=0; j<=6; j++) {
      digitalWrite(j+2, FND[displayMem[i]][j]);
    }
    delay(1);
  }
}
/*******************/
