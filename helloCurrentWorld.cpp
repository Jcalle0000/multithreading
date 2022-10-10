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
    
    // perhaps compile in c++11?
    // std::thread t{ (  hello()  ) };
    // std::thread t{ hello() }; // this causes an error in regards to the brackets

    bool b;
    b= true; // if true then main will wait for t to be completed and will see its output
    // if false then main will most likely complete before t completes

    if(b){
        t.join();
        // this allows the the thread "t" to complete 
    }else{
        t.detach(); // we dont wait for "t" to finish before we finish main;
        // and this case we may not see hello output its data
    }

}

// g++ helloCurrentWorld.cpp -o helloCurrentWorld.exe