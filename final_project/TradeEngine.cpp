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

