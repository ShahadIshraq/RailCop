#include <EEPROM.h>
byte currentChannel = 2;
unsigned char ID = 1;
unsigned char reply;
unsigned long QT = 230;
unsigned long RAT = 230;
unsigned long RT = 500;
unsigned long offStart = 0;
unsigned long offTimer = 120000;



int ThQ = 5;
int ThR = 5;
int ThRA = 5;

int off = 0;


void transport_layer(unsigned char in);
void send_ack(unsigned char in);
unsigned char change_destination_id_to_next(unsigned char in);
int wait_for_ack(unsigned long timer);
int wait_for_reply();
unsigned char rd = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  delay(200);
  switch_channel(currentChannel);
  delay(1000);
  rd =  EEPROM.read(0);
  if(rd == 0 || rd == 255) EEPROM.write(0 , QT);
  else QT = rd;
  RAT = QT;
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
        Serial.print(QT);
      }
    }
    cur = millis();
  }



  
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
  if(Serial.available() && off == 0)
  {
    unsigned char in = Serial.read();
    transport_layer(in);
    if(off == 1 ) offStart = millis();
  }
  else {
    while(Serial.available()) Serial.read();
  }
  
  if(offStart != 0 && millis()-offStart >= offTimer) off = 0;
  
  if(offStart != 0 && millis()-offStart >= offTimer+12000) off = 1 ;
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
   unsigned char offCheck = in & 0b00000011;
   if(offCheck == 1) {
    off = 1;
   }
   unsigned char type = in & 0b11000000;
   type = type >> 6;
   if (type != ID) return;
   type = in & 0b00110000;
   type = type >> 4;
   if(type == 0)  //Query from train
   {

    send_ack(in);
    delay(80);
    change_current_channel();
    switch_channel(currentChannel);
    unsigned char out = change_destination_id_to_next(in);
    
    
    //Wait for ack on QT
    int timeOut = 0;
    int check = 0;
    while(!check)
    {
      Serial.write(out);
      //delay(100);
      timeOut++;
      if(timeOut > ThQ)
      {
        reply = in;
        reply = reply | 0b00110000; //marking as reply
        reply = reply & 0b11111011; //unsetting second reply bit
        reply = reply | 0b00001000; //setting first reply bit
        break;
      }
      check = wait_for_ack(QT);
    }


    //Wait for reply on RT
    timeOut = 0;
    check = 0;

    while(!check)
    {
      timeOut++;
      if(timeOut > ThR)
      {
        reply = in;
        reply = reply | 0b00110000; //marking as reply
        reply = reply & 0b11111011; //unsetting second reply bit
        reply = reply | 0b00001000; //setting first reply bit
        break;
      }
      check = wait_for_reply();
    }
    if(check)
    {
      send_ack(reply);
    }

    change_current_channel();
    switch_channel(currentChannel);

    
    //Wait for ack on RAT
    timeOut = 0;
    check = 0;
    while(!check)
    {
      Serial.write(reply);
      //delay(100);
      timeOut++;
      if(timeOut > ThRA)
      {
        return;
      }
      check = wait_for_ack(RAT);
    }
    return;

    
   }

   else if(type == 1) // Query from another node
   {
    unsigned char id = (in & 0b11000000)>>6;
    if(id != ID) return;
    send_ack(in);
    //delay(200);
    unsigned char rep = in | 0b00110100;
    
    //Wait for ack on RAT
    int timeOut = 0;
    int check = 0;
    while(!check)
    {
      Serial.write(rep);
      //delay(200);
      timeOut++;
      if(timeOut > ThRA)
      {
        return;
      }
      check = wait_for_ack(RAT);
    }
    return;
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



