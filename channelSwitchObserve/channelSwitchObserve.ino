#include<SoftwareSerial.h>
SoftwareSerial mySerial(2,3);
unsigned long startTime, deltaTime;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);
  mySerial.begin(1200);
  delay(2000);
  Serial.println("Start");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mySerial.available())
  {
    Serial.write( mySerial.read());
  }
}
