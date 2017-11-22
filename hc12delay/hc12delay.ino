//HC-12 messenger send/receive
//autor Tom Heylen tomtomheylen.com

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX
unsigned long count = 0;
unsigned long del = 0;
unsigned char in[5];
int check = 0;
void setup() {
  //pinMode(LED_BUILTIN , OUTPUT);
  //pinMode(4,OUTPUT);
  Serial.begin(1200);
  mySerial.begin(1200);
  Serial.println("Start");
}

void loop() {
  if (millis() - count >= 3500 && check == 0) { //Read from HC-12 and send to serial monitor
    //while (Serial.available()) Serial.read();
    mySerial.write("01234");
    count = millis();
    //Serial.print(count);
    Serial.println(" s");
    check = 1;
  }
  if (mySerial.available())
  {
    mySerial.readBytes(in,5);
    del = millis()-count;
    Serial.println();
    Serial.println(del);
    count = millis();
    check = 0;
  }
}
