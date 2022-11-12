// protecta list that can be accessed by multiple threads
// using std::mutex, along with std::lock_guard

// lists are double linked lists

#include<list> // ?
#include<mutex> // mutex and lock_guard
#include<algorithm>
#include<thread>

#include<iostream>

// g++ protectList.cpp -o protectList.exe 

std::list<int> some_list;
// How does "firstMutex" know to protect "some_list"
std::mutex firstMutex; // this is a global instance

// so when we use "firstMutex" in "add_to_list", "firstMutex" is locked and unable to be used in "list_contains" 

// we are locking some_list
void add_to_list( int new_value){

    std::lock_guard< std::mutex > guard(firstMutex); //  ideally mutex with be associated with some_list in some object
    some_list.push_back(new_value);
}

// we are lock "some_list"
bool list_contains(int value_to_find){
    std::lock_guard<std::mutex> guard(firstMutex);
     
    return std::find(
        some_list.begin(),
        some_list.end(),
        value_to_find
    )
    !=some_list.end();  // amake sure std::find does not return end of list, meaning it didnt find it
}

bool list_contains_2(int value_to_find, bool &b_){
    std::lock_guard<std::mutex> guard(firstMutex);
     
    // return std::find(
    auto x = std::find(        
        some_list.begin(),
        some_list.end(),
        value_to_find
    );

    

    if( x==some_list.end() ){
        std::cout<< "Not Found\n";
        b_ = false;
    }else{
        std::cout<< "Found\n";
        b_=true;
    }   

    return b_;

}

bool list_contains_3(int value_to_find, bool b){
    std::lock_guard<std::mutex> guard(firstMutex);
     
    // return std::find(
    auto x = std::find(        
        some_list.begin(),
        some_list.end(),
        value_to_find
    );

    

    if( x==some_list.end() ){
        std::cout<< "Not Found\n";
        b = false;
    }else{
        std::cout<< "Found\n";
        b=true;
    }   

    return b;

}

// creating this because idk how to return the bool value from a thread
// void call_list_contains(int value_to_find){
    
// }

int main(){
    
    int i=0;
    while(i<6){
        some_list.push_back(i);
        i++;
    }

    // some_list has {0,1,2,2,3,4,5}
    add_to_list( 6 );
    list_contains( 6 );

    // some_list has{0,1,2,2,3,4,5,6}

    // invalid use of void expression
    // std::thread threadTwo( list_contains(7) );

    bool b;

    // std::thread threadTwo( list_contains, 7 ); // how would you check the bool value of list_contains?

    std::thread threadTwo( list_contains_2, 7, std::ref(b) );
    bool c;
    // std::thread threadThree( list_contains_2, 7, c ); // this will not work - why?
    std::thread threadThree( list_contains_2, 7, std::ref(c) ); // this works
    

    // outputs not found

    // std::thread threadOne( add_to_list, 7 );

}