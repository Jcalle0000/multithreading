#include<thread>
#include<iostream> // print, cout
#include<mutex> // to make the data mutually exlcusive
#include<condition_variable> // since c++11

// for debugging and time output
#include <iomanip>   // For std::put_time
#include <chrono>    // For time functions
#include <ctime>     // For std::localtime

// how could i figure out from which function this has been called?
// // if all the work is async/threads
// void getTime(std::string ref){

//     std::cout<< "getTime-"<< ref<< "\n";
//     auto now = std::chrono::system_clock::now();

//     // Convert time_point to time_t to extract a calendar time (std::tm)
//     std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

//     // Convert to local time (calendar time)
//     std::tm* localTime = std::localtime(&currentTime);

//     // Output the current time in the desired format
//     std::cout<< ref << "-" << "Current time: " 
//               << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") 
//               << '\n';
// }

// it looks like printFoo and printBar
// needed to be function objects
// but why would this cause a different output

void printFoo(){
    std::cout<< "Foo";
}

void printBar(){
    std::cout<< "Bar\n";
}

// void print(std::string & ref){
//     std::cout<< ref;
// }

class FooBar {

    private:
        std::mutex mtx;
        std::condition_variable cv;
        bool complete; // this initialized to 0 by default
        int n;

    public:
        FooBar(int n){
            this->n=n;
            // complete=true;
        }
//   int n=10;


        void foo(){

            complete=true;
           

            for (int i = 0; i < n; i++) {
                std::unique_lock<std::mutex> lk(mtx); //  create instance of lock guard
       
                // printFoo();

                while(!complete){ // this is false so it will not be executed until its true
            
                    cv.wait(lk) ; // so here we wait to recieve feedback from bar()?
                  
                }
                
                printFoo();
                complete=false;
              
                cv.notify_one();
            }
        }

        // public void bar() {
        void bar(){
    
            for (int i = 0; i < n; i++) {
                std::unique_lock<std::mutex> lk(mtx); // without this foo wiil finish running before?
                printBar();

                while(complete){

                    cv.wait(lk); 

                }


                printBar();
                complete=true; 


                cv.notify_one();
            }
        }
};

int main(){
    FooBar fb1(10);
    // std::thread t1(fb1.foo() );
    // std::thread t2(fb1.bar() );
    std::thread t1(&FooBar::foo, &fb1);
    std::thread t2(&FooBar::bar, &fb1);

    t1.join();
    t2.join(); // what happens if we dont join?
}
