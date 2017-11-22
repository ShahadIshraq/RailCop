byte currentChannel = 2;
unsigned char ID = 0;
unsigned char reply;
unsigned long QT = 50;
unsigned long RAT = 50;
unsigned long RT = 50;
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
 // delay(5000);
 // switch_channel(currentChannel);
}

void change_current_channel()
{
  if(currentChannel == 1) currentChannel = 2;
  else currentChannel = 1;
}

void switch_channel(byte channel)
{
  digitalWrite(2, LOW);
  delay(200);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0)
  {
    char in = Serial.read();
    Serial.write(in);
    delay(1000);
   /* if(currentChannel == 1)
    {
      switch_channel(2);
      currentChannel = 2;
    }
    else if(currentChannel == 2)
    {
      switch_channel(1);
      currentChannel = 1;
    }*/
    //in++;
   //Serial.write(in); 
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
   unsigned char type = in & 0b00110000;
   type = type >> 4;
   if(type == 0)  //Query from train
   {
    send_ack(in);
    change_current_channel();
    switch_channel(currentChannel);
    unsigned char out = change_destination_id_to_next(in);
    Serial.write(out);
    
    //Wait for ack on QT
    int timeOut = -1;
    int check = 0;
    while(!check)
    {
      timeOut++;
      if(timeOut > ThQ)
      {
        //Report Problem
      }
      check = wait_for_ack(QT);
    }


    //Wait for reply on RT
    timeOut = -1;
    check = 0;
    while(!check)
    {
      timeOut++;
      if(timeOut > ThR)
      {
        //Report Problem
      }
      check = wait_for_reply();
    }

    send_ack(reply);

    change_current_channel();
    switch_channel(currentChannel);

    Serial.write(reply);

    //Wait for ack on RAT
    timeOut = -1;
    check = 0;
    while(!check)
    {
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
    unsigned char rep = in | 0b11000000;
    Serial.write(rep);
    //Wait for ack on RAT
    int timeOut = -1;
    int check = 0;
    while(!check)
    {
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
  in = in | 0b00100000;
  in = in & 0b11101111;
  Serial.write(in); 
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



