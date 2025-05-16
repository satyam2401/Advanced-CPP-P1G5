#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <random>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include "Buffer.h"
#include "TradeInterface.h"

// TradeEngine contains the logic for the system
// input: 10 securities (strings) to parse, with a challenge_ID and a target
    // one of the securites will have the listed target - find and return that one.
    // SEC|SEC0005|BID|64.642|ASK|65.0693, TARGET:SEC0005
    // CHALLENGE_RESPONSE <CHALLENGE_ID> <TARGET_TICKER> <BID> <ASK> <TraderName>

// one thread does I/O - it gathers all the securities, passes them into a Buffer without parsing.
    // this can write to the buffer, but not read from it.
    // the info comes in one security at a time.
    // I/O will collect 5 at a time, then pass all 5 to the mapper. do twice.
    // It will then read the challenge ID and target.
    // query the buffer's map for the target security 

// this Buffer will have enough space for 2 or 3 sets of securities (strings)
    // this probably is the most computationally intensive part
    // running on its own few threads - one to clear out parts of the buffer, some to make a map
    // it will also have a starting index that is writeable only by the buffer clearer and readable by all else
        // this is the starting index of the currently in use portion of the buffer
    
    // buffer has a map (size 1000) which stores the security ID's, and their corresponding positions in the buffer
    // map readable by all, writable only by the buffer mapper.
    // starts from the starting index, reads the chars [7, 8, 9, 10] from each string as ints
    // puts pointer to string obj in buffer in the map at index [chars it read]


const int packetSize = 10;
const int maxID = 9999;
const int port = 8080; // change!!!!

Buffer *orderBuffer = new Buffer(packetSize, maxID);
TradeInterface *tradeInterface = new TradeInterface(port, packetSize);
string *securities = new string[packetSize];
string *parsed = new string[3];

// need these for multithreaded string parsing, but 
double parseBid(string security){
    string sec_to_p = security.substr(16, 7);
    double sec;

    if(sec_to_p[sec_to_p.length() - 1] == '|'){
        sec = stod(security.substr(0, 6));
    } else {
        sec = stod(security);
    }

    return sec;
}

double parseAsk(string security, int len){
    string sec_to_p = security.substr(len, 7);
    double sec;

    if(sec_to_p[0] == '|'){
        sec = stod(security.substr(0, 6));
    } else {
        sec = stod(security);
    }

    return sec;
}

// not sure how efficient this is. I would prefer to do this multithreaded, 
    // but we are not guaranteed the character length of the bid value: "7 or less" 
void parseSec(string sec, char delim){
    // declaring temp string to store the curr "word" upto del
    string temp = "";
    int numDelim = 0;
  
    for(int i=0; i<(int)sec.size(); i++){
        // If cur char is not delim, append it to the current section, otherwise
        // you have completed the section, add to array, and start a new section.
        if(sec[i] != delim){
            temp += sec[i];
        }
        else{
            parsed[i] = temp;
            numDelim++;
            temp = "";
        }
    }
}


int main(){

    while (true){
        // main program loop

        securities = tradeInterface->getSecurities();
        // I kinda want to detach these so the challenge can be gotten at the same time-
            // mapper.join() is preventing this, but is this necessary?
            // we need to test this.
        orderBuffer->receiveValueHandler(&securities);

        // If the mappers use all the cores, these tasks will be put on the stack.
            // These will probabaly not complete before all the mappers are done - expected.
                // this receives two messages, so it should be slower than any one mapper.
                // we should test this and see if we need to use more control.
            // I want these to be running at the same time as the mappers, 
            // but for the map to be accessed only after all mappers complete.
            // Because the threads are in different functions, this may be more difficult.
            // Also, using join() on threads that have already completed (so, most of them, I hope) is wasteful.
        tradeInterface->getChallenge();

        int target = stoi(tradeInterface->ticker.substr(3, 4));

        // I could run the above two on their own threads, and detach them.
            // at this point, all mappers must be completed. This is not enforced.
        string security = orderBuffer->map[target];

        // parse the security string
        // SEC|SEC0005|BID|64.642|ASK|65.0693
        
        parseSec(security.substr(16), '|');
        tradeInterface->submitResponse(parsed[0], parsed[2]);
    }

    delete orderBuffer;
    orderBuffer = nullptr;

    delete tradeInterface;
    tradeInterface = nullptr;

    delete securities;
    securities = nullptr;

    delete parsed;
    parsed = nullptr;

    return 0;
};
