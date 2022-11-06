#include "func_struct.h"
#include<iostream>

void sayHello(int &i){

    // for()
    std::cout<< "\nHello Concurrent world:" << i <<  "x" ;
}

void func::operator()(){
        for(int j=0;j<1000;j++){
        sayHello(i); // i is the parameter passed into the object
    }

}