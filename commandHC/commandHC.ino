//HC-12 messenger send/receive
//autor Tom Heylen tomtomheylen.com

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX
int count = 0;
void setup() {
  //pinMode(LED_BUILTIN , OUTPUT);
  //pinMode(4,OUTPUT);
  Serial.begin(1200);
  mySerial.begin(1200);
  Serial.println("Start");
}

void loop() {
  if (Serial.available()) { //Read from HC-12 and send to serial monitor
    mySerial.print(Serial.readString());
  }
  if (mySerial.available())
  {
    Serial.println(mySerial.readString());
  }
}
