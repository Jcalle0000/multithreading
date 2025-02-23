#include<iostream>
#include<vector>

int main(){
    // std::vector<int> vec = {5, 10, 15, 20, 25}; // output for partition point will be 2

    std::vector<int> vec = {5, 10, 15, 20, 25, 30, 35, 40}; // outputs 5 
    // referring to first index where odd number is seen

    // predicate
    auto is_even=[](int x){
        return x%2==0;
    };

    // partition the vector 
    // re-arrange the elements such that the even numbers appear before
    // the odd numbers
    auto partition_point=std::partition(
                                        vec.begin(),
                                        vec.end(),
                                        is_even);

    // display the partition vector
    std::cout<< "Partitioned vec:\n";

    for(const auto &num:vec ){
        std::cout<< num << "\n";
    }

    // output the iterator returned by partition
    std::cout<< "Partition point index" << std::distance(vec.begin(), partition_point) << "\n";

}