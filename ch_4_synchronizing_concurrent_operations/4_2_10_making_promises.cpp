
// when would you need to handle a lot of network connections?
// when would you need to make a lot of threads?

// using too many threads can consume OS resources affectinng performace

// std::promise<T> provides a means of setting a value
// this can be later read through std::future<T>

// when the value of the promise is set (using set_value())
// the future becomes ready and  can be used to retrived the store
// value

// if you destroy the std::promise without setting a value
// an exception is store instead

// example code for thread processing connections
// 

#include<future>

#include<iostream>
#include<future>
#include<queue>
#include<unordered_map>
#include<vector>
#include<thread>
#include<mutex>
#include<chrono>

// simulate payload type

using payload_type =  std::string;

struct data_packet{
    int id;
    payload_type payload;
};

// std::promise is a non-copyable

struct outgoing_packet{
    payload_type payload;
    std::promise<bool> promise;
};

class connection{
    private:
        int id;
        std::mutex mtx;
        std::queue<data_packet> incoming_queue;
        std::queue<outgoing_packet> outgoing_queue;
        std::unordered_map<int, std::promise<payload_type> > promises_map;

    public:

        // default constructor
        connection(int id): id(id){}

        // simulate incoming data
        bool has_incoming_data(){
            std::lock_guard<std::mutex> lock(mtx);
            
            // if empty (true) we negate it to false
            // if not empty (false) we negate it to true
            return !incoming_queue.empty();
        }

        data_packet incoming(){
            std::lock_guard<std::mutex> lock(mtx);

            // this will probably require a move to?
            data_packet packet= incoming_queue.front();
            incoming_queue.pop();
            return packet;
        }

        bool has_outgoing_data(){
            std::lock_guard<std::mutex> lock(mtx);

            // 
            return !outgoing_queue.empty();
        }

        outgoing_packet top_of_outgoing_queue(){
            std::lock_guard<std::mutex> lock(mtx);

            // b/c this holds packets, and the packets hold promises
            // we need to move the resource
            outgoing_packet packet= std::move( outgoing_queue.front() );
            outgoing_queue.pop();
            return packet;
        }

        void send( const payload_type &payload){
            std::this_thread::sleep_for(
                std::chrono::milliseconds(500)); // simulate send delay
                std::cout<< "Sent payload: " << payload << " from connection "
                << id << "\n";
        }

        // the packet is a struct with id and string
        void add_incoming_data( const data_packet &packet){
            std::lock_guard<std::mutex> lock(mtx);
            
            // how do we add data into the packet?
            // incoming_queue.push(packet);

            incoming_queue.push(
                // packet
                std::move(packet)
            );
        }

        void add_outgoing_data(const outgoing_packet &packet){
            std::lock_guard<std::mutex> lock(mtx);
            // outgoing_queue.push(packet);
            outgoing_queue.push(
                // packet
                std::move(packet)
                );
        }

        std::promise<payload_type> &get_promise(int packet_id){
            std::lock_guard<std::mutex> lock(mtx);
            return promises_map[packet_id];
        }

        std::future<payload_type> get_future(int packet_id){
            std::lock_guard<std::mutex> lock(mtx);
            return promises_map[packet_id].get_future();
        }
};

// using connection_set=std::vector<connection>;
using connection_set = std::vector<
                                    std::shared_ptr<connection>
                                  >;

bool done(const connection_set & connections){
    // simulate a termination condition
    return false;
}


                        // we pass in a vector of connections
void process_connections( connection_set& connections){

    // std::vector<connection>::iterator connection_iterator;

    // we keep loopig until done returns true
    while(!done(connections) )
    {
        // for( std::vector<connection>::iterator connection =connections.begin(), 
        //                                   end=connections.end();
        //     connection!=end; ++connection
        //     ){
        for (auto connection = connections.begin(), end = connections.end(); 
             connection != end; ++connection) {

            // assuming an incoming data packet has some ID and 
            // a payload with the actual data in it
            if( (*connection)->has_incoming_data() ){
                data_packet data=(*connection)->incoming();

                // any incoming packet has some ID and a payload with the 
                // actual data in it

                std::promise<payload_type> &p = (*connection)->get_promise(data.id);
                p.set_value(data.payload);

                std::cout<< "Processed incoming data: "<<
                                            data.payload << "\n";
            }

 
            if( (*connection)->has_outgoing_data() ){

                outgoing_packet data=(*connection)->top_of_outgoing_queue();
                (*connection)->send(data.payload);

                // once the send has completed the promise associated with the outgoing
                // data is set to true to indicate successful transmission
                data.promise.set_value(true);
                std::cout<< "Processed outgoing data: " << data.payload << "\n";
            }
        }
    }
    
}

int main(){

    // this didnt work
    // connection_set connections={
    //     connection{1},
    //     connection(2)
    // };

    connection_set connections = {std::make_shared<connection>(1), std::make_shared<connection>(2)};


    // connection_set connections;
    // connections.push_back( connection(1) );
    // connections.push_back( connection(2) );

    // connections.emplace_back(1);
    // connections.emplace_back(2);

    // connection_set connections;
    // connections.push_back(
    //     std::make_shared<connection>(1)
    // ) ;

    // connections.push_back(
    //     std::make_shared<connection>(2)
    // );

    // simulate adding incooming and outgoing data

    // connections[0].add_incoming_data( 
    //     // intialize the packet
    //     std::move(data_packet{1, "Hello from connection 1"}) 
    // );

    // connections[1].add_incoming_data(
    //     // intialize the packet
    //     {2, "Hello from connection 2"}
    // );

    connections[0]->add_incoming_data(
        // std::move(
        //     data_packet
            {1, "Hello from connection 1"}
        );
    // );

    connections[1]->add_incoming_data(
        // std::move(data_packet
        {2, "Hello from connection 2"});
    // );

    // comprimised of a string and a promise
    outgoing_packet out1={"Goodbye from connection 1"};
    // connections[0].add_outgoing_data( std::move(out1) ) ;
    connections[0]->add_outgoing_data(std::move(out1) );

    outgoing_packet out2={"Goodbye from connection 2"};
    // connections[0].add_outgoing_data( std::move(out2) );
    connections[1]->add_outgoing_data(std::move(out2) );


    // process connectios in a seperate thread
    std::thread processor_thread(
        process_connections, std::ref(connections)    
    );

    // wait for the thread
    // which will run indefinetly
    processor_thread.join();
    return 0;


}