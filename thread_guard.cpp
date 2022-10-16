class scoped_thread
{
    std::thread t;
    public:
        explicit scoped_thread(std::thread t_): // why is this made explicit?
            t(std::move(t_)) // move constructor
            {
                if(!t.joinable())
                    throw std::logic_error(“No thread”); // what is std::logic_error?

            }
        ~scoped_thread() // destructor
        {
            t.join();
        }
        
        scoped_thread(scoped_thread const&)=delete; // copy constructor
        scoped_thread& operator=(scoped_thread const&)=delete; // copy operator
};



struct func;
void f() {
See listing 2.1
int some_local_state;
scoped_thread t(std::thread(func(some_local_state)));
e
    do_something_in_current_thread();
}
