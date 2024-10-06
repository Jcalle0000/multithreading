#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>

// it looks like printFoo and printBar
// needed to be function objects
// but why would this cause a different output

void printFoo();
void printBar();


class FooBar {
private:
    int n;
    std::mutex mtx;
    std::condition_variable cv;
    bool fooTurn;

public:
    FooBar(int n) {
        this->n = n;
        fooTurn = true;  // Start with foo
    }

    void foo(std::function<void()> printFoo) {
        for (int i = 0; i < n; i++) {
            std::unique_lock<std::mutex> lock(mtx);
            
            // Wait while it's not foo's turn
            while (!fooTurn) {
                cv.wait(lock);  // Wait for bar to signal
            }

            // printFoo() outputs "foo". Do not change or remove this line.
            printFoo();
            
            fooTurn = false;  // Now it's bar's turn
            cv.notify_one();  // Notify one waiting thread (bar)
        }
    }

    void bar(std::function<void()> printBar) {
        for (int i = 0; i < n; i++) {
            std::unique_lock<std::mutex> lock(mtx);
            
            // Wait while it's not bar's turn
            while (fooTurn) {
                cv.wait(lock);  // Wait for foo to signal
            }

            // printBar() outputs "bar". Do not change or remove this line.
            printBar();
            
            fooTurn = true;  // Now it's foo's turn
            cv.notify_one();  // Notify one waiting thread (foo)
        }
    }
};


void printFoo() {
    std::cout << "foo";
}

void printBar() {
    std::cout << "bar\n";
}

int main(){
    FooBar foobar(2);  // n = 2

    // does printFoo have to be a function object
    // for this to work?
    std::thread t1(&FooBar::foo, &foobar, printFoo);
    std::thread t2(&FooBar::bar, &foobar, printBar);
    
    t1.join();
    t2.join();
}

