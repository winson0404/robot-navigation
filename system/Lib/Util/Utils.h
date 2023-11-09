#pragma once
#include "../Socket/TcpSocket.h"
#include "../SerialComm/SerialComm.h"
#include <mutex>

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
        int prediction;
    };

    struct Sensor1Output
    {
        float distance;
    };

    struct InputState
    {
        InputState(): visual(nullptr), sensor1(nullptr) {}
        VisualOutput *visual;
        Sensor1Output *sensor1;
    };

    VisualOutput *visual_process(char *buffer, int data_length);
    Sensor1Output *sensor1_process(char *buffer, int data_length);
}

namespace system_fnc
{
    extern movement::MOVEMENT previous_move;
    void handleClient(comms::RemoteClient client, comms::SocketServer &server, inputs::InputState *input_state);
    void tcp_listener(inputs::InputState *input_state);
    void serial_listener(comms::SerialSettings settings, inputs::InputState *input_state);
    void status_checker(inputs::InputState *input_state);
    void control(movement::MOVEMENT move, char *SERIAL_PORT);
    movement::MOVEMENT decision_maker(
        inputs::VisualOutput visual,
        inputs::Sensor1Output sensor1);

}