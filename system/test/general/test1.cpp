#include <iostream>
#include <string>

int main(){
    const char* test = "test";
    int a = 1;
    a+=2;
    const int b = a;


    const char* test2 = test;

    std::cout << &test << std::endl;
    std::cout << &test2 << std::endl;


    return 0;

}