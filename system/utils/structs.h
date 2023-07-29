namespace inputs{
    enum INPUT_TAG{
        VISUAL_TAG = 0,
        SENSOR1_TAG = 1,
    };

    struct VisualOutput{
        // int x;
        // int y;
        // int z;
        // int yaw;
        // int pitch;
        // int roll;
        int prediction;
    };

    struct Sensor1Output{
        int x;
        int y;
        int z;
    };

    struct InputState{
        VisualOutput* visual = nullptr;
        Sensor1Output* sensor1 = nullptr;
    };
}