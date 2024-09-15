#include<iostream>
#include<thread>
#include<mutex>
#include<string>

using std::mutex;
using std::function;
using std::cout;
using std::thread;

class Foo {
    // solution
    private:
        // mutex is for mutual exclsion and allow for synchronization of data
        mutex task1;
        mutex task2;

    public:
        Foo() {
            // solution - at the constructor we lock the mutexes
            task1.lock();
            task2.lock();
        }

        void first(function<void()> printFirst) {
            
            // printFirst() outputs "first". Do not change or remove this line.
            printFirst();
            task1.unlock();
        }

        void second(function<void()> printSecond) {
            task1.lock();// we lock this in order to prevent access?
            // printSecond() outputs "second". Do not change or remove this line.
            printSecond();
            task2.unlock();
        }

        void third(function<void()> printThird) {
            task2.lock();
            // printThird() outputs "third". Do not change or remove this line.
            printThird();
        }
};

int main(){
    auto printFirst = []() { cout << "first\n"; };
    auto printSecond = []() { cout << "second\n"; };
    auto printThird = []() { cout << "third\n"; };

    Foo data;

    // Create threads for each method
    thread t1(&Foo::first, &data, printFirst);
    thread t2(&Foo::second, &data, printSecond);
    thread t3(&Foo::third, &data, printThird);


    // Join threads to ensure they complete before the main thread exits
    t1.join();
    t2.join();
    t3.join();
}