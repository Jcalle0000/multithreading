#include<list> // ?
#include<mutex> // mutex and lock_guard
#include<algorithm>
#include<string>

class some_Object{
    private:
        std::list<int> some_list;
        std::string b;
    
    public:
        void add_to_list(int new_value){
            
        }

};

class some_Object_wrapper{
    private:
        some_Object anObject;
        std::mutex firstMutex;
    public:
        template<typename Function>
        void process_anObject(Function func){ //  Function?
            std::lock_guard<std::mutex> l(firstMutex);
            func(anObject);
        }

}