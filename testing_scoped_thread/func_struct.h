#ifndef THE_FUNC_FUNCTIONS
#define THE_FUNC_FUNCTIONS


struct func{
    int &i;

    // default constructor declaration (see func_struct.cpp)
    func(int &i_):i(i_){} 
    // declaration
    void operator()();
};

#endif