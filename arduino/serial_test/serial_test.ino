#include <stdio.h>
#include <string.h>


const int TEST1 = 2;
const int TEST2 = 4;

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  pinMode(TEST1, OUTPUT);
  pinMode(TEST2, OUTPUT);
  digitalWrite(TEST1, LOW);
  digitalWrite(TEST2, LOW);
}
void send(){
// Your data to be sent as bytes
  // byte dataBytes[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII
  // char packet[5];
  char start_bit = 123;
  char task = 2;
  // float data = 3.152f;
  char data_length = 3;
  char packet_length = 4;
  unsigned short data = 3152;
  char* cdata = reinterpret_cast<char*>(&data);

  char message[4];
  sprintf(message, "%c%c%c%c", start_bit, task, data_length, packet_length);
  Serial.write(message, 4);
  // sprintf(message, "")

  // Serial.write(start_bit);
  // Serial.write(&task);
  // Serial.write(&packet_length);
  // Serial.write(&data_length);
  // Serial.write(reinterpret_cast<char*>(&data), data_length);
  // delay(1000);
  Serial.flush();
  // Serial.flush();

}

void receive(){
  char* data;
  char start_bit = -1;
  char task= 0, packet_length=0;
  Serial.readBytes(data, 1);
  if ((int)*data == 0){ //start bit received
    //do stuffs
    // Serial.println("Start bit received");
    // data++;
    data = nullptr;
    Serial.readBytes(data, 1);
    task = *data;
    // data++;
    data = nullptr;
    Serial.readBytes(data, 1);
    packet_length = *data;
    // data++;
    data = nullptr;
    Serial.readBytes(data, (int) packet_length);
    // Serial.print("Task:");
    // Serial.println((int) task);
    // Serial.print("Packet Length:");
    // Serial.println((int) packet_length);
    Serial.flush();
    send();
  }
  // Serial.println((int)*data);
}
void loop() {
  // delay(1000);
  Serial.flush();
  if(!Serial.availableForWrite()){
    send();

  }
  // if(Serial.available())
  //   receive();
}