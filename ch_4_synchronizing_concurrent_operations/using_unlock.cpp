#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

// Shared resources
std::queue<int> dataQueue;
std::mutex mtx;
std::condition_variable cv;
bool done = false;  // Flag to indicate when production is done

// Producer function
void producer() {
    for (int i = 0; i < 5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Simulate work
        std::lock_guard<std::mutex> lock(mtx); // we lock twice?
        dataQueue.push(i);
        std::cout << "Produced: " << i << std::endl;
        cv.notify_one(); // Notify the consumer that data is available
    }

    // Set done flag and notify the consumer that production is complete
    {
        std::lock_guard<std::mutex> lock(mtx); // second lock?
        done = true;
    }
    cv.notify_one();
}

// Consumer function
void consumer() {

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait for data to be available or for the done flag to be set
        cv.wait(lock, [] { // we wait until there is data
            return !dataQueue.empty() || done; 
        });

        // Check if the done flag is set and the queue is empty
        if (done && dataQueue.empty()) {
            break; // exit while loop
        }

        // Consume data
        int value = dataQueue.front();
        dataQueue.pop();
        lock.unlock(); // Unlock the mutex to allow producer to continue
        std::cout << "Consumed: " << value << std::endl;
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
