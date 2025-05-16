#include <string>
#include <thread>
#include <stdexcept>
#include <vector>

using namespace std;


class Buffer{
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
    const int numCoresAvailable = 5;
    thread *mapperStorage = new thread[this->numCoresAvailable];

Buffer(int packetSize, int maxID){
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

void receiveValueHandler(string *values[]){
    // when each packet is received, clear the last one in the background
    thread clearer = thread(clearValues);
    // don't know if the detach is necessary.
    clearer.detach();

    for (int j=0; j < this->packetSize; j+=this->numCoresAvailable){
        // initialize 5 mappers
        for (int i=0; i < this->numCoresAvailable; i++){
            this->mapperStorage[i + j] = thread(receiveValue, values[i]);
        }

        // join the mappers back 
        for (int i=0; i < this->numCoresAvailable; i++){
            this->mapperStorage[i + j].join();
        }
    }

    // only one clearer should be going at once, but don't disrupt the mappers.
    clearer.join();


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