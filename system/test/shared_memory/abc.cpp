#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

struct VisualOutput
{
    int prediction;
};

struct Sensor1Output
{
    int x;
    int y;
    int z;
};

struct Sensor2Output
{
    int x;
    int y;
    int z;
};

struct InputState
{
    Sensor1Output *sensor1 = nullptr;
    VisualOutput *visual = nullptr;
};

const int SHARED_MEMORY_SIZE = sizeof(InputState); // Shared memory size in bytes

void *create_shared_memory(size_t size)
{
    // Our memory buffer will be readable and writable:
    int protection = PROT_READ | PROT_WRITE;

    // The buffer will be shared (meaning other processes can access it), but
    // anonymous (meaning third-party processes cannot obtain an address for it),
    // so only this process and its children will be able to use it:
    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    // The remaining parameters to `mmap()` are not important for this use case,
    // but the manpage for `mmap` explains their purpose.
    return mmap(NULL, size, protection, visibility, -1, 0);
}

int main()
{

    InputState* shared_memory = static_cast<InputState*>(create_shared_memory(SHARED_MEMORY_SIZE));

    pid_t pid1, pid2;

    // Fork the first subprocess
    pid1 = fork();
    if (pid1 < 0)
    {
        std::cerr << "Fork failed.\n";
        return 1;
    }
    else if (pid1 == 0)
    {
        int visual_value = 1;
        while (true)
        {
            // Modify the data in shared memory
            shared_memory->visual = new VisualOutput;
            if (shared_memory->visual != nullptr)
            {
                shared_memory->visual->prediction = visual_value++;
            }

            std::cout << "Child Process 1: Visual = " << shared_memory->visual->prediction << std::endl;
            if(shared_memory->sensor1 != nullptr)
                std::cout << "Child Process 1: Sensor1 = " << shared_memory->sensor1->x << ", " << shared_memory->sensor1->y << ", " << shared_memory->sensor1->z << std::endl;
            sleep(2);
        }
    }

    // Fork the second subprocess
    pid2 = fork();
    if (pid2 < 0)
    {
        std::cerr << "Fork failed.\n";
        return 1;
    }
    else if (pid2 == 0)
    { // Child process 2 (Sensor1 Process)
        int sensor1_value = 1;
        while (true)
        {
            // Modify the data in shared memory
            shared_memory->sensor1 = new Sensor1Output;
            if (shared_memory->sensor1 != nullptr)
            {
                shared_memory->sensor1->x = 1 + ++sensor1_value;
                shared_memory->sensor1->y = 2 + ++sensor1_value;
                shared_memory->sensor1->z = 3 + ++sensor1_value;
            }

            if (shared_memory->visual != nullptr)
                std::cout << "Child Process 2: Visual = " << shared_memory->visual->prediction << std::endl;
            std::cout << "Child Process 2: Sensor1 = " << shared_memory->sensor1->x << ", " << shared_memory->sensor1->y << ", " << shared_memory->sensor1->z << std::endl;
            sleep(2);
        }
    }

    // Parent process
    while (true)
    {
        // Read data from shared memory
        int value1;
        int value2;
        int value3;
        int value4;

        // check for null pointer
        if (shared_memory->visual != nullptr)
        {
            value1 = shared_memory->visual->prediction;
        }
        else
        {
            value1 = -1;
        }
        if (shared_memory->sensor1 != nullptr)
        {
            value2 = shared_memory->sensor1->x;
            value3 = shared_memory->sensor1->y;
            value4 = shared_memory->sensor1->z;
        }
        else
        {
            value2 = -1;
            value3 = -1;
            value4 = -1;
        }

        std::cout << "Parent Process: Data = " << value1 << ", " << value2;
        std::cout << ", " << value3 << ", " << value4 << std::endl;
        sleep(1);
    }


    return 0;
}
