byte currentChannel = 2;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
 // delay(5000);
 // switch_channel(currentChannel);
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
  //if(Serial.available()>0)
  {
  char in = 'a';
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
