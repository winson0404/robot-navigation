#include <iostream>
#include <string>

void test_fnc(std::string &data, int data_length){
    // char buffer[1024] = "Hello from test_fnc";
    char *buffer = (char*)malloc(data_length);
    buffer = "Hello from test_fnc";

    data = buffer;
}


int main(){

    std::string data;

    test_fnc(data, 1024);

    std::cout << data << std::endl;
}