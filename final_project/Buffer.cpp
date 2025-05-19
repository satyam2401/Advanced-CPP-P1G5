#include <string>
#include <thread>
#include <stdexcept>
#include <vector>

using namespace std;


class Buffer{
    public:
    // There should not be data races here. Thus, there should be no need for atomic.
    // I have designed this such that no threads are writing to the same place in memory.

    int packetSize;
    int maxID;
    int startingIndex = 0;
    // could be changed
    const int bufferSpace = 3;
    bool *clearables = new bool[this->bufferSpace];
    string *buffer;
    string *map;
    thread mapper;
    thread clearer;
    // Xi's laptop M3 Pro has 12 cores - use 5 for mapping
    // this should be a number s.t. packetSize (10) / numCoresAvailable is an integer.
    // currently testing with the OS task switching, not using join().
    const int numCoresAvailable = 8;
    thread *mapperStorage = new thread[this->numCoresAvailable];

Buffer(int packetSize, int maxID){
    // packetSize is max 1000
    this->packetSize = packetSize;
    this->maxID = maxID;
    string *map = new string[maxID];
    string *buffer = new string[packetSize * bufferSpace];
    // thread mapper = thread();
    // thread clearer = thread();
}

void receiveValue(string value, int index){
    this->buffer[startingIndex + index] = value;
    // this could be optimized
    int secIdx = stoi(value.substr(7, 4));
    this->map[secIdx] = this->buffer[startingIndex + index];
}

void receiveValueHandler(string *values[], int numValues){

    // joining manually
    // for (int j=0; j < this->packetSize; j+=this->numCoresAvailable){
    //     // initialize 5 mappers
    //     for (int i=0; i < this->numCoresAvailable; i++){
    //         this->mapperStorage[i + j] = thread(receiveValue, values[i]);
    //     }

    //     // join the mappers back 
    //     for (int i=0; i < this->numCoresAvailable; i++){
    //         this->mapperStorage[i + j].join();
    //     }
    // }

    // OS switches tasks - but won't for 8 cores 
    for(int j=0; j < numValues; j++){
        // is detatch() relevant here, if threads are stored in a container on the heap?
            // they aren't owned by the function due to that, right?
        this->mapperStorage[j] = thread(receiveValue, values[j]);
        this->mapperStorage[j].detach();
    }
    // dont rejoin

    // only one clearer should be going at once, but don't disrupt the mappers.
    // clearer.join(); // don't think join() works after detach().
}

thread clearValuesHandler(){
        thread clearer = thread(clearValues);
        // don't know if the detach is necessary.
        clearer.detach();

        return clearer;
}
void clearValues(){
    // only one thread needs to be clearing
    // this is the one that updates the startingIndex

    for(int i = 0; i < this->bufferSpace * this->packetSize; i += this->packetSize){
        int clearMe = i / this->packetSize;

        // don't clear the current section but clear everything else.
        // the other threads are using this section.
        if(i==this->startingIndex){
            this->clearables[clearMe] = false;
        } else {
            this->clearables[clearMe] = true;
        }
    }

    for(int i=0; i < this->bufferSpace; i++){
        if(this->clearables[i]){
            // clear the section
            for(int j=0; j < packetSize; j++){
                int clearIdx = i * this->packetSize + j;
                delete &this->buffer[clearIdx];
                this->buffer[clearIdx] = nullptr;
            }
        }
    }

    // update the startingIndex
    this->startingIndex += this->packetSize;
    if(this->startingIndex > this->packetSize * this->bufferSpace){
        this->startingIndex = 0;
    }
}


};