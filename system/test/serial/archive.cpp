#include <iostream>
#include <string>
#include <cstring>
#include <libserial/SerialPort.h>


void send_task(LibSerial::SerialPort &serial_port, char task, char packet_length)
{
    char start_bit = 0;
    std::cout << "Sending Task: " << (int)task << std::endl;
    std::cout << "Sending Packet Length: " << (int)packet_length << std::endl;
    serial_port.WriteByte(start_bit);
    serial_port.WriteByte(task);
    serial_port.WriteByte(packet_length);
    // serial_port.DrainWriteBuffer();
    serial_port.DrainWriteBuffer();
    // serial_port.FlushIOBuffers();
}

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

    serial_port.FlushIOBuffers();
    // Set the number of stop bits.
    // serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

    // bool send_task = false;
    char start_bit = 0;
    // send_task(serial_port, 2, 0);
    int i = 0;
    while (true)
    {
        if (serial_port.IsDataAvailable() && serial_port.GetNumberOfBytesAvailable() >= 1)
        {
            // std::string received_data;
            std::cout<<"Byte size: "<<serial_port.GetNumberOfBytesAvailable()<<std::endl;
            // serial_port.FlushIOBuffers();
            std::cout << "Receiving Reply " << std::endl;
            serial_port.ReadByte(start_bit);
            std::cout << "Start Bit: " << (int)start_bit << std::endl;
            serial_port.FlushOutputBuffer();
            break;
            if ((int)start_bit != 123)
            {
                std::cout << "Invalid start bit received: " << (int)start_bit << std::endl;
                // break;
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
            char *sensor_pointer = nullptr;
            memcpy(sensor_pointer, cdata, (int)sensor_size);
            std::cout<<"Test"<<std::endl;
            unsigned short *sensor_data = reinterpret_cast<unsigned short *>(sensor_pointer);
            std::cout << "Sensor Data: " << *sensor_data << std::endl;
            break;
        }
        else{
            std::cout<<"Waiting for data " << i++ <<std::endl;
            // serial_port.FlushIOBuffers();
            continue;
        }
    }
}