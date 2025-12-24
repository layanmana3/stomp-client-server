#include "../include/ReadFromSocketThread.h"


ReadFromSocketThread::ReadFromSocketThread(ConnectionHandler &connectionHandler , StompProtocol &protocol): connectionHandler(connectionHandler) , protocol(protocol){}

void ReadFromSocketThread::run(){
    while (!protocol.isTerminated()){
        string response = ""; 
        if(connectionHandler.getLine(response)){
            protocol.MsgFromServer(response);
        }
    }

}