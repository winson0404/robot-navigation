#include <stdio.h>
#include <string.h>
// #include <vector>
#include <SoftwareSerial.h>
#include <MemoryUsage.h>

struct packet
{
    char checksum;
    char packet_length;
    char task;
    char num_data;
    char *data_length;
    unsigned short *data;
};

int numChars = 32;
char *receivedChars;
const char *target_string = "test";
const int TX_PIN = 12;
const int RX_PIN = 13;
const int TEST1 = 2;
const int TEST2 = 3;
const int TEST3 = 4;
const int ERROR1 = 5;
const int ERROR2 = 6;
const char startMarker = '<';
const char endMarker = '>';
char mode = 'r';
bool new_data = false;
packet received_packet;

SoftwareSerial ser(RX_PIN, TX_PIN);

char check_response(){
  char* response = (char *)malloc(numChars);
  bool _new_data = false;
  recvWithStartEndMarkers(response, _new_data);
  char status = response[0];
  free(response);
  if (_new_data){
    switch ((int)status) {
    case 0:
      return 0; //success
    default:
      return 1; //fail
    }
  }

  return 2; // no response
}

void send_response(char response){
  ser.write(startMarker);
  ser.write(response);
  ser.write(endMarker);
  // Serial.print("Resonse: ");
  // Serial.println(response);
}

void postprocess(char *data, packet *p)
{
    // protocol for data: <task|packet_length|data_length|data>
    // task: 1 byte
    // packet_length: 1 byte
    // data_length: 1 byte
    // data: (data_length) byte

    // packet p;
    // checksum : 0
    // packet_length : 1
    // task : 2
    // num_data : 3
    int counter = 0;
    p->packet_length = data[counter];
    counter++;
    p->task = data[counter];
    counter++;
    p->num_data = data[counter];
    counter++;
    // free(p->data_length);
    // free(p->data);
    // p->data_length = (char *)malloc((int)p->num_data);
    // p->data = (unsigned short *)malloc((int)p->num_data);
    for (int i = 0; i < p->num_data; i++)
    {
        p->data_length[i] = data[counter];
        counter++;
        // unsigned short data_value = 0;
        char *temp_data = (char *)malloc(p->data_length[i]);
        for (int j = 0; j < (int)p->data_length[i]; j++)
        {
            temp_data[j] = data[counter];
            // data_value = data_value << 8;
            // data_value = data_value | data[counter];
            counter++;
        }

        unsigned short *data_value = reinterpret_cast<unsigned short *>(temp_data);
        p->data[i] = *data_value;
        free(temp_data);
        // free(data_value);
    }
    p->checksum = data[counter];
}

packet structure_data(char task, char num_data, char *data_length, unsigned short *data)
{
    packet p;
    p.task = task;
    p.num_data = num_data;
    p.data_length = data_length;
    p.data = data;
    p.packet_length = 2; // checksum and packet_length
    p.packet_length += sizeof(task);
    p.packet_length += sizeof(num_data);
    for (int i = 0; i < num_data; i++)
    {
        p.packet_length += sizeof(data_length[i]);
        p.packet_length += (int)data_length[i];
    }
    p.checksum = 0;
    // p.checksum += p.packet_length;
    // p.checksum += p.task;
    // p.checksum += p.num_data;
    // for (int i = 0; i < num_data; i++)
    // {
    //     p.checksum += p.data_length[i];
    //     for (int j = 0; j < (int)data_length[i]; j++)
    //     {
    //         p.checksum += (char)data[i];
    //     }
    // }
    return p;
}

void clear_buffer(){
    while(ser.available() > 0){
        ser.read();
    }

}

void send(packet send_packet)
{
    ser.write(startMarker);
    ser.write(send_packet.packet_length);
    ser.write(send_packet.task);
    ser.write(send_packet.num_data);
    // for (int i = 0; i < send_packet.num_data; i++)
    // {
    //     ser.print(send_packet.data_length[i]);
    //     char *cdata = reinterpret_cast<char *>(&send_packet.data[i]);
    //     for(int j = 0; j < (int)send_packet.data_length[i];j++){
    //       ser.print(cdata[j]);
    //     }
        
    // }
    ser.write(send_packet.data_length[0]);
    char *cdata = reinterpret_cast<char *>(&send_packet.data[0]);
    ser.write(cdata[0]);
    ser.write(cdata[1]);
    ser.write(send_packet.data_length[1]);
    cdata = reinterpret_cast<char *>(&send_packet.data[1]);
    ser.write(cdata[0]);
    ser.write(cdata[1]);
    ser.write(send_packet.checksum); 
    ser.write(endMarker);

    while(true){
      char response = check_response();
      if ((int)response == 0){
        
        digitalWrite(TEST1, HIGH);
        delay(200);
        digitalWrite(TEST1, LOW);
        digitalWrite(ERROR1, LOW);
        break;
      }
      else if((int)response == 1){
        digitalWrite(TEST2, HIGH);
        delay(200);
        digitalWrite(TEST2, LOW);
        digitalWrite(ERROR1, LOW);
        break;

      }
      else{
        digitalWrite(ERROR1, HIGH);
      }
    }
}

void showNewData()
{
        // Serial.print("This just in ... ");
        bool correctness = false;

        if (received_packet.data[0] == 3152 && received_packet.data[1] == 2431 && (int)received_packet.task == 3){
          send_response(0);
          digitalWrite(TEST3, HIGH);
          delay(100);
        }
        else{
          send_response(1);
        }

        digitalWrite(TEST3, LOW);
        // Serial.print("<");
        // Serial.write(receivedChars, 5);
        // Serial.print(">");
}

void recvWithStartEndMarkers(char *data, bool &newData)
{
    static bool recvInProgress = false;
    static uint8_t ndx = 0;
    char rc;
    // memset(data, '\0', 32);

    // if (Serial.available() > 0) {
    // digitalWrite(ERROR2, HIGH);
    while (ser.available() > 0 && newData == false)
    {
        rc = ser.read();

        if (recvInProgress == true)
        {
            int data_count = 0;
            int packet_length = 0;
            char checksum = 0;
            if (rc != endMarker)
            {
                // digitalWrite(TEST1, HIGH);
                // delay(500);
                // digitalWrite(TEST1, LOW);
                // delay(500);
                // digitalWrite(ERROR1, HIGH);
                data[ndx] = rc;
                ndx++;
                if (ndx == 0)
                {
                    packet_length = data[ndx];
                }

                data_count++;
                if (ndx >= numChars)
                {
                  // Serial.println("In here");
                    ndx = numChars - 1;
                }

                if(ser.overflow()){
                  clear_buffer();
                  // digitalWrite(ERROR2, HIGH);
                  send_response(1);
                  break;
                }
            }
            else
            {
                // ser.print("In end ");
                // data[ndx] = '\0'; // terminate the string
                // digitalWrite(TEST1, HIGH);
                // delay(1000);
                recvInProgress = false;
                ndx = 0;
                // digitalWrite(ERROR2, LOW);
                newData = true;
                // Serial.println("End marker found");
                // Serial.println(ser.available());
                if (data_count != packet_length)
                {
                    // digitalWrite(ERROR1, HIGH);
                    // delay(100);
                    // digitalWrite(ERROR1, LOW);

                    clear_buffer();
                    newData = false;
                    // mode = 'w';
                    // Serial.println("here");
                    send_response(1);
                    break;
                }
            }
        }

        else if (rc == startMarker)
        {                    
            // Serial.println("Start marker found");
            recvInProgress = true;
        }
    }
    // if (newData == false)
      // ser.write("<0>");
      // return;
    Serial.println("End rec");
}

void setup()
{
    // Start the serial communication
    ser.begin(9600);
    Serial.begin(9600);
    pinMode(TEST1, OUTPUT);
    pinMode(TEST2, OUTPUT);
    pinMode(TEST3, OUTPUT);
    pinMode(ERROR1, OUTPUT);
    digitalWrite(TEST1, HIGH);
    digitalWrite(TEST2, HIGH);
    digitalWrite(TEST3, HIGH);
    digitalWrite(ERROR1, HIGH);
    digitalWrite(TEST1, LOW);
    digitalWrite(TEST2, LOW);
    digitalWrite(TEST3, LOW);
    digitalWrite(ERROR1, LOW);
    receivedChars = (char *)malloc(numChars * sizeof(char)); // Allocate memory for receivedChars
    received_packet.data_length = (char *)malloc(numChars * sizeof(char));
    received_packet.data = (unsigned short *)malloc(numChars * sizeof(unsigned short));
    FREERAM_PRINT
    // clear_buffer();
    // ser.print("<started>");
    // Serial.println("HI");
    // delay(10000);
}

void loop()
{
    // delay(1000);
    if (mode == 'r')
    {
        // digitalWrite(ERROR2, HIGH);
        recvWithStartEndMarkers(receivedChars, new_data);
        if (new_data == true)
        {
            postprocess(receivedChars, &received_packet);
            showNewData();
            new_data = false;
            mode = 'w';
        }
        // delay(1000);
        // digitalWrite(ERROR2, LOW);
        // delay(1000);
    }
    FREERAM_PRINT
    if (mode == 'w')
    {
        char task = 3;
        char num_data = 2;

        char *data_length = (char *)malloc(num_data * sizeof(char));
        unsigned short *data = (unsigned short *)malloc(num_data * sizeof(unsigned short));
        data_length[0] = 2;
        data[0] = 1234;
        data_length[1] = 2;
        data[1] = 5678;
        
        packet send_data = structure_data(task, num_data, data_length, data);
        send(send_data);
        mode = 'r';
        free(data_length);
        free(data);
    }

    // send((char)received_packet.task, (char)received_packet.data_length, (char)p.packet_length, p.data);
    // if(Serial.available())
    //   receive();
}