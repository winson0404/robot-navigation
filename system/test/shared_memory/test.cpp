#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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

struct InputState
{
    Sensor1Output *sensor1 = nullptr;
    VisualOutput *visual = nullptr;
};

const int SHARED_MEMORY_SIZE = sizeof(InputState); // Shared memory size in bytes

int main()
{
    // Generate a unique key to access the shared memory segment
    key_t key = ftok("shared_memory_example", 65);

    // Create a shared memory segment
    int shmid = shmget(key, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        std::cerr << "Failed to create shared memory segment.\n";
        return 1;
    }

    // Attach the shared memory segment to the process' address space
    InputState *shared_memory = static_cast<InputState *>(shmat(shmid, nullptr, 0));
    if (shared_memory == reinterpret_cast<InputState *>(-1))
    {
        std::cerr << "Failed to attach shared memory segment.\n";
        return 1;
    }
    shared_memory->visual = new VisualOutput;
    shared_memory->sensor1 = new Sensor1Output;

    pid_t pid1, pid2;

    // Fork the first subprocess
    pid1 = fork();
    if (pid1 < 0)
    {
        std::cerr << "Fork failed.\n";
        return 1;
    }
    else if (pid1 == 0)
    { // Child process 1 ( Visual Process)
        while (true)
        {

            int shmid = shmget(key, SHARED_MEMORY_SIZE, 0666);
            if (shmid == -1)
            {
                std::cerr << "Failed to create shared memory segment.\n";
                return 1;
            }

            // Attach the shared memory segment to the process' address space
            InputState *_shared_memory = static_cast<InputState *>(shmat(shmid, nullptr, 0));
            if (_shared_memory == reinterpret_cast<InputState *>(-1))
            {
                std::cerr << "Failed to attach shared memory segment.\n";
                return 1;
            }
            // Modify the data in shared memory
            _shared_memory->visual->prediction = 1;
            // You can also modify the char array
            // strcpy(shared_memory->name, "NewName");

            std::cout << "Child Process 1: Data = " << _shared_memory->visual->prediction << std::endl;
            sleep(3);
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
    { // Child process 2 ( Sensor1 Process )
        while (true)
        {
            // Modify the data in shared memory
            int shmid = shmget(key, SHARED_MEMORY_SIZE, 0666);
            if (shmid == -1)
            {
                std::cerr << "Failed to create shared memory segment.\n";
                return 1;
            }

            // Attach the shared memory segment to the process' address space
            InputState *_shared_memory = static_cast<InputState *>(shmat(shmid, nullptr, 0));
            if (_shared_memory == reinterpret_cast<InputState *>(-1))
            {
                std::cerr << "Failed to attach shared memory segment.\n";
                return 1;
            }
            // Modify the data in shared memory
            _shared_memory->visual->prediction = 1;

            _shared_memory->sensor1->x = 1;
            _shared_memory->sensor1->y = 2;
            _shared_memory->sensor1->z = 3;

            std::cout << "Child Process 2: Data = " << _shared_memory->sensor1->x << ", " << _shared_memory->sensor1->y << ", " << shared_memory->sensor1->z << std::endl;
            sleep(2);
        }
    }

    // Parent process
    while (true)
    {
        int shmid = shmget(key, SHARED_MEMORY_SIZE, 0666);
        if (shmid == -1)
        {
            std::cerr << "Failed to create shared memory segment.\n";
            return 1;
        }

        // Attach the shared memory segment to the process' address space
        InputState *_shared_memory = static_cast<InputState *>(shmat(shmid, nullptr, 0));
        if (_shared_memory == reinterpret_cast<InputState *>(-1))
        {
            std::cerr << "Failed to attach shared memory segment.\n";
            return 1;
        }

        // Read data from shared memory
        int value1 = -2;
        int value2 = -2;
        int value3 = -2;
        int value4 = -2;

        std::cout << "Hello World" << std::endl;
        // check for null pointer

        // try
        // {
        if (_shared_memory->visual != nullptr)
        {
            value1 = _shared_memory->visual->prediction;
        }
        else
        {
            value1 = -1;
        }
        if (_shared_memory->sensor1 != nullptr)
        {
            value2 = _shared_memory->sensor1->x;
            value3 = _shared_memory->sensor1->y;
            value4 = _shared_memory->sensor1->z;
        }
        else
        {
            value2 = -1;
            value3 = -1;
            value4 = -1;
        }

        std::cout << "Parent Process: Data = " << value1 << ", " << value2;
        std::cout << "," << value3 << ", " << value4 << std::endl;
        // }
        // catch (const std::exception &e)
        // {
        //     std::cerr << e.what() << '\n';
        // }

        sleep(1);
    }

    // Detach and remove the shared memory segment
    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, nullptr);

    return 0;
}
