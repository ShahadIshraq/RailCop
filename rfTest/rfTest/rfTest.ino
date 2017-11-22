//HC-12 messenger send/receive
//autor Tom Heylen tomtomheylen.com

#include <SoftwareSerial.h>

char a = 0;
SoftwareSerial mySerial(2, 3); //RX, TX
          
void setup() {
 // pinMode(13, OUTPUT);
  Serial.begin(9600);
  mySerial.begin(1200);
}

void loop() {
 
 
  if(Serial.available()){//Read from serial monitor and send over HC-12
  char input = Serial.read();
  mySerial.write(input);    
  }
 
  if(mySerial.available() ){//Read from HC-12 and send to serial monitor
  char input = mySerial.read();
  Serial.println(input);   
//  if(a)  digitalWrite(13,LOW);
//  else digitalWrite(13,HIGH);
//  a = ~a;
  }
  delay(20);
}
