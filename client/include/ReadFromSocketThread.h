#pragma once
#include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"
using namespace std;

class ReadFromSocketThread{
private:
        ConnectionHandler &connectionHandler;
        StompProtocol &protocol;
public:
        ReadFromSocketThread(ConnectionHandler &connectionHandler, StompProtocol &protocol);
        void run();

};