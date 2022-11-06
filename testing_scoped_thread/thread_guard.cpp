#include "func_struct.h"
#include<thread>

// an attemp to use std::logic_error::logic_error
#include<exception>
    #include<stdexcept>

#include<iostream>

class scoped_thread
{
    std::thread t;
    public:
        explicit scoped_thread(std::thread t_): // why is this made explicit?
            t(std::move(t_)) // move constructor
            {
                if(!t.joinable()){
                    throw std::logic_error("s"); // what is std::logic_error?
                    std::string my_error = "No Thread\n";
                    std::cout<< my_error;
                }

            }
        ~scoped_thread() // destructor
        {
            t.join();
        }
        
        scoped_thread(scoped_thread const&)=delete; // copy constructor
        scoped_thread& operator=(scoped_thread const&)=delete; // copy operator
};



struct func; // defined in "func_struct.h"
// func calls sayHello 1000x to the number you pass by copy (some_local_state)


// g++ thread_gaurd.cpp func_struct.cpp -o thread_guard.cpp

void f();

int main(){

    f();

}

void f() {

    // was this, what was causing the dangling reference previously?
    int some_local_state=1;

    func my_function(some_local_state);

    std::thread first_thread;
    
    // first_thread( func my_function(some_local_state) );
    // std::thread first_thread( my_function );
    // libc++abi: terminating
    // zsh: abort      ./thread_guard.exe

    // first_thread.join(); // allows us to finish work before exiting out of f?

    // scoped_thread t( 
    //     (
    //         // first_thread // wont work with this
    //         // std::thread
    //         (
    //             func(some_local_state)
    //         )
    //     ) 
    // ); //  this does not want to run - why?

    scoped_thread t(
        (
            std::thread
            (
                func(some_local_state)
            )
        )
    );
    
    // do_something_in_current_thread();

}
