 // atomic operations

 // cannot be observed, its either done or undone

 // atomic types include bool, char, int, short, long etc

 // theres also

 // int_least8_t, int_least64_t, int_fast8_t

// member functions include fetch_add, fetch_or, load, store, exchange
// compare_exchange_weak, compare_exchange_strong


// each of these operations have optional memory-ordering arguments
// store opeations
    // memory_order_relaxed, memory_order_release, memory_order_seq_cst

// load operations
    // memory_order_relaxed, memory_order_consume, memory_order_aquire
    // memory_order_seq_cst
    
// default for all operations is memory_order_seq_cst