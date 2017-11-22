unsigned char in[5];
//unsigned int in
void setup() {
  Serial.begin(1200);
}

void loop() {
  if (Serial.available()) { //Read from HC-12 and send to serial monitor
    Serial.readBytes(in,5);
//    while(Serial.available()<1);
//    in[1] = Serial.read();
//    while(Serial.available()<1);
//    in[2] = Serial.read();
//    Serial.write(in[0]);
//    Serial.write(in[1]);
    Serial.write("01234");
  }

}
