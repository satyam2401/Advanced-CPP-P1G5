#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <chrono>
#include <atomic>
#include <cassert>
#include <random>

struct Order {
    std::string id;  // String-based order ID
    double price;
    int quantity;
    bool isBuy;
};

class OrderBook {
private:
    std::map<double, std::unordered_map<std::string, Order>> orderLevels;
    

public:
    std::unordered_map<std::string, Order> orderLookup;

    void addOrder(const std::string& id, double price, int quantity, bool isBuy) {
        Order order = {id, price, quantity, isBuy};
        orderLevels[price][id] = order;
        orderLookup[id] = order;
    }

    void modifyOrder(const std::string& id, double newPrice, int newQuantity) {
        if (orderLookup.find(id) != orderLookup.end()) {
            Order oldOrder = orderLookup[id];
            orderLevels[oldOrder.price].erase(id);  
            addOrder(id, newPrice, newQuantity, oldOrder.isBuy);
        }
    }
    
    void deleteOrder(const std::string& id) {
        if (orderLookup.find(id) != orderLookup.end()) {
            Order order = orderLookup[id];
            orderLevels[order.price].erase(id);  
            orderLookup.erase(id);
        }
    }

    void benchmark() {
        auto start = std::chrono::high_resolution_clock::now();
        
        addOrder("ORD001", 50.10, 100, true);
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        
        std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
    }
};

class OptimizedOrderBook {
    std::vector<Order> orderPool;
    int orderCounter = 0;
    int oldestOrder = 0;
    size_t size = 0;
    // std::atomic<int> orderCount{0};

public:
    OptimizedOrderBook(){}

    OptimizedOrderBook(size_t size) {
        orderPool.reserve(size); 
        this->size = size;
    }

    void handleOrder(Order O){
        // given an incoming order, put it in the pool if memory exists. Else, clear the oldest order.
        if (orderCounter < this->size){
            orderCounter++;
            this->orderPool.push_back(O);
        } else {
            // order counter is equal to the size of the vector
            // could put in longer-term storage with some condition
            this->orderPool[oldestOrder] = O;
            oldestOrder++;

            if (oldestOrder == this->size){
                oldestOrder = 0;
            };
        };
    }

    void processOrders(std::vector<Order>& orders) {
        for (size_t i = 0; i < orders.size(); i += 2) {  // Unrolling
            handleOrder(orders[i]);
            if (i + 1 < orders.size()) handleOrder(orders[i + 1]);
        }
    }

    // void addAtomicOrder() {
    //     this->orderCount.fetch_add(1, std::memory_order_relaxed);
    // }
};

void testAddOrder() {
    OrderBook book;
    book.addOrder("ORD001", 50.10, 100, true);
    
    // orderLookup
    assert(book.orderLookup.count("ORD001") == 1);  // Order should exist
}

void stressTestUnopt(OrderBook& book, int numOrders) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> priceDist(50.0, 100.0);
    std::uniform_int_distribution<int> quantityDist(1, 500);

    for (int i = 0; i < numOrders; ++i) {
        std::string id = "ORD" + std::to_string(i);
        double price = priceDist(rng);
        int quantity = quantityDist(rng);

        book.addOrder(id, price, quantity, true);
    }
}

void stressTestOpt(OptimizedOrderBook& book, int numOrders) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> priceDist(50.0, 100.0);
    std::uniform_int_distribution<int> quantityDist(1, 500);
    // vector of orders
    std::vector<Order> OrderList;

    for (int i = 0; i < numOrders; ++i) {
        std::string id = "ORD" + std::to_string(i);
        double price = priceDist(rng);
        int quantity = quantityDist(rng);
        Order o = {id, price, quantity, true};
        OrderList.push_back(o);
    }
    
    book.processOrders(OrderList);
}

int main(){

    const int testReps = 5;
    size_t testSizes[testReps] = {100000, 500000, 1000000, 5000000, 10000000};
    double testTimesU[testReps];
    double testTimesO[testReps];

    std::vector<OptimizedOrderBook> OptBooks;

    for (int i=0; i < testReps; i++){
    OrderBook unOptBook = OrderBook();
    OptBooks.push_back(OptimizedOrderBook(testSizes[i]));

    int numRuns = 1;
    // any number greater than 1 throws error

    // "warm start"
    // stressTestUnopt(unOptBook, 10);
    

    double unOptAvg = 0;
    for (int i=0; i < numRuns; i++){
        auto startU = std::chrono::high_resolution_clock::now();
        stressTestUnopt(unOptBook, testSizes[i]);
        auto endU = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedU = endU - startU;
        
        unOptAvg += elapsedU.count();
    };
    testTimesU[i] = (unOptAvg / numRuns);

    double optAvg = 0;
    for (int i=0; i < numRuns; i++){
        auto startO = std::chrono::high_resolution_clock::now();
        stressTestOpt(OptBooks[i], testSizes[i]);
        auto endO = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsedO = endO - startO;

        optAvg += elapsedO.count();
    };
    testTimesO[i] = (optAvg / numRuns);

    };
    
    std::cout << "Execution times (Unoptimized): ";
    for (int i=0; i < testReps; i++){
        // std::cout << "size " << testSizes[i] << ": ";
        std::cout << testTimesU[i] << ", ";
    };
    std::cout << "\b\b seconds "<< std::endl;

        std::cout << "Execution times (Optimized): ";
    for (int i=0; i < testReps; i++){
        // std::cout << "size " << testSizes[i] << ": ";
        std::cout << testTimesO[i] << ", ";
    };
    std::cout << "\b\b seconds "<< std::endl;

    return 0;
};

// base
// Execution times (Unoptimized): 0.0046684, 1.87e-05, 1.61e-05, 1.63e-05, 1.63e-05 seconds 
// Execution times (Unoptimized): 1.4e-05, 1.12e-05, 1.07e-05, 1.08e-05, 1.09e-05 seconds