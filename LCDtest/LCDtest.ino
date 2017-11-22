/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 7
 * LCD D5 pin to digital pin 6
 * LCD D6 pin to digital pin 5
 * LCD D7 pin to digital pin 4
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */


#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

int packetCount = 0 ;

void setup() {
  mySerial.begin(1200);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Starting");
  delay(2500);
  lcd.clear();
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print(packetCount);
  if(mySerial.available() > 0){//Read from HC-12 and send to serial monitor
    lcd.setCursor(0, 1);
    char input = mySerial.read();
    lcd.write(input);
    packetCount++;   
  }
  
}

