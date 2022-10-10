#include<thread>
#include<iostream>
#include<chrono>

void sayHello(int &i){

    // for()
    std::cout<< "\nHello Concurrent world:" << i <<  "x" ;
}

// we will be calling the struct object "func" as if it were a function because of the double ()() operators

struct func{
    
    int &i; // this is the property and _i is the parameter it takes in
    // default constructor, this is usually implicity created, but now we are creating it

    func(int & i_ ):i(i_) {} // i hold the value thats passed in "i_"
    
    // you can overload the () operator to call your object as if it was a function
    // class A {
    //     public:
    //         void operator()(int x, int y) {
    //             // Do something
    //         }
    // };

    // A x;
    // x(5, 3); // at this point operator () gets called

    void operator()() { // 2 ()
        for(int j=0;j<1000;j++){
            sayHello(i) ; // i is constant and output 1000x
        }
    }
};

void oops(){ // this void function will create a new thread

    int some_local_state=4;

    // baseline

    // auto start1 = std::chrono::high_resolution_clock::now();
    // for(int j=0;j<1000;++j){
    //     sayHello(some_local_state);
    // }
    //     sayHello(some_local_state);
    // auto stop1 = std::chrono::high_resolution_clock::now();

    // auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

    func my_func(some_local_state);
    // Object above acts as a function

    std::cout<< "\n\n";

    auto start = std::chrono::high_resolution_clock::now();
        std::thread first_thread(my_func); // thread recieves object's address

    //first_thread.detach(); // dosent wait for thread to finish
        first_thread.join(); // allows for the thread to complete
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout<<"\n\nFinished, duration: "<< duration.count() << " microseconds" << "\n";

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