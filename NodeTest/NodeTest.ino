//HC-12 messenger send/receive
//autor Tom Heylen tomtomheylen.com

//#include <SoftwareSerial.h>

//char a = 0;
//SoftwareSerial mySerial(2, 3); //RX, TX
            
void setup() {
 pinMode(2, OUTPUT);
 digitalWrite(2,HIGH);
  Serial.begin(1200);
  //mySerial.begin(1200);
}

void loop() {
  
  
  if(Serial.available() > 0){//Read from serial monitor and send over HC-12
    String input = Serial.readString();
    String output = "Recieved : "+input;
    Serial.println(output);    
  }
// 
//  if(mySerial.available() > 0){//Read from HC-12 and send to serial monitor
//    String input = mySerial.readString();
//    mySerial.println(input);   
//    if(a)  digitalWrite(13,LOW);
//    else digitalWrite(13,HIGH);
//    a = ~a;
//  }
  //delay(20);
}
