#pragma once
#include "event.h"
#include "../include/ConnectionHandler.h"
using namespace std;
#include "../include/summary.h"
// TODO: implement the STOMP protocol
class StompProtocol
{
private:
    bool isterminated;
    map<string,string> subIdTochannel;
    map<string,string> channelTosubId;
    map<string, string> game_nameTouser_name;
    map<string,names_and_events> user_nameTogame_name;
    map<string, string> receipt2sentFrame;
    string UserName ;
    ConnectionHandler &connectionHandler;
    int messageId=0;
    summary Summary;
public:
    StompProtocol(ConnectionHandler &connectionHandler,summary &Summary);
    string toServerMsg(string msg);
    string MsgFromServer(string msg);
    bool isTerminated();
    void terminate();
    void SEND(names_and_events event);
    string printMap(map<string,string> m);
    void makeEvent(string msg);
};
