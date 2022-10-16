#include<iostream>
#include"some_functions.h"
#include<thread>


//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wunused-parameter"


int some_function(){
    std::cout<< "Hello from some_function\n";
    return 1;
}

int some_other_function()
{
    std::cout<<"Hello from some_other_function\n";
    return 2;
}

//void some_function_2(){
//    std::cout<< "Hello from some_function\n";
//}
// the code below outputs an error
//std::thread f1(){
//    void some_function_2();
//    return std::thread( some_function_2 );
//}

//void f(std)
