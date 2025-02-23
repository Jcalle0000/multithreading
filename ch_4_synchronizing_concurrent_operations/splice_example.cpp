#include<list>

int main(){
    std::list<int> list1= {1,2,3,4};
    std::list<int> list2 = {5,6,7};

    // use splice to move elements from list2 to list1
    // move the first element from list2 to list1

    list1.splice(list1.end(), list2, list2.begin() );

    
}