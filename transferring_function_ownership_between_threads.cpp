#include<iostream>
#include<thread>

void some_function(){
    std::cout<< "Hello from some_function\n";
}

void some_other_function()
{
    std::cout<<"Hello from some_other_function\n";
}

void f1(){

    std::thread t1( some_function );

    std::thread t2 = std::move( t1 ); // t1 is now empty

    t1 = std::thread( some_other_function ); // this will output

    std::thread t3 = std::move(t2); // some_function is now part of t3

    // the line below will terminate the program
    /*
        In this case t1 already had an associated thread which was running with
        "some_other_function"  

        You must explicitly wait for a thread to complete or deatach it before destruction
        and the same applies to assignment, you cannnot simply drop the thread managing "some_function"
    */

    t1 = std::move(t3); // will this terminate the program?
}

int main(){
    f1();
    std::cout<< "Program did not terminate\n";
}
