#include <iostream>
#include <libserial/SerialPort.h>

#include "Utils.h"

namespace system_fnc
{
    void status_checker(inputs::InputState *input_state)
    {
        std::cout<< "Status checker" << std::endl;
        if (input_state == nullptr)
        {

            std::cerr << "Input state is null!" << std::endl;
            exit(1);
        }

        // previous_move = movement::LEFT;

        while (true)
        {
            if (input_state->visual == nullptr)
                continue;
            if (input_state->sensor1 == nullptr)
                continue;

            std::cout<< "============================== New Input Detected, Starting To Make Decision" << std::endl;
            movement::MOVEMENT move = decision_maker(
                *input_state->visual,
                *input_state->sensor1);

            //TODO: send move to arduino
            char* SERIAL_PORT = "/dev/ttyACM0";
            system_fnc::control(move, SERIAL_PORT);

            delete input_state->visual;
            delete input_state->sensor1;
            input_state->visual = nullptr;
            input_state->sensor1 = nullptr;
        }
    }

    movement::MOVEMENT decision_maker(
        inputs::VisualOutput visual,
        inputs::Sensor1Output sensor1)
    {
        movement::MOVEMENT output, previous_move = movement::LEFT;

        if (visual.prediction == 0) // no_objects
        {
            output = movement::FORWARD; 
        }
        else if (visual.prediction == 1)
        {
            previous_move = movement::LEFT; // left
            output = movement::LEFT;
        }
        else if (visual.prediction == 2)
        {
            previous_move = movement::RIGHT; // middle
            output = movement::RIGHT;
        }
        else if (visual.prediction == 3) // right 
            output = movement::LEFT;    

        else if (visual.prediction == 4) // all
            output = movement::RIGHT;

        else if (visual.prediction == 5) // left_mid
            output = movement::RIGHT;
            
        else if (visual.prediction == 6) // right_mid
            output = movement::LEFT;
        else
        {
            std::cerr << "Invalid prediction: " << visual.prediction << std::endl;
            exit(1);
        }

        std::cout << "==================Decision: " << output << std::endl;
        return output;
    }

    void control(movement::MOVEMENT move, char *SERIAL_PORT)
    {
        LibSerial::SerialPort serial_port;
        // send
        std::cout << "================Sending Control: " << move << std::endl;

        try
        {
            // Open the Serial Port at the desired hardware port.
            serial_port.Open(SERIAL_PORT);
        }
        catch (const LibSerial::OpenFailed &)
        {
            std::cerr << "The serial port did not open correctly." << std::endl;
            exit(EXIT_FAILURE);
        }
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

        serial_port.WriteByte((char)move);

        // Wait until the data has actually been transmitted.
        serial_port.DrainWriteBuffer() ;
    }

}