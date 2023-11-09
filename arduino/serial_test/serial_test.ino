void setup() {
  // Start the serial communication
  Serial.begin(9600);
}

void loop() {
  // Your data to be sent as bytes
  // byte dataBytes[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII
  char tag = 1;
  char start_bit = 100;
  float data = 3.152f;
  float *floatPointer = &data;
  char* dataPointer = reinterpret_cast<char *>(floatPointer);
  char data_length = sizeof(float);

  char *recv_data;
  Serial.readBytes(recv_data, 1);
  Serial.write(recv_data, 1);

  // Send the data over the serial port
  Serial.write(&start_bit, sizeof(char));
  Serial.write(&tag, sizeof(char));
  Serial.write(&data_length, sizeof(char));
  for(short i = 0; i < data_length; ++i){
    char test= 'a';
    Serial.write(&test, sizeof(char));
  }

  // Wait for a short duration before sending the next data
  delay(1000); // Adjust the delay as needed
}