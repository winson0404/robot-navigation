#include <structs.h>

namespace inputs{
    VisualOutput* visual_process(char* buffer){

        //get the first element of buffer
        int no_items = buffer[0];
        VisualOutput* output;
        output->prediction = buffer[1];
        // int length_index = 1;
        // for (int i = 0; i < no_items; i++){
        //     //get the next element
        //     int item_length = buffer[length_index];
        //     //do something with the item


        // }


        return output;

    }

    Sensor1Output* sensor1_process(char* buffer){
        Sensor1Output* output;
        output->x = buffer[0];
        output->y = buffer[1];
        output->z = buffer[2];
        return output;
    }

}