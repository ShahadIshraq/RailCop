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

void send_ack(unsigned char in)
{
  in = in | 0b00100000;
  in = in & 0b11101111;
  mySerial.write(in); 
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
  // put your main code here, to run repeatedly:
  if(mySerial.available())
  {
    Serial.println(millis());
    unsigned char in = mySerial.read();
    if((in & 0b00110000) == 0b00100000) Serial.print("Recieved ack : ");
    else 
    {
      send_ack(in);
      Serial.print("Recieved and acked : ");
    }
    
    Serial.println(show_binary(in));
    Serial.println();
    //deltaTime = millis()-startTime;
    //Serial.println(deltaTime);
  }
  if(Serial.available())
  {
    
    String choice = Serial.readString();
    unsigned char out = 0 ;
    for (int i = 0 ; i < 8 ; i++)
    {
      if(choice[i] == '1') out |= (1<<(7-i));
    }
    Serial.print("Sending : ");
    Serial.println(show_binary(out));
    mySerial.write(out);
    Serial.println(millis());
    /*
    ------------------------------------------------------------------
    | 2bit dest id | 2 bit  pckt type | 2bit report | reserved 2 bit |
    ------------------------------------------------------------------

  Packet type : 
   00 = Query from train
   01 = Query from previous node
   10 = Ack
   11 = Reply

 
 */
    //mySerial.write(choice);
  }

}
