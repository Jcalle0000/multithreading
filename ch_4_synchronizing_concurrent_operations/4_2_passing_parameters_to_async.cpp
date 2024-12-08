#include<string>
#include<future>

struct X{
    // function foo takes 2 parameters
    void foo(int, std::string const& ); // the const extends the life of the r-value
    // function bar takes 1 parameter
    std::string bar(std::string const&);
};

X x;

                // the type and functionn, the parameter
auto f1=std::async(&X::foo,&x, 42, "hello"); // calls p->foo where p is &x
auto f2=std::async(&X::bar,x,"goodbye"); // calls tmpx, where this is a copy of x

// can we detect when a copy is needed?
// if so, can we tell the user to pass in the reference in static analysis?

struct Y{
    // we overload the function call operator
    double operator()(double); // this acts a functor object?
};

Y y;

auto f3=std::async(Y(), 3.141); // what is meant by
// "calls tmpY(3.141)" where tmpy is move-constructed
// from Y()

// double result = y(5.0);  // Calls y.operator()(5.0)


