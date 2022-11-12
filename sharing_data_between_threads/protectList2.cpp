
#include<list> // ?
#include<mutex> // mutex and lock_guard
#include<algorithm>
#include<thread>

class someObject{

    public:
    void add_to_list( int new_value){
    // void operator()(){

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

    // someObject( int &tempValue_):tempValue(tempValue_){}

    
    // void operator()(){
    //     add_to_list()
    // }


    private:
        std::list<int> some_list;
        // firstMutex is suppose to protect "some_list"
        std::mutex firstMutex;
        int tempValue;
};

// g++ protectList2.cpp -o protectList2.exe

int main(){

    someObject anObject1;
    int i=0;
    while(i<6){
        anObject1.add_to_list(i);
        i++;
    }
    // anObject1.some_list= {0,1,2,2,3,4,5}

    // invalid use of void expression
    std::thread firstThread( anObject1.add_to_list,6 );
}