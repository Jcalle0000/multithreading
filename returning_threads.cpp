#include "some_functions.h"

#include<iostream>
#include<thread>

// g++ some_functions.cpp returning_threads.cpp -o returning_threads.exe


void some_function_2(){
    std::cout<< "Hello from some_function\n";
}

std::thread f1(){
    void some_function_2();
    std::thread first_thread( some_function_2 );
    return first_thread;
//    return std::thread( some_function_2 );
}




int main(){
    
    int a = some_function();
    int b = some_other_function();
    f1();
}
