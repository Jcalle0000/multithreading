#include<thread>
#include<iostream>
#include<chrono>

void sayHello(int &i){

    // for()
    std::cout<< "\nHello Concurrent world:" << i <<  "x" ;
}

struct func{
    int &i;
    func(int & i_ ):i(i_) {} // i hold the value thats passed in "i_"
    
    void operator()() { // 2 ()
        for(int j=0;j<1000;j++){
            sayHello(i) ; // i vs j
        }
    }
};

void oops(){ // this void function creates a new thread

    int some_local_state=4;

    // baseline

    // auto start1 = std::chrono::high_resolution_clock::now();
    // for(int j=0;j<1000;++j){
    //     sayHello(some_local_state);
    // }
    //     sayHello(some_local_state);
    // auto stop1 = std::chrono::high_resolution_clock::now();

    // auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);



    func my_func(some_local_state); // we pass this function to do work on first_thread

    auto start = std::chrono::high_resolution_clock::now();
        std::thread first_thread(my_func); // thread recieves my_func address

    first_thread.detach(); // dosent wait for thread to finish
        // first_thread.join(); // allows for the thread to complete
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout<<"\n\nFinished, duration: "<< duration.count() << "\n";

    // std::cout<<"\n\nFinished, duration: "<< duration.count() << " baseline:"<< duration1.count() << "\n";

    //2881 vs  (baseline) 2465 microseconds, why is it faster in the mean thread?

    // perhaps we should add chrono info in the operator?

}

int main(){

    int x=5;
    // doSomething(x); // testing

    oops();
    
}

 // g++ -std=c++11 waitingForThreadToFinish.cpp -o waitingForThreadToFinish.exe