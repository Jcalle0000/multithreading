// protecta list that can be accessed by multiple threads
// using std::mutex, along with std::lock_guard

// lists are double linked lists

#include<list> // ?
#include<mutex> // mutex and lock_guard
#include<algorithm>
#include<thread>

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

int main(){
    
    int i=0;
    while(i<6){
        some_list.push_back(i);
        i++;
    }

    // some_list has {0,1,2,2,3,4,5}
    add_to_list( 6 );
    list_contains( 2 );

    // some_list has{0,1,2,2,3,4,5,6}

    // invalid use of void expression
    // std::thread threadTwo( list_contains(7) );

    bool b;

    std::thread threadTwo( list_contains, 7 ); // how would you check the bool value of list_contains?

    std::thread threadOne( add_to_list, 7 );

}