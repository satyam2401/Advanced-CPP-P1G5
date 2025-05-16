#include <string>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

// do I/o
class TradeInterface{
    string header;
    string challenge_id;
    string ticker;
    string submitted_bid;
    string submitted_ask;
    string client_name;

    string getSecurity(){}

    void submitResponse(){}
};