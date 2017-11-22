// include the library code:
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //RX, TX

void setup() {  
  mySerial.begin(1200);
  Serial.begin(1200);
  Serial.println("start ");
  
}


String show_binary(unsigned char u) 
{ 
  String a = "";

  int t = pow(2, 7);   // t is the max number that can be represented

  for(t; t>0; t = t/2)           // t iterates through powers of 2
      if(u >= t){                // check if u can be represented by current value of t
          u -= t;
          a += "1";               // if so, add a 1
      }
      else {
          a += "0";               // if not, add a 0
      }

  return a ;                     // returns string
}


void loop() {
  if(mySerial.available()){
    unsigned char in = mySerial.read();
    Serial.println(show_binary(in));
  }
}

