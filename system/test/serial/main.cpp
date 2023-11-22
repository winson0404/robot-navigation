#include <iostream>
#include <string>
#include <libserial/SerialPort.h>

int main()
{
    int move = 1;

    LibSerial::SerialPort serial_port;
    // send
    std::cout << "================Receiving Control: " << std::endl;

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
    serial_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);
    while(true){
        char start_bit;
        if (serial_port.IsDataAvailable() == false){
            continue;
        }
        serial_port.ReadByte(start_bit);
        std::cout << "Start Bit: " << (int)start_bit << std::endl;
        if (start_bit != 100){
            std::cout << "Invalid start bit received: " << (int)start_bit << std::endl;
            continue;
        }
        char tag;
        serial_port.ReadByte(tag);
        char data_length;
        serial_port.ReadByte(data_length);
        std::cout << "Tag: " << (int)tag << std::endl;
        std::cout << "Data Length: " << (int)data_length << std::endl;

        float data;
        char* dataPointer = reinterpret_cast<char*>(&data);
        for (int i = 0; i < data_length; ++i) {
            serial_port.ReadByte(*dataPointer);
            std::cout<< "Data: " << (int)*dataPointer << std::endl;
            dataPointer++;
        }

        std::cout << "Data: " << data << std::endl;


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
    serial_port.DrainWriteBuffer();
}