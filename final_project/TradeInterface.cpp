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
    public: 

    string header = "CHALLENGE_RESPONSE"; // 18
    string challenge_id; // 4? (9999 max? - one every 10 seconds for how long?)
    string ticker; // 7: SECXXXX 
    string submitted_bid; // 7: XX.XXXX
    string submitted_ask; // 7: XX.XXXX
    string client_name = "Group5"; // 6
    int client_fd;
    sockaddr_in server_addr;
    int packetSize;
    socklen_t server_len;

    TradeInterface(int port, int packetSize){
        this->client_fd = socket(AF_INET, SOCK_DGRAM, 0);
        this->server_addr = {AF_INET, htons(port), INADDR_ANY};
        this->packetSize = packetSize;
        this->server_len = sizeof(server_addr);
    }

    string* getSecurities(){
        // receive a packet and return a list of the strings

        string values[10] = {0};

        for (int i=0; i<packetSize; i++){
            char buffer[1024] = {0};
            recvfrom(this->client_fd, buffer, 16, 0, (sockaddr*)&this->server_addr, &this->server_len);
            values[i] = string(buffer);
        }

        return values;
    }

    void getChallenge(){

        // challenge
        char chalBuffer[1024] = {0};
        recvfrom(this->client_fd, chalBuffer, 16, 0, (sockaddr*)&this->server_addr, &this->server_len);
        // CHALLENGE_ID:X
        this->challenge_id = string(chalBuffer).substr(12); // include all to end of str

        // ticker
        char tickBuffer[1024] = {0};
        recvfrom(this->client_fd, tickBuffer, 16, 0, (sockaddr*)&this->server_addr, &this->server_len);
        // TARGET:SECXXXX
        this->ticker = string(tickBuffer).substr(7);
    }

    void submitResponse(string submitted_bid, string submitted_ask){
        // submit to main server in form:
        string response = this->header + ' ' +
                        this->challenge_id + ' ' +
                        this->ticker + ' ' +
                        submitted_bid  + ' ' +
                        submitted_ask + ' ' +
                        this->client_name;
        
        const int len = response.length();
        char sendResponse[55];

        strcpy(sendResponse, response.c_str());

        sendto(this->client_fd, sendResponse, len, 0, (sockaddr*)&server_addr, sizeof(server_addr));

    }
};