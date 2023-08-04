#pragma once
#include "../Socket/TcpSocket.h"

namespace movement{
    enum MOVEMENT
    {
        FORWARD = 0,
        BACKWARD = 1,
        LEFT = 2,
        RIGHT = 3
    };
}

namespace inputs
{
    enum INPUT_TAG
    {
        VISUAL_TAG = 0,
        SENSOR1_TAG = 1,
    };

    struct VisualOutput
    {
        // int x;
        // int y;
        // int z;
        // int yaw;
        // int pitch;
        // int roll;
        int prediction;
    };

    struct Sensor1Output
    {
        int x;
        int y;
        int z;
    };

    struct InputState
    {
        VisualOutput *visual = nullptr;
        Sensor1Output *sensor1 = nullptr;
    };

    VisualOutput *visual_process(char *buffer, int data_length);
    Sensor1Output *sensor1_process(char *buffer, int data_length);

}

namespace system_fnc
{
    extern movement::MOVEMENT previous_move;
    void handleClient(comms::RemoteClient client, comms::SocketServer &server, inputs::InputState *input_state);
    void listener(inputs::InputState *input_state);
    void status_checker(inputs::InputState *input_state);
    void control(movement::MOVEMENT move, char *SERIAL_PORT);
    movement::MOVEMENT decision_maker(
        inputs::VisualOutput visual,
        inputs::Sensor1Output sensor1);

}