#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <socket.h>
#include <in.h>
#include <inet.h>
#include <unistd.h>

using namespace std;

// do I/o
class TradeInterface{
    string header;
    string challenge_id;
    string ticker;
    string submitted_bid;
    string submitted_ask;
    string client_name;

    string getSecurity(){
        // receive a packet and return a list of the strings
        string *values[];
        return values;
    }

    void submitResponse(){
        // submit to main server in form:
        string response = this->header +
                        this->challenge_id + 
                        this->ticker + 
                        this->submitted_bid  + 
                        this->submitted_ask +
                        this->client_name;
    }
};