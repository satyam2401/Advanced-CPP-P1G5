#include <string>
#include <thread>
#include <stdexcept>
#include <vector>

using namespace std;


class Buffer{

public:

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

Buffer(int packetSize, int maxID){}

void receiveValue(string value, int index){}

void receiveValueHandler(string *values[], int numValues){}

thread clearValuesHandler(){}

void clearValues(){}

};

