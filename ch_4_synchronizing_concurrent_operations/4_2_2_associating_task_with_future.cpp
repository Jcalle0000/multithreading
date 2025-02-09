#include<vector>
#include<string>

template<>

// can all callable objects be wrapped in a std::function, that can then be passed as a thread function?

class packaged_task<std::string(std::vector<char>*,int)>{ 
    public:
        template<typename Callable>
        explicit packaged_task(Callable&& f); 
        // what does the explicit do?

        std::future<std::string> get_future();
        void operator()(std::vector<char>*, int);
};