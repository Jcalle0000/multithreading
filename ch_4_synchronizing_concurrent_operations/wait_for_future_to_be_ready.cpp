

// waiting up to 35 milliseconds for a future to be ready

#include<chrono>
#include<future>
#include<thread>
#include<iostream>

int some_task(){

    // simulate work
    std::this_thread::sleep_for(
        // if this is less than 35, then it will run
        std::chrono::milliseconds(40)
    ); 

    return 42;
}

void do_something_with(int result){
    std::cout<< "Result recieved: " << result << "\n";
}

int main(){

    // launch "some_task" through async
    std::future<int> f=std::async(
        std::launch::async,
        some_task
    );

    // check if the task finishes with 35 milliseconds
    if(     f.wait_for(std::chrono::milliseconds(35))
                ==
            std::future_status::ready ){

        // "f" has the task
        do_something_with(f.get() );
    }else{
        std::cout<< "Task did not complete in time!\n";
    }

    return 0;
}