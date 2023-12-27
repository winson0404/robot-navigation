#include <stdio.h>
#include <string.h>

int numChars = 32;
char *receivedChars;
const char *target_string = "test";
const int TEST1 = 2;
const int TEST2 = 4;
const int TEST3 = 7;
const char startMarker = '<';
const char endMarker = '>';
bool newData = false;


struct packet
{
    int task;
    int packet_length;
    int data_length;
    unsigned short data;
};

packet postprocess(char *data)
{
    // protocol for data: <task|packet_length|data_length|data>
    // task: 1 byte
    // packet_length: 1 byte
    // data_length: 1 byte
    // data: (data_length) byte
    packet p;
    int counter = 0;
    char task = data[counter];
    counter++;
    char packet_length = data[counter];
    counter++;
    p.task = (int)task;
    p.packet_length = (int)packet_length;
    while (packet_length > 0)
    {
        int 
        exit() = (int)data[counter];
        counter++;
        // unsigned short data_value = 0;
        char* temp_data = (char*) malloc((data_length));
        for (int i = 0; i < data_length; i++)
        {
            temp_data[i] = data[counter];
            // data_value = data_value << 8;
            // data_value = data_value | data[counter];
            counter++;
        }



        
        p.data_length = data_length;
        unsigned short *data_value = reinterpret_cast<unsigned short*>(temp_data);
        p.data = *data_value;
        free(temp_data);
        packet_length--;
    }


    // point back to head of Data
    // data = data - (int)packet_length;


    return p;
}

void send(char task, char data_length, char packet_length, unsigned short data)
{
    // Your data to be sent as bytes
    // byte dataBytes[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII
    // char packet[5];
    // char task = 2;
    // float data = 3.152f;
    // char data_length = 2;
    // char packet_length = 3;
    // unsigned short data = 3152;
    char *cdata = reinterpret_cast<char *>(&data);
    // char message[4];
    // sprintf(message, "%c%c%c%c", start_bit, task, data_length, packet_length);
    // Serial.write(message, 4);
    // sprintf(message, "")

    Serial.print(startMarker);
    Serial.write(&task, 1);
    Serial.write(&packet_length, 1);
    Serial.write(&data_length, 1);
    Serial.write(cdata, 2);
    Serial.print(endMarker);
    // delay(1000);
    Serial.flush();
    // Serial.flush();
}

// void receive()
// {
//     char *data;
//     char start_bit = -1;
//     char task = 0, packet_length = 0;
//     Serial.readBytes(data, 1);
//     if ((int)*data == 0)
//     { // start bit received
//         // do stuffs
//         //  Serial.println("Start bit received");
//         //  data++;
//         data = nullptr;
//         Serial.readBytes(data, 1);
//         task = *data;
//         // data++;
//         data = nullptr;
//         Serial.readBytes(data, 1);
//         packet_length = *data;
//         // data++;
//         data = nullptr;
//         Serial.readBytes(data, (int)packet_length);
//         // Serial.print("Task:");
//         // Serial.println((int) task);
//         // Serial.print("Packet Length:");
//         // Serial.println((int) packet_length);
//         Serial.flush();
//         send(task, char data_length, char packet_length, unsigned short data);
//     }
//     // Serial.println((int)*data);
// }

void showNewData()
{
    if (newData == true)
    {

        packet p = postprocess(receivedChars);
        // Serial.print("This just in ... ");
        
        if (p.task == 2){
            digitalWrite(TEST1, HIGH);
        }
        if (p.packet_length == 3){
            digitalWrite(TEST2, HIGH);
        }
        if (p.data == 3152){
            digitalWrite(TEST3, HIGH);
        }
        delay(2000);
        // // if (strcmp(receivedChars, target_string) == 0)
        // {
        //     // Serial.println("Matched");
        //     digitalWrite(TEST1, HIGH);
        //     // digitalWrite(TEST2, LOW);
        //     delay(1000);
        // }
        // else
        // {
        //     // Serial.println("Not Matched");
        //     // digitalWrite(TEST1, LOW);
        //     digitalWrite(TEST2, HIGH);
        //     delay(1000);
        // }

        digitalWrite(TEST1, LOW);
        digitalWrite(TEST2, LOW);
        digitalWrite(TEST3, LOW);
        send((char)p.task, (char)p.data_length, (char)p.packet_length, p.data);
        // Serial.print("<");
        // Serial.write(receivedChars, 5);
        // Serial.print(">");
        newData = false;
    }
}

void recvWithStartEndMarkers(char *data)
{
    static bool recvInProgress = false;
    static uint8_t ndx = 0;
    char rc;
    // memset(data, '\0', 32);

    // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false)
    {
        rc = Serial.read();

        if (recvInProgress == true)
        {
            int data_count = 0;
            if (rc != endMarker)
            {
                // digitalWrite(TEST1, HIGH);
                // delay(500);
                // digitalWrite(TEST1, LOW);
                // delay(500);
                data[ndx] = rc;
                ndx++;
                data_count++;
                if (ndx >= numChars)
                {
                    ndx = numChars - 1;
                }
            }
            else
            {
                // Serial.print("In end ");
                // data[ndx] = '\0'; // terminate the string
                // digitalWrite(TEST1, HIGH);
                // delay(1000);
                recvInProgress = false;
                ndx = 0;
                newData = true;
                // if (data_count != 5){

                //     digitalWrite(TEST1, HIGH);
                //     delay(1000);
                //     newData = false;
                // }
            }
        }

        else if (rc == startMarker)
        {
            // Serial.print("In start ");
            recvInProgress = true;
        }
    }
}

void setup()
{
    // Start the serial communication
    Serial.begin(9600);
    pinMode(TEST1, OUTPUT);
    pinMode(TEST2, OUTPUT);
    pinMode(TEST3, OUTPUT);
    digitalWrite(TEST1, HIGH);
    digitalWrite(TEST2, HIGH);
    digitalWrite(TEST3, HIGH);
    delay(100);
    digitalWrite(TEST1, LOW);
    digitalWrite(TEST2, LOW);
    digitalWrite(TEST3, LOW);
    receivedChars = (char *)malloc(numChars * sizeof(char)); // Allocate memory for receivedChars

    Serial.print("<started>");
}

void loop()
{
    // delay(1000);
    recvWithStartEndMarkers(receivedChars);
    showNewData();
    // if(Serial.available())
    //   receive();
}