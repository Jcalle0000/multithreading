#include<deque>
#include<mutex>
#include<future>
#include<thread>
#include<utility>
#include<iostream>
#include<chrono>

std::mutex m;
bool gui_shutdown=false;

bool gui_shutdown_message_recieved(){
    return gui_shutdown;
}

// we have a deque of packaged_tasks/futures
std::deque<std::packaged_task<void()> > tasks;

bool gui_shutdown_message_recieved();

void get_and_process_gui_message(){
    std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
    std::cout<< "Processing gui message\n";
}

void gui_thread(){

    std::cout<< "Inside the gui thread\n\n";

    while(!gui_shutdown_message_recieved() ){  // we repeatedly poll for gui messages to handle

        get_and_process_gui_message(); // examples can be user clicks and for tasks on the task queue
        std::packaged_task<void()> task;

        // brackets allow us to create a local scope for "lk"        
        {
            std::lock_guard<std::mutex> lk(m);
            if( tasks.empty()){
                std::cout<< "\ntasks are empty\n";
                continue; // if there are no tasks, loop again inside
                // the loop
            }

            // otherwise, extract the task from the queue
            task=std::move(tasks.front() );
            tasks.pop_front();
        }
        // m is released as soon as the scope for lk ends

        task(); // execute task outside the lock
    }
}



// function is used to post tasks to the gui thread
// function is designed to allow you to add tasks to the task queue

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f){

    // f will be lambda functions in the example below
    // packaged_task encapsulated the callable "f"
    std::packaged_task<void() > task(f); // post a task on the queue
    // a new task is created from the suplied function "f"

    // we tie the packaged_task to a future
    std::future<void> res=task.get_future();
    // this can be used to get the result
    // or know when the task is complete

    {
        std::lock_guard<std::mutex> lk(m);
    // task is then put on the list
        tasks.push_back(std::move(task) );
    }
    return res; // we return the future to the caller?
}

// std::packaged_task<void()>
// this can wrap a function or other callable objects that
// take no parameters and returns void
// if anything else is returned it will discard the value

int main(){

    // we create a thread and pass in the function gui_thread?
    std::thread gui_bg_thread(gui_thread);

    // we are actually passing a lambda function
    // the lambda function represents the task

    auto f1=post_task_for_gui_thread([]{
        std::cout<< "Task 1 executed\n";
    });

    auto f2=post_task_for_gui_thread([]{
        std::cout<< "Task 2 executed\n";
    });

    // wait for tasks to complete
    f1.get();
    f2.get();

    // signal shutdown
    gui_shutdown=true;

    // wait for the gui thread to finish
    gui_bg_thread.join();

    std::cout<< "Gui thread terminated\n";
    return 0;
}