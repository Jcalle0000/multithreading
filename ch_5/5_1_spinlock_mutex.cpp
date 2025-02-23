#include<atomic>
#include<iostream>
#include<vector>
#include<thread>

class spinlock_mutex{
    std::atomic_flag flag;

    public:

        // did not work
        // spinlock_mutex(): 
        //                     flag( ATOMIC_FLAG_INIT )
        //                     {}

        // worked
        // spinlock_mutex(){
        //     flag.clear( std::memory_order_relaxed ); // initialize to false
        // }

        spinlock_mutex()=default;
        
        void lock(){

            // if the lock is free (not set)
            // the thread aquires it

            // if its already taken, the thread spins in a busy-wait loop
            // repeatedly checking the lock until its released by another thread
            // this called spinning
            while(flag.test_and_set( std::memory_order_acquire ) );
        }

        void unlock(){
            flag.clear(std::memory_order_release);
        }
};

int shared_data=0;

spinlock_mutex lock;

void increment_shared_data(int thread_id){
    
    lock.lock();

    std::cout<< "Thread: " << thread_id << " will increment shared_data\n";

    ++shared_data;

    // release
    lock.unlock();
}

int main(){

    const int num_threads=10;
    std::vector<std::thread> thread_vector;

    for(int i=0;i<num_threads;i++){
        thread_vector.push_back( std::thread( increment_shared_data, i ) );
    }

    for(auto& t: thread_vector){
        t.join();
    }

    std::cout << "Final value of shared_data: " << shared_data << std::endl;

}