// std::condition_variable
// std::condition_variable_any

#include<mutex>
#include<queue>
#include<string>

struct data_chunk{
    std::string a;
    std::string b;
};

// global variables to be accessed b/w threads?
std::mutex mut;
std::queue<data_chunk> data_queue; // used to pass data b/w 2 threads
std::condition_variable data_cond;

// lock thread and push data into queue
void data_preparation_thread(){
    while(more_data_to_prepare() ){
        data_chunk const data=prepare_data();
        
        // does lock_guard only work on mutex?
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data); // we p
        data_cond.notify_one(); // notifies waiting thread, if there's one?

    }
}

void data_processing_thread(){
    while(true){
        std::unique_lock<std::mutex> lk(mut); 
        // whats the difference b/w unique_lock and lock_guard?
        data_cond.wait(
            lk, []{return !data_queue.empty() ; }
        );

        data_chunk data=data_queue.front();
        data_queue.pop();
        lk.unlock();
        process(data);
        if(is_last_chunk(data)
            break;
    }
}