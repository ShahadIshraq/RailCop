byte currentChannel = 1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);
  pinMode(2, OUTPUT);
  //pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(2, HIGH);
  //digitalWrite(LED_BUILTIN, LOW);
 // delay(5000);

}

void change_current_channel()
{

  if(currentChannel == 1) currentChannel = 2;
  else currentChannel = 1;

}

void switch_channel(byte channel)
{
  //Serial.write('s');
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
  delay(100);
  digitalWrite(2, HIGH);
  while(Serial.available())
  {
    Serial.read();
  }
  //Serial.write('S');
}

void loop() {
  switch_channel(currentChannel);
  delay(100);
  char in = 'a';
  Serial.write(in);
  delay(1000);
  change_current_channel();
  switch_channel(currentChannel);
  delay(100);
  in = 'b';
  Serial.write(in);
  delay(1000);
  change_current_channel();
}

