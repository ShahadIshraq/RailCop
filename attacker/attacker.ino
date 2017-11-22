#include <EEPROM.h>
byte currentChannel = 1;
unsigned char ID = 0;
unsigned char reply;
unsigned long QT = 230;
unsigned long RAT = 230;
unsigned long RT = 4600;
unsigned long del = 3;
unsigned long curr = 0;
unsigned char rd = 0;

int ThQ = 5;
int ThR = 5;
int ThRA = 5;


void transport_layer(unsigned char in);
void send_ack(unsigned char in);
unsigned char change_destination_id_to_next(unsigned char in);
int wait_for_ack(unsigned long timer);
int wait_for_reply();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(200);
  switch_channel(currentChannel);
  delay(1000);
  rd =  EEPROM.read(0);
  unsigned char dell = EEPROM.read(1);
  if(rd == 0 || rd == 255 || dell == 255){ 
    EEPROM.write(0 , QT);
    EEPROM.write(1 , del);
    
  }
  else QT = rd;
  RAT = QT;
  del = dell;
  unsigned long start = millis();
  unsigned long cur = start;
  while(cur-start < 10000)
  {
    if(Serial.available())
    {
      unsigned char input = Serial.read();
      int in;
      if(input == 's')
      {
        Serial.print(QT);
        while(Serial.available() < 1);
        String inString =  Serial.readString();
        in = inString.toInt();
        QT = in;
        RAT = in;
        EEPROM.update(0,in);
        delay(100);
        Serial.println(QT);
        Serial.println(del);
        while(Serial.available() < 1);
        inString =  Serial.readString();
        del = inString.toInt();
        EEPROM.update(1,del);
        delay(100);
        Serial.print(del);
      }
    }
    cur = millis();
  }

  //Serial.println(del*1000);

  
}

void change_current_channel()
{
  if(currentChannel == 1) currentChannel = 2;
  else currentChannel = 1;
}

void switch_channel(byte channel)
{
  digitalWrite(2, LOW);
  delay(100);
  if(channel == 1)
  {
    Serial.write("AT+C001");
  }
  else if (channel == 2)
  {
    Serial.write("AT+C002");
  }
  delay(200);
  digitalWrite(2, HIGH);
  while(Serial.available())
  {
    Serial.read();
  }
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(curr == 0 || (millis()-curr) > del*1000 )
  {
    while(Serial.available()) Serial.read();
    transport_layer('1');
    //Serial.println("out");
  }

  
}

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


void transport_layer(unsigned char in) //Also does the data-link layer job 
{
  
   //Serial.println("Trying 0");
   if(in == '1') 
   {
        unsigned char query = 0b00000000;

        
        //Wait for ack on QT
        int timeOut = 0;
        int check = 0;
        while(!check)
        {
          Serial.write(query);
          curr = millis();
          //Serial.println(queryStart);
          timeOut++;
          if(timeOut > ThQ)
          {
            //Serial.println("Failed to connect to 0");
            return;
          }
          check = wait_for_ack(QT);
        }

        //Serial.println("Recieved ack.");

        //Wait for reply on RT
        timeOut = 0;
        check = 0;
    
        while(!check)
        {
          timeOut++;
          if(timeOut > ThR)
          {
            //Serial.println("Not receiving reply from 0");
            return;
          }
          check = wait_for_reply();
        }
        //Serial.println("Reply got");
        //Serial.println("Recieved reply at : "+replyReceived);
        send_ack(reply);
        //Serial.println("Sent ack");
        //unsigned long e2eDelay = replyReceived - queryStart;
        //Serial.println("End to end delay : "+e2eDelay);        
   }
}




void send_ack(unsigned char in)
{
  unsigned char out = in | 0b00100000;
  out = out & 0b11101111;
  Serial.write(out); 
}


unsigned char change_destination_id_to_next(unsigned char in)
{
  unsigned char dest = in & 0b11000000;
  dest = dest >> 6;
  dest = (dest+1)<<6;
  unsigned char other = in & 0b00111111;
  in = dest | other;
  in = in & 0b11011111;
  in = in | 0b00010000;
  return in; 
}

int wait_for_ack(unsigned long timer)
{
  unsigned long startTime = millis();
  unsigned long currentTime = startTime;
  while(currentTime - startTime < timer)
  {
    if(Serial.available())
    {
      unsigned char in = Serial.read();
      if((in & 0b00110000) == 0b00100000) return 1;
    }
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
    if(Serial.available())
    {
      unsigned char in = Serial.read();
      if((in & 0b00110000) == 0b00110000) {
        reply = in;
        return 1;
      }
    }
    currentTime = millis();
  }
  return 0;
}



