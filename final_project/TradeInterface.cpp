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

    string* getSecurity(){
        // receive a packet and return a list of the strings


        // sendto(this->client_fd, "Hello Server", strlen("Hello Server"), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        string values[10] = {0};

        for (int i=0; i<packetSize; i++){
            char buffer[1024] = {0};
            recvfrom(this->client_fd, buffer, 16, 0, (sockaddr*)&this->server_addr, &this->server_len);
            values[i] = buffer;
        }

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