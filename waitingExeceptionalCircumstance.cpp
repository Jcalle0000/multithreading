// same func object from waitingForThreadToFinsh.cpp

#include<thread>
#include<iostream>

void sayHello(int &i){

    // for()
    std::cout<< "\nHello Concurrent world:" << i <<  "x" ;
}

struct func{
    int &i;

    // default constructor
    func(int &i):i(i_){} // _i is the parameter

    void operator()(){
        for(int j=0;j<1000;j++){
            sayHello(i); // i is the parameter passed into the object
        }
    }
};

// the function below does handle all situations so its best to go with RAII method
void f(){

    int some_local_state=4;

    func my_func(some_local_state); // object acts as a function

    std::thread first_thread( my_func );
    /*
    try{
        // do_something_in_current_thread(); // like what?
    }
    catch(){
        t.join() // wait for the thread to complete // function exists by an exception
        throw
    }
    t.join() // function exits normally
    */

}

class thread_guard{
    std::thread &t; // why is everything a reference?
    public:
        explicit thread_guard( std::thread& t_ ):t(t_){} // disallows any implcit conversions
        
        ~thread_guard(){ // deconstructor?
            if( t.joinable() ){
                t.join();
            }
        }

        // copying or assigning such an object would dangerous, as it may then outlive the scope
        // of the thread it was joining 
        // marking this as delete will generate a compilation error
        thread_guard( thread_guard const& )=delete; // copy constructor
        thread_guard & operator=( thread_guard const& ) = delete; // copy assignment operator
}

/* 
when f2 is completed or at the end of the function
the local objects are destroyed in reverse order of construction
Therefore when "guard" is destoryed it will make sure to wait for the thread to be competed 

Even if do_something_in_current_thread throws an exception the deconstructor will still follow through
*/
void f2(){
    int some_local_state = 4;
    func my_func(some_local_state); // my_func acts as a function
    std::thread first_thread(my_func);
    thread_guard guard (first_thread);
    /* do_something_in_current_thread() */

}