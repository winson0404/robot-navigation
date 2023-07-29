#include "utils/comms.h"

namespace inputs{
    template <typename T>
    class Base_Input{
        private:
            bool is_complete;
        public:
            virtual T receive_handler(char* data) = 0;
    };

    class Visual_Input : Base_Input<VisualOutput>{
        public:
            VisualOutput receive_handler(char* data);
    };

}