#include <SoftwareSerial.h>

unsigned char reply;
unsigned long QT = 200;
unsigned long RAT = 200;
unsigned long RT = 4600;
int ThQ = 5;
int ThR = 5;
int ThRA = 5;

unsigned long queryStart = 0;
unsigned long ackReceived = 0;
unsigned long replyReceived = 0;

SoftwareSerial mySerial (2,3);

void transport_layer(unsigned char in);
void send_ack(unsigned char in);
int wait_for_ack(unsigned long timer);
int wait_for_reply();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);
  mySerial.begin(1200);
}


void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    unsigned char in = Serial.read();
    transport_layer(in);
    Serial.println("ok");
  }
}


void transport_layer(unsigned char in) //Also does the data-link layer job 
{
  
   Serial.println("Trying 0");
   if(in == '1') 
   {
        unsigned char query = 0b00000001;

        
        //Wait for ack on QT
        int timeOut = 0;
        int check = 0;
        while(!check)
        {
          mySerial.write(query);
          queryStart = millis();
          Serial.println(queryStart);
          timeOut++;
          if(timeOut > ThQ)
          {
            Serial.println("Failed to connect to 0");
            return;
          }
          check = wait_for_ack(QT);
        }

        Serial.println("Recieved ack.");

        //Wait for reply on RT
        timeOut = 0;
        check = 0;
    
        while(!check)
        {
          timeOut++;
          if(timeOut > ThR)
          {
            Serial.println("Not receiving reply from 0");
            return;
          }
          check = wait_for_reply();
        }
        Serial.println("Reply got");
        //Serial.println("Recieved reply at : "+replyReceived);
        send_ack(reply);
        Serial.println("Sent ack");
        //unsigned long e2eDelay = replyReceived - queryStart;
        //Serial.println("End to end delay : "+e2eDelay);        
   }
}



void send_ack(unsigned char in)
{
  unsigned char out = in | 0b00100000;
  out = out & 0b11101111;
  mySerial.write(out); 
}

int wait_for_ack(unsigned long timer)
{
  unsigned long startTime = millis();
  unsigned long currentTime = startTime;
  while(currentTime - startTime < timer)
  {
    
    if(mySerial.available())
    {
      unsigned char in = mySerial.read();
      unsigned long t = millis();
      if((in & 0b00110000) == 0b00100000) {
        replyReceived = millis();
        Serial.println(t);
        return 1;
      }
    }
    replyReceived = millis();
    currentTime = millis();
  }
  return 0;
}

int wait_for_reply()
{
  unsigned long startTime = millis();
  unsigned long currentTime = startTime;
  while(currentTime - startTime < RT)
  {
    //Serial.println("waiting");
    if(mySerial.available())
    {
      unsigned char in = mySerial.read();
      unsigned long t = millis();
      if((in & 0b00110000) == 0b00110000) {
        replyReceived = t;
        reply = in;
        Serial.println(millis());
        return 1;
      }
    }
    currentTime = millis();
  }
  return 0;
}



