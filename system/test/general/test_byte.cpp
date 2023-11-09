#include <iostream>
// #include <cstdint>

int main() {
    float floatValue = 3.14159f; // The float value you want to convert to bytes

    // Create a pointer to the float
    float* floatPointer = &floatValue;

    // Create a pointer to a char (byte)
    char* bytePointer = reinterpret_cast<char*>(floatPointer);

    // Determine the number of bytes in a float
    short byteCount = sizeof(float);

    // Output the bytes
    std::cout << "Count: " << byteCount <<"Float as bytes: ";
    for (short i = 0; i < byteCount; ++i) {
        std::cout << static_cast<int>(*bytePointer) << " ";
        ++bytePointer;
    }
    std::cout << std::endl;

    // Reading the bytes back into a float
    bytePointer = reinterpret_cast<char*>(floatPointer); // Reset the bytePointer

    // Create a new float and read the bytes back into it
    float* newFloatValue=nullptr;
    // char* newFloatPointer = reinterpret_cast<char*>(&newFloatValue);
    newFloatValue = reinterpret_cast<float*>(bytePointer);
    // for (size_t i = 0; i < byteCount; ++i) {
        // *newFloatPointer = *bytePointer;
        // ++newFloatPointer;
        // ++bytePointer;
    // }

    std::cout << "Recovered float: " << (int)bytePointer[4] << std::endl;

    return 0;
}