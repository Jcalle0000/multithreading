#include <iostream>
#include <thread>
#include <mutex>

std::mutex myMutex;
int sharedCounter = 0;

void incrementCounter(int id) { // thread_id
    for (int i = 0; i < 5; ++i) {
        // Lock the mutex using std::lock_guard
        std::lock_guard<std::mutex> guard(myMutex);

        // Critical section
        ++sharedCounter;
        std::cout << "Thread " << id << " incremented counter to " << sharedCounter << std::endl;

        // Mutex is automatically unlocked when guard goes out of scope
    }
}

int main() {
    std::thread t1(incrementCounter, 1); // thread_id
    std::thread t2(incrementCounter, 2); // thread_id

    t1.join();
    t2.join();

    std::cout << "Final counter value: " << sharedCounter << std::endl;
    return 0;
}

/*

Thread 1 incremented counter to 1
Thread 1 incremented counter to 2
Thread 1 incremented counter to 3
Thread 1 incremented counter to 4
Thread 1 incremented counter to 5
Thread 2 incremented counter to 6
Thread 2 incremented counter to 7
Thread 2 incremented counter to 8
Thread 2 incremented counter to 9
Thread 2 incremented counter to 10


*/