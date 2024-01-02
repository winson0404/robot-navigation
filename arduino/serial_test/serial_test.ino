#include <stdio.h>
#include <string.h>
// #include <vector>

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
const int TEST1 = 2;
const int TEST2 = 4;
const int TEST3 = 7;
const int ERROR1 = 8;
const char startMarker = '<';
const char endMarker = '>';
char mode = 'r';
bool newData = false;
packet received_packet;

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
    int counter = 2;
    p->packet_length = data[1];
    p->task = data[counter];
    counter++;
    p->num_data = data[counter];
    counter++;
    free(p->data_length);
    free(p->data);
    p->data_length = (char *)malloc((int)p->num_data);
    p->data = (unsigned short *)malloc((int)p->num_data);
    for (int i = 0; i < p->num_data; i++)
    {
        p->data_length[i] = data[counter];
        counter++;
        // unsigned short data_value = 0;
        char *temp_data = (char *)malloc((int)p->data_length[i]);
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
    }
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

void send(packet send_packet)
{
    char *cdata = reinterpret_cast<char *>(&send_packet);
    Serial.print(startMarker);
    // Serial.write(cdata, send_packet.packet_length);
    Serial.write(&send_packet.checksum, 1);
    Serial.write(&send_packet.packet_length, 1);
    Serial.write(&send_packet.task, 1);
    Serial.write(&send_packet.num_data, 1);
    for (int i = 0; i < send_packet.num_data; i++)
    {
        Serial.write(&send_packet.data_length[i], 1);
        char *cdata = reinterpret_cast<char *>(&send_packet.data[i]);
        Serial.write(cdata, (int)send_packet.data_length[i]);
        
    }
    Serial.print(endMarker);
    Serial.flush();
    mode = 'r';
}
// {
//     // Your data to be sent as bytes
//     // byte dataBytes[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII
//     // char packet[5];
//     // char task = 2;
//     // float data = 3.152f;
//     // char data_length = 2;
//     // char packet_length = 3;
//     // unsigned short data = 3152;
//     char *cdata = reinterpret_cast<char *>(&data);
//     // char message[4];
//     // sprintf(message, "%c%c%c%c", start_bit, task, data_length, packet_length);
//     // Serial.write(message, 4);
//     // sprintf(message, "")

//     Serial.print(startMarker);
//     Serial.write(&task, 1);
//     Serial.write(&packet_length, 1);
//     Serial.write(&data_length, 1);
//     Serial.write(cdata, 2);
//     Serial.print(endMarker);
//     // delay(1000);
//     Serial.flush();
//     // Serial.flush();
// }

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

        // Serial.print("This just in ... ");
        if ((int)received_packet.task == 2)
        {
            digitalWrite(TEST1, HIGH);
        }
        if ((int)received_packet.packet_length == 10)
        {
            digitalWrite(TEST2, HIGH);
        }
        if (received_packet.data[1] == 2431)
        {
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
        // Serial.print("<");
        // Serial.write(receivedChars, 5);
        // Serial.print(">");
        newData = false;
        mode = 'w';
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
            int packet_length = 0;
            char checksum;
            if (rc != endMarker)
            {
                // digitalWrite(TEST1, HIGH);
                // delay(500);
                // digitalWrite(TEST1, LOW);
                // delay(500);
                data[ndx] = rc;
                ndx++;

                if (ndx == 0)
                {
                    checksum = data[ndx];
                }
                else if (ndx == 1)
                {
                    packet_length = data[ndx];
                }

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
                if (data_count != packet_length)
                {

                    digitalWrite(ERROR1, HIGH);
                    delay(1000);
                    digitalWrite(ERROR1, LOW);
                    newData = false;
                }
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
    pinMode(ERROR1, OUTPUT);
    digitalWrite(TEST1, HIGH);
    digitalWrite(TEST2, HIGH);
    digitalWrite(TEST3, HIGH);
    digitalWrite(ERROR1, HIGH);
    delay(100);
    digitalWrite(TEST1, LOW);
    digitalWrite(TEST2, LOW);
    digitalWrite(TEST3, LOW);
    digitalWrite(ERROR1, LOW);
    receivedChars = (char *)malloc(numChars * sizeof(char)); // Allocate memory for receivedChars
    received_packet.data_length = (char *)malloc(16 * sizeof(char));
    received_packet.data = (unsigned short *)malloc(16 * sizeof(unsigned short));
    Serial.print("<started>");
}

void loop()
{
    // delay(1000);
    if (mode == 'r')
    {
        recvWithStartEndMarkers(receivedChars);
        if (newData == true)
        {
            postprocess(receivedChars, &received_packet);
        }
        showNewData();
    }

    else if (mode == 'w')
    {
        char task = 2;
        char num_data = 2;

        char *data_length = (char *)malloc(num_data * sizeof(char));
        unsigned short *data = (unsigned short *)malloc(num_data * sizeof(unsigned short));
        data_length[0] = 2;
        data[0] = 1234;
        data_length[1] = 2;
        data[1] = 5678;
        
        packet send_data = structure_data(task, num_data, data_length, data);
        send(send_data);
        free(data_length);
        free(data);
    }

    // send((char)received_packet.task, (char)received_packet.data_length, (char)p.packet_length, p.data);
    // if(Serial.available())
    //   receive();
}