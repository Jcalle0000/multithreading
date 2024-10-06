// usinng condition_variable to communicate b/w 2 threads
// using notify_one

// producer thread will produce an item
// then it will notify the consumer thread than item is available

// consumer will wait for notification and then consume?

#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<iostream>
#include<chrono> // for sleep_for?

// ideally all of this would be encapsulated
std::queue<int> q; // this will also be shared b/w producer and consumer
std::mutex mtx; // this is going to be shared b/w producer and cosumer?
// mtx is used to protect q and provide mutual exclusion
std::condition_variable cv; // this is being shared

const int maxBufferSize=5; // essentially limit to not producing more than 5


// producer will produce items in a loop
// however before producing it will lock the mutex, and check if the buffer is full
    // if it is full it waits on the condition variable
    // essentially we are trying to limit over, producing?

// after producing an item, it pushes onto q, 
    // and notifies one waiting consumer

// essentially if consumer takes a while to consume, than consumer will dictate
// the time of the program


void producer(){
    for(int i=0; i<10; ++i){ // ++i is more efficient than i++ - why though?

        // this is not necessary since its used for simulation?
        std::this_thread::sleep_for(std::chrono::milliseconds(100) ); // pre
        // when producer sleep_for is 500, we do not see producer waits, with consumer of sleep_for 800

        // std::lock_guard<std::mutex> lk(mtx); // mtx locking q at this point?
        std::unique_lock<std::mutex> lk(mtx);

        // we are limiting the amount of items being produced
        // if the size is "overflowing" than we go into a waiting state
        // and assk the consumer to eat?
        while(q.size()>=maxBufferSize){ // when sleep_for is 500, we do not see the wait
            std::cout<< "Producer waits, since queue is full\n";
            cv.wait(lk); 
            // wait unlocks the mutex, and puts the thread in a blocked/waiting state
            
        }

        q.push(i); // we add into the queue
        std::cout<< "Produced: "<< i<< "\n";
        cv.notify_one(); // what is this notifying? 
        // the thread waiting ?
    }
}

void consumer(){

    for(int i=0;i<10;++i){

        std::this_thread::sleep_for(std::chrono::milliseconds(500) ); // simulate work
        // std::lock_guard<std::mutex> lk(mtx);
        std::unique_lock<std::mutex> lk(mtx);

        // connsumer has to wait until there is an item to consume
        // we check to see if q is empty, and wait unti the condition variable
        while(q.empty() ){
            std::cout<< "consumer has to wait, since q is empty\n\n";
            cv.wait(lk); // what does this do?
            // so are we just waiting here?
        }

        // consume
        int item=q.front();
        q.pop();
        std::cout<< "consumed: "<< item<<"\n\n";
        cv.notify_one(); // here we notify the producer?
    }
}

int main(){
    std::thread prod(producer);
    std::thread cons(consumer);

    prod.join();
    cons.join();

    return 0;
}
