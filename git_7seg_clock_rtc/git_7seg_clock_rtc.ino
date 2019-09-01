/*  ******   VinoJV  ******   Working   *******  
 *   
 * https://github.com/vinojv96
 * Be sure to check if you have a common anode or cathode display.(change @ line 26)
 * 
 */
#include <Wire.h>
#include <SevSeg.h> //https://github.com/sparkfun/SevSeg/tree/master/src
#include <blink.h> //can be installed from arduino library
#define DS1307_ADDRESS 0x68
//Create an instance
SevSeg sevseg; 
Blink led(13,500);
byte zero = 0x00; 
unsigned long timer;
void setup()
{
  pinMode(13, OUTPUT);
  //Serial.begin(9600);
  Wire.begin();
  //Comment this line after you have set the date @ void TimeSelection() below
  // TimeSelection(); //Uncomment this line when your setting the time. Set time @ (line 66)
  //Change to COMMON_ANODE if you have a Common anode display.
  byte numDigits = 4;
  byte digitPins[] = {12, 11, 10, 9}; 
  byte segmentPins[] = {2, 7, 6, 5, 4, 3, 8, 18};
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default. Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  //Initialize display
   sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros);
  //Set brightness
  sevseg.setBrightness(90);
  timer = millis();
}
 
void loop()
{
  led.Update();
  char tempString[10]; //Used for sprintf
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, 7);
  int seconds = ConvertToDecimal(Wire.read());
  int minutes = ConvertToDecimal(Wire.read());
  int hours = ConvertToDecimal(Wire.read() & 0b111111); 
  sprintf(tempString, "%02d%02d", hours, minutes);
  sevseg.setChars(tempString);
  sevseg.refreshDisplay(); // Must run repeatedly
  
}

void TimeSelection()   //Select date and time here below!!(line 69,70,71)
//after setting time also comment time TimeSelection() @ void setup
{
  byte seconds = 00; //Values 0 to 59
  byte minutes = 14; //Values 0 to 59
  byte hours = 11; //Values  0 to 23
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(zero); 
  Wire.write(ConvertToPCD(seconds));
  Wire.write(ConvertToPCD(minutes));
  Wire.write(ConvertToPCD(hours));
  Wire.write(zero);
  Wire.endTransmission(); 
}
 
byte ConvertToPCD(byte val)
{ 
  //Convert Decimal to binary
  return ( (val/10*16) + (val%10) );
}
 
byte ConvertToDecimal(byte val)  
{ 
  //Convert binary to decimal
  return ( (val/16*10) + (val%16) );
}
