#include <iostream>
#include<unistd.h>   
#include <sys/socket.h>
#include <thread>
#include <mutex>
#include <utils/utils.h>







int main(){
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        std::cerr << "Error: Fork failed." << std::endl;
        return 1;

    }
    if(pid == 0){
        decision_maker();
    }
    else{
        control();
    }

    return 0;
}
