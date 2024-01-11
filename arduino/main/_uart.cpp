#include "_uart.h"

namespace serial
{

    void clear_buffer(SoftwareSerial &ser)
    {
        while (ser.available() > 0)
        {
            ser.read();
        }
    }

    void send_with_start_end_markers(SoftwareSerial &ser, comms::packet p, uint8_t startMarker, uint8_t endMarker)
    {
        // protocol for data: <task|packet_length|data_length|data>
        // task: 1 byte
        // packet_length: 1 byte
        // data_length: 1 byte
        // data: (data_length) byte

        // packet p;
        // checksum : 0
        // packet_length : 1
        // data_length : 1
        // data : (data_length) byte

        // send start marker
        ser.write(startMarker);

        // send packet length
        ser.write(p.packet_length);

        // send task
        ser.write(p.task);

        // semd num_data
        ser.write(p.num_data);

        for (int i = 0; i < p.num_data; i++)
        {
            // send data length
            ser.write(p.data_length[i]);

            // send data
            uint8_t *cdata = reinterpret_cast<uint8_t *>(&p.data[i]);
            for (int j = 0; j < (int)p.data_length[i]; j++)
            {
                ser.write(cdata[j]);
            }
        }

        // send checksum
        ser.write(p.checksum);

        // send end marker
        ser.write(endMarker);


    }

    void recv_with_start_end_markers(SoftwareSerial &ser, uint8_t data_buffer[], bool &newData, uint8_t startMarker, uint8_t endMarker)
    {
        static bool recvInProgress = false;
        static uint8_t ndx = 0;
        uint8_t rc;

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
                    data_buffer[ndx] = rc;
                    ndx++;
                    if (ndx == 0)
                    {
                        packet_length = data_buffer[ndx];
                    }

                    data_count++;
                    if (ndx >= constants::RECEIVE_BUFFER_SIZE)
                    {
                        // Serial.println("In here");
                        ndx = constants::RECEIVE_BUFFER_SIZE - 1;
                    }

                    if (ser.overflow())
                    {
                        clear_buffer(ser);
                        serial::send_acknowledge(ser, constants::ACKNOWLEDGE_FAIL, startMarker, endMarker);
                        break;
                    }
                }
                else // if reached end marker
                {
                    recvInProgress = false;
                    ndx = 0;
                    newData = true;
                    if (data_count != packet_length)
                    {

                        clear_buffer(ser);
                        newData = false;
                        serial::send_acknowledge(ser, constants::ACKNOWLEDGE_FAIL, startMarker, endMarker);
                        break;
                    }

                    serial::send_acknowledge(ser, constants::ACKNOWLEDGE_SUCCESS, startMarker, endMarker);
                }
            }

            else if (rc == startMarker)
            {
                recvInProgress = true;
            }
        }
    }

    void send_acknowledge(SoftwareSerial &ser, uint8_t task, uint8_t start_marker, uint8_t end_marker)
    {
        ser.write(start_marker);
        ser.write(task);
        ser.write(end_marker);
    }

    bool receive_acknowledge(SoftwareSerial &ser, uint8_t task, uint8_t start_marker, uint8_t end_marker)
    {
        // uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * constants::RECEIVE_BUFFER_SIZE);
        uint8_t data_buffer[constants::RECEIVE_BUFFER_SIZE];
        bool new_data_available = false;
        unsigned long StartTime = millis();
        unsigned short time_out = 1000;
        while (true)
        {
            serial::recv_with_start_end_markers(ser, data_buffer, new_data_available, start_marker, end_marker);
            // set time out for 1 seconds to receive acknowledge
            if (new_data_available)
            {
                if (data_buffer[0] == constants::ACKNOWLEDGE_SUCCESS)
                {
                    // free(data);
                    return true;
                }
                else if (data_buffer[0] == constants::ACKNOWLEDGE_FAIL)
                {
                    // free(data);
                    return false;
                }
            }
            else if (millis() - StartTime > time_out)
            {
                // free(data);
                return false;
            }
        }
    }
}