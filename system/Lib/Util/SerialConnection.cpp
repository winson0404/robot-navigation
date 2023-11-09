#include "Utils.h"
#include <iostream> 

namespace system_fnc{
    // comms::SerialSettings settings = comms::SerialSettings();
    //     settings.serial_port_name = SERIAL_PORT;
    //     comms::SerialComm serial_comm(settings);
    void serial_listener(comms::SerialComm serial_comm, inputs::InputState *input_state)
    {

        while (true){
            std::string data;

            std::cout << "Waiting for data..." << std::endl;
            serial_comm.Receive(data);

            int tag = (int)data[0];
            int data_length = (int)data[1];

            if (tag == inputs::VISUAL_TAG){
                input_state->visual = inputs::visual_process((char *)data.c_str() + 2, data_length);
            }
            else if (tag == inputs::SENSOR1_TAG){
                input_state->sensor1 = inputs::sensor1_process((char *)data.c_str() + 2, data_length);
            }
            else{
                std::cerr << "Invalid tag received: " << tag << std::endl;
                exit(1);
            }
            

            // after receiving all data
        }
        
    

    }

}