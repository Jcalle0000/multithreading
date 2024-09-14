#include<string>
#include<mutex>
#include<iostream>

class some_data{
    int a;
    std::string b;
    public:
        void do_something(){};
        int getValue(){
            return a;
        }
};

class data_wrapper{ // what are wrappers usually for?
    private:
        some_data data; // we are protectig this from multiple thread access
        std::mutex m;   // this will limit data from being accessed by multiple threads?
    
    public:
        // template<typename T> // typename can be called anything
        template<typename Function>// Function is a place holder for {callable_object, function pointer, lambda function, functor}
        // esentially we can now pass it any kind of object
        void process_data(Function func){
            std::lock_guard<std::mutex> lock(m); //1(m); //(m); //1(m); // so we are locking are mutex m?
            // limitinng access to "some_data" object, to ensure thread safety 
            // we have access to "data" b/c we are in the data_wrapper
            func(data);
        }
};

// now we are creating more functios
void print_int(some_data& data){
    std::cout<< "data value: "<< data.getValue() << "\n";
}

data_wrapper x;


void my_example(){
    x.process_data( print_int );
}

int main(){
    my_example(); // we use the wrapper we 
}


some_data* unprocted_pointer;

void degenerate_function(some_data & protected_data){
    unprocted_pointer=&protected_data;
}



void foo(){
    // data_wrapper.process_data( degenerate_function )
    x.process_data( degenerate_function ); // i didnt know we could pass functions
    unprocted_pointer->do_something();
}
