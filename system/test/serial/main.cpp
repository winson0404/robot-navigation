#include <iostream>
#include <string>
#include <cstring>
#include <libserial/SerialPort.h>

int main()
{
    int move = 1;

    LibSerial::SerialPort serial_port;
    // send
    // std::cout << "================Receiving Control: " << std::endl;

    // try
    // {
    // Open the Serial Port at the desired hardware port.
    serial_port.Open("/dev/ttyACM0");
    // }
    // catch (const LibSerial::OpenFailed &)
    // {
    //     std::cerr << "The serial port did not open correctly." << std::endl;
    //     exit(EXIT_FAILURE);
    // }
    // Set the serial port settings (make sure to match with Arduino's settings)
    // Set the baud rate of the serial port.
    serial_port.SetBaudRate(LibSerial::BaudRate::BAUD_9600);

    // Set the number of data bits.
    // serial_port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_5);

    // Turn off hardware flow control.
    serial_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);

    // Disable parity.
    serial_port.SetParity(LibSerial::Parity::PARITY_NONE);

    // Set the number of stop bits.
    // serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

    bool send_task = false;
    int i = 0;  
    while (true)
    {
        // LibSerial::DataBuffer data_buffer(5);
        // serial_port.Read(data_buffer, 5);
        // std::cout << "Data: " << (int)data_buffer[0] << std::endl;
        // std::cout << "Data: " << (int)data_buffer[1] << std::endl;
        // std::cout << "Data: " << (int)data_buffer[2] << std::endl;
        // std::cout << "Data: " << (int)data_buffer[3] << std::endl;
        // std::cout << "Data: " << (int)data_buffer[4] << std::endl;

        // wait for 1 second

        char start_bit;
        if (!serial_port.IsDataAvailable())
        {
            if (!send_task)
            {
                char start_bit = 0;
                char task = 2; // SENSOR
                char packet_length = 0;
                std::cout << "Sending Task: " << (int)task << std::endl;
                std::cout << "Sending Packet Length: " << (int)packet_length << std::endl;
                serial_port.WriteByte(start_bit);
                serial_port.WriteByte(task);
                serial_port.WriteByte(packet_length);
                // continue;
                serial_port.DrainWriteBuffer();
                send_task = true;
                i=0;
            }
            else
            {
                std::cout<<"Waiting Reply "<< i++ <<std::endl;
                continue;
            }
        }
        else
        {
            send_task = false;
            std::cout<<"Receiving Reply " << std::endl;
            // serial_port.FlushIOBuffers();
            serial_port.ReadByte(start_bit);
            std::cout << "Start Bit: " << (int)start_bit << std::endl;
            if ((int)start_bit != 0)
            {
                std::cout << "Invalid start bit received: " << (int)start_bit << std::endl;
                continue;
            }
            // char tag;
            // serial_port.ReadByte(tag);
            char task;
            serial_port.ReadByte(task);
            char packet_length;
            serial_port.ReadByte(packet_length);
            // std::cout << "Tag: " << (int)tag << std::endl;
            std::cout << "Received Packet Length: " << (int)packet_length << std::endl;
            unsigned short *data;
            // LibSerial::DataBuffer data_buffer((int)data_length);
            std::string data_buffer;
            serial_port.Read(data_buffer, (int)packet_length);

            // convert data buffer to unsigned short
            //  data = (unsigned short)data_buffer[0] << 8 | (unsigned short)data_buffer[1];
            //  std::cout<< "Data: " << (unsigned short)data << std::endl;
            //  data = reinterpret_cast<unsigned short*>(data_buffer);
            // convert string to char *
            char *cdata = new char[data_buffer.size()];
            std::copy(data_buffer.begin(), data_buffer.end(), cdata);
            char sensor_size = *cdata;
            cdata++;
            std::cout << "Sensor Size: " << (int)sensor_size << std::endl;
            char* sensor_pointer = nullptr;
            memcpy(sensor_pointer, cdata, (int)sensor_size);
            unsigned short* sensor_data = reinterpret_cast<unsigned short*>(sensor_pointer);
            std::cout << "Sensor Data: " << *sensor_data << std::endl;
            // while(data_buffer.size() < (int)packet_length){
            //     char temp;
            //     serial_port.ReadByte(temp);
            //     data_buffer.push_back(temp);
            // }
            // data = reinterpret_cast<unsigned short *>(&data_buffer[0]);
            // std::cout << "Data: " << *data << std::endl;
            // std::cout << "Data 1:" << (int)data_buffer[0] << std::endl;
            // std::cout << "Data 2:" << (int)data_buffer[1] << std::endl;

            // char end_bit = 0;
            // serial_port.ReadByte(end_bit);
            // std::cout << "End Bit: " << (int)end_bit << std::endl;
            // serial_port.Read(reinterpret_cast<char*>(&data), (int)data_length);

            // float data;
            // char* dataPointer = reinterpret_cast<char*>(&data);
            // for (int i = 0; i < data_length; ++i) {
            //     serial_port.ReadByte(*dataPointer);
            //     std::cout<< "Data: " << (int)*dataPointer << std::endl;
            //     dataPointer++;
            // }

            // std::cout << "Data: " << data << std::endl;
        }
    }

    // char data[data_length];
    // std::string data;
    // LibSerial::DataBuffer data(data_length);
    // serial_port.Read(data, data_length);
    // float* newFloatValue=nullptr;
    // newFloatValue = reinterpret_cast<float*>(&data[0]);
    // // for (int i = 0; i < data_length; i++){
    // //     std::cout << "Data: " << (int)data[i] << std::endl;
    // // }
    // std::cout << "Data: " << *newFloatValue << std::endl;
    // int tag = (int)data[0];
    // int data_length = (int)data[1];

    // float* newFloatValue=nullptr;
    // char* data = data.c_str() + 2;
    // newFloatValue = reinterpret_cast<float*>(data.c_str() + 2);

    // Wait until the data has actually been transmitted.
    // serial_port.DrainWriteBuffer();
}