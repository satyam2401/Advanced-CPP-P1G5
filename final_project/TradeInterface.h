#include <string>
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

// do I/o
class TradeInterface{
    public:

    string header = "CHALLENGE_RESPONSE"; // 18
    string challenge_id; // 4
    string ticker; // 7: SECXXXX 
    string submitted_bid; // 7: XX.XXXX
    string submitted_ask; // 7: XX.XXXX
    string client_name = "Group5"; // 6

    TradeInterface(int port, int packetSize){}

    string *getSecurities(){}

    void getChallenge(){}

    void submitResponse(string submittedBid, string submittedAsk){}
};