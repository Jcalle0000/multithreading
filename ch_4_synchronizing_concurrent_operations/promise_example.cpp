

// example for promise and future

#include<future>
#include<exception>
#include<iostream>

std::promise<double> some_promise;

double calculate_value(){
    throw std::runtime_error("calculation error");
    return 42;
}

// function that will be run in thread
void worker(){
    try{
        some_promise.set_value( calculate_value() );
    }catch(...){
        std::cout<< "inside catch worker\n";

        // you can store an exception if needed
        some_promise.set_exception(std::current_exception() );
        // what kind of error is set here?

        // you could also copy an exception
        // why would you do this though?
        // looks like this is cleaner and allows the compiler
        // to optimize code
        /*
            some_promise.set_exception(
                std::copy_exception(
                    std::logic_error("foo")
                )
            );
        */
    }
}

int main(){

    // what is happening here?

    // by creating a future
    // you make a promise to provide a value or exception
    // if the compiler didnt store anything in the future
    // waiting threads could potentially wait forever
    std::future<double> result=some_promise.get_future();

    // are futures also used to pass data from one thread to another?
    // accessing future from multiple threads will result in data races/ undefined behavior
    // after calling get, only one thread can access the data, after the first call there's
    // no value to retrive

    // thread will set a value but also a exception?
    std::thread t(worker);

    try{
        double value= result.get();
        std::cout<< "value: " << value << "\n";
    }catch( const std::exception &e ){

        // we will have caught an excpetion because a run time error is thrown
        std::cout<< "exception caught: "<< e.what() << "\n";
        // we actually will see here "calculation error"
    }

    t.join();
}