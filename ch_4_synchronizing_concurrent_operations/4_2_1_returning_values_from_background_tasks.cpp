//g++ -std=c++11 -pthread your_code.cpp -o your_program


// std::thread has no ways of returning a value
// under the future header we have std::async function template

// std::async returns a std::future object, which will eventually hold
// hold the return value, thread will block itself until the future is
// ready, to return the value after you call get()

#include<future>
#include<iostream>

#include<chrono>
#include<thread>

int find_answer(){
    // simulate a time consuming calculation
    std::this_thread::sleep_for(std::chrono::seconds(2) );
    return 42;
}

void do_other_stuff(){
    std::cout<< "Do other stuff while waiting for the answer...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout<< "Still working on other stuff...\n";
}

int main(){

    // launch "find_answer" asynchrnously

    // not specifying std::launch::async may defer execution until the
    // "the_answer" (future) is called
    // we force a seperate thread to be called 
    std::future<int> the_answer=std::async(std::launch::async, find_answer);
    // async can take mulitple parameters
    /*
    If the first argument is a pointer to a member function, 
    the second argument provides the object on which to apply 
    
    the member function (either directly, or via a pointer, 
    or wrapped in std::ref), and the remaining arguments 
    are passed as arguments to the member function. 
    Otherwise, the second and subsequent arguments are 
    passed as arguments to the function or callable object 
    specified as the first argument.
    */


    do_other_stuff();
    std::cout<< "Answer is: "<< the_answer.get()<< "\n";

}

