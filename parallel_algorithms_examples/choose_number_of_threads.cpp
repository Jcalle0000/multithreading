// std::thread::hardware_concurrency()
//  on a multicore system it might return the number of CPU cores
// function might return 0 if the information is not available

// Implementation below assumes that none of the operations will thrown an exception

// Handling exceptions for thread is in ch 8

// a template is used for accumulate_block, 
// so we need to pass the iterator class and the T class

// operator allows the struct "accumulate_block" to be used 
// as a function?

template<typename Iterator, typename T> 
struct accumulate_block{ // will this act as a function?
    void operator()( Iterator first, Iterator last, T& result ){
        result = std::accumulate( first,last,result ); // what is std accumulate
    }
}


// Divide the number of elements to processs by the minimum block size,
// in order to give the maximum number of threads

template<typename, Iterator, typename T>
T parallel_accumulate( Iterator first, Iterator last, T init ){
    unsigned long const length = std::distance(first,last); // this is linear?
    // why const? to not change its value?

    if(!length)
        return init;

    // why is everything unsigned long ?
    
    unsigned long const min_per_thread=25;

    unsigned long const max_threads = 
        (length+min_per_thread-1)/min_per_thread;

    unsigned long const hardware_threads = 
        std::thread::hardware_concurrency();

    // conditional operator
    // hardware_threads!=0?hardware_threads:2,max_threads

    // bool condition = hardware_threads!=0; // will return true or false
    // condition?hardware_threads:2,max_threads;
    // if condition is true max hardware_threads=2
    // else make hardware_threads=max_threads

    unsigned long const num_threads=
        std::min( hardware_threads!=0?hardware_threads:2,max_threads  );

    unsigned long const block_size=length/num_threads;

    std::vector<T> results(num_threads);
    // a vector with O( num_threads) space?

    std::vector<std::thread> threads(num_threads-1);

    Iterator block_start = first;

    for( unsigned long i=0;i<num_threads-1;++i){
        Iterator block_end=block_start;
        std::advance(block_end, block_size); // what is this

        threads[i]=std::thread(
            accumulate_block<Iterator, T>(),
            block_start, block_end, std::ref( results[i] )
        );
        block_start=block_end;
    }

    accumulate_block<Iterator, T>()(
        block_start, last, result[num_threads-1];

        std::for_each( threads.begin(), threads.end(),
            std::mem_fn( &std::thread::join )
        );
        return std::accumulate( results.begin(), results.end(), init );
    )


}
