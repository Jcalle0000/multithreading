#include<thread>
#include<iostream> // print, cout
#include<mutex> // to make the data mutually exlcusive
#include<condition_variable> // since c++11

// for debugging and time output
#include <iomanip>   // For std::put_time
#include <chrono>    // For time functions
#include <ctime>     // For std::localtime

// how could i figure out from which function this has been called?
// if all the work is async/threads
void getTime(std::string ref){

    std::cout<< "getTime-"<< ref<< "\n";
    auto now = std::chrono::system_clock::now();

    // Convert time_point to time_t to extract a calendar time (std::tm)
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert to local time (calendar time)
    std::tm* localTime = std::localtime(&currentTime);

    // Output the current time in the desired format
    std::cout<< ref << "-" << "Current time: " 
              << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") 
              << '\n';
}

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
        }
//   int n=10;


        void foo(){
        //   public void foo() {
            // std::cout<< "\nfoo-thread has been initalized\n";

            // getTime("foo");
            // std::cout<< "foo-thread finished initializing?\n";

            complete=true;
            // this should only run once
            // std::cout<< "foo::complete:initial: (" << complete<< ")\n\n";


            for (int i = 0; i < n; i++) {
                std::unique_lock<std::mutex> lk(mtx); //  create instance of lock guard
            //   print("foo");
                // std::cout<< "foo-for-i-start " << i<< "\n";
                // std::cout<< "foo\n";
                // printFoo(); // the issue was here
                // std::cout<< "foo-for-start-complete:" << complete<< "\n";

                while(!complete){
                // below is the original
                // while(complete){ // this is false so it will not be executed until its true
                    // complete=false; // moving to outside
                    // std::cout<< "foo::while::completed: " << complete << "\n";
                    // std::cout<< "foo will now wait\n";
                    cv.wait(lk) ; // so here we wait to recieve feedback from bar()?
                    // we wait until "complete" is set to true

                    // or were waiting for lk to be unlocked?
                    
                    // looks like this is not reached after wait?
                    std::cout<< "\nfoo::while::waiting\n";
                }

                printFoo();
                complete=false;
                // std::cout<< "foo-for-i-end " << i<< "\n";
                // std::cout<< "foo-for-end-complete:" << complete<< "\n";

                cv.notify_one();
            }
        }

        // public void bar() {
        void bar(){
            
            // std::cout<< "\nbar thread has been intialized\n";
            // getTime("bar");  
            // std::cout<< "bar thread has finished initializing?\n";
            // std::cout<< "bar::complete:initial (" << complete<< ")\n\n";


            for (int i = 0; i < n; i++) {
            //   print("bar");
                std::unique_lock<std::mutex> lk(mtx); // without this foo wiil finish running before?
                // std::cout<<"bar-for-i-start " << i << "\n";
                // printBar();
                // std::cout<< "\n";

                // while(!complete){
                while(complete){
                    // complete=true; // this was causing deadlock inn the wait 
                    // std::cout<< "bar::while::completed " << complete << "\n";
                    // std::cout<< "bar will now wait\n";
                    cv.wait(lk); // this was causing deadlock

                    std::cout<< "\nbar::while::waiting\n";
                }

                printBar();
                complete=true;
                // below is the original
                // complete=false; // we set this to true so that foo can run

                // std::cout<< "bar-for-i-end " << i<< "\n\n";

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
