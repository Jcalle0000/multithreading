#include<thread>
#include<iostream>

int main(){

    unsigned int con_threads = std::thread::hardware_concurrency();
    std::cout<< "Number of concurrent threads: " << con_threads << "\n";

    // office computer outputs 12
}