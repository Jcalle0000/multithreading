#include<queue>
#include<mutex>
#include<condition_variable>
#include<string>

struct data_chunk{
    std::string a;
    std::string b;
};

template<typename T>

class threadsafe_queue{
    private:
        mmutable std::mutex mtx; // why do we make this mutable?
        std::queue<T> q;
        std::condition_variable cv;


    public:


        // default constructor
        threadsafe_queue(){}

        // is this a copy constructor?
        threadsafe_queue(threadsafe_queue ts_q const& other){
            std::lock_guard<std::mutex> lk(other.mtx);  // we are transferring mutex ownership?
            q=other.q;
        }

        void push(T new_value){
            std::lock_guard<std::mutex> lk(mtx); // lock the mutex to ensure
            // only 1 thread can modify the queue at a time
            q.push(new_value);
            cv.notify_one(); // notifies threads that maybe waiting
            // ONLY 1 thread will be notified
        }

        void wait_and_pop(T& value){ // why do we want this by reference to ensure?
            
            std::unique_lock<std::mutex> lk(mtx);
            // what is "this" for?
            cv.wait(lk,[this]{
                return! q.empty(); // we wait until the queue is not empty
            });

            value=q.front(); // this grabbed by reference?
            q.pop();
            
        }

        // why do we need a shared_ptr?
        std::shared_ptr<T> wait_and_pop(){
            std::unique_lock<std::mutex> lk(mtx);// how we know if we need unique?
            // b/c of the wait?

            cv.wait(lk, [this]{
                return!q.empty();
            });
        }
};

threadsafe_queue<data_chunk> ts_q;

