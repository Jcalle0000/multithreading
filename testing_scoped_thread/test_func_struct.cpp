// g++ func_struct.cpp test_func_struct.cpp -o test_func_struct.exe

#include"func_struct.h"
#include<thread>

int main(){

    int some_local_state=5;
    func my_func(some_local_state);

    std::thread first_thread(my_func);
    first_thread.join();
}