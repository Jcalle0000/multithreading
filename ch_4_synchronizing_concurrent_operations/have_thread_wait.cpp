#include<mutex>
#include<chrono>
#include<thread>

bool flag;
std::mutex m;

void wait_for_flag(){
    std::unique_lock<std::mutex> lk(m);

    while(!flag){
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100 ) );
        // we sleep the thread so that another thread can access flag?
        lk.lock(); // relock mutex
    }
}