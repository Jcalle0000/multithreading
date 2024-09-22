#include<mutex>

class some_big_object;
void swap(smme_big_object &lhs, some_big_object &rhs);

class X // using this to encapsulate "some_big_object"
{

    private:
        some_big_object o1;

        // mutual exlclusion, limiting the access of multiple threads for shared data
        std::mutex m; 

    public:
        // constructor? with initializer list?
        X(some_big_object const& const_big_object):o1(const_big_object){

        }

        // "friend swap" - allows for access to the private and protected members of the class
        // making the swap more efficient 
        friend void swap(X& lhs, X& rhs){
            if(&lhs==&rhs){
                return;
            }
            std::lock(lhs.m, rhs.m); // we lock both mutexes 
            // used to avoid deadlock?
            std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock);

            std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock);
            swap(lhs.o1, rhs.o1);

        }

};