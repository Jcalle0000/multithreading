#include<iostream>
#include<thread>

void hello(){
    std::cout<< "Hello Concurrent world\n";
}

int main(){ // main thread thats able to make more threads, like in this instance "t"
    
    // how long does setting up thread take?
    std::thread t( hello );

    // another way to do this
    // std::thread t{ hello };  // didnt work 



    bool b;

    b= false;

    if(b){
        t.join();
        // this allows the the thread "t" to complete 
    }else{
        t.detach(); // we dont wait for "t" to finish before we finish main;
        // and this case we may not see hello output its data
    }


    // t.join(); // we wait fot the thread "t" to finish
}

// g++ helloCurrentWorld.cpp -o helloCurrentWorld.exe