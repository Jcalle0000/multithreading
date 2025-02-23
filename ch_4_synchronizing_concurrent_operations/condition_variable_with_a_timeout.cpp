#include<thread>
#include<condition_variable>
#include<mutex>


std::condition_variable cv;
bool done;
std::mutex m;


bool wait_loop(){
    auto const timeout= std::chrono::steady_clock::now()
                                    +
                        std::chrono::milliseconds(500);

    while(!done){
        if( cv.wait_until(lk, timeout) ==std::cv_status::timeout)
            break;
    }

    return done;
}

// functions that accept timeouts

/*
    1. std::timed_mutex
    2. std::recursive_timed_mutex
        A. they support try_lock_for
        B. try_lock_until
*/