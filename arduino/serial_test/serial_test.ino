void setup() {
  // Start the serial communication
  Serial.begin(9600);
}

void loop() {
  // Your data to be sent as bytes
  // byte dataBytes[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII
  // char packet[5];
  char tag = 1;
  char start_bit = 0;
  char end_bit = 1;
  // float data = 3.152f;
  char data_length = sizeof(unsigned short);
  unsigned short data = 3152;
  // float *floatPointer = &data;
  // char* dataPointer = reinterpret_cast<char *>(floatPointer);

  // char *recv_data;
  // Serial.readBytes(recv_data, 1);
  // Serial.write(recv_data, 1);

  // Send the data over the serial port
  Serial.write(&start_bit, sizeof(char));
  // Serial.print("start bit: ");
  // Serial.println((int)start_bit);
  // Serial.write(&tag, sizeof(char));
  Serial.write(&data_length, sizeof(char));
  // Serial.print("data length: ");
  // Serial.println((int)data_length);
  // char * test = reintepret_cast<char*> data;
  // char* data_char = ;

  // Serial.print("data [0]:");
  // Serial.println((int)data_char);
  // Serial.print("data [1]:");
  // Serial.println((int)(data_char+1));
  // unsigned short* temp = reinterpret_cast<unsigned short*>(data_char);
  // Serial.print("original:");
  // Serial.println(*temp);

  Serial.write(reinterpret_cast<char*>(&data), data_length);
  // Serial.print("data: ");
  // Serial.println(data);
  Serial.write(&end_bit, sizeof(char));
  // for(short i = 0; i < data_length; ++i){
  //   char test= 'a';
  //   Serial.write(&test, sizeof(char));
  // }

  // Wait for a short duration before sending the next data
  // delay(500); // Adjust the delay as needed
}