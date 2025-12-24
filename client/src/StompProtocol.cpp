#include "../include/StompProtocol.h"
// #include "../include/summary.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <atomic>
#include <iostream>
#include "event.h"
using namespace std;

//// I  ADD
std::atomic<int> ID(1);
std::atomic<int> RECIEPT(1);
vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    res.push_back (s.substr (pos_start));
    return res;
}
//,subIdTochannel(new map<string,string>),channelTosubId(new map<string,string>)
bool StompProtocol::isTerminated(){
    return isterminated;
}
void StompProtocol::terminate(){
    isterminated=true;
}
StompProtocol::StompProtocol(ConnectionHandler &connectionHandler,summary &summary):Summary(summary), connectionHandler(connectionHandler),isterminated(false),UserName(""){}
string StompProtocol::toServerMsg(string msg){
    std::vector<std::string> words;
    words = split(msg, " ");
    std::string command=words[0];
    std::string sendmsg ="";
    if(command=="login")
    {
        UserName=words[2];
        std::string commandd = "CONNECT";
        std::string accept_version = "accept-version:1.2";
        std::string host = "host:stomp.cs.bgu.ac.il";
        std::string login = words[2];
        std::string passcode = words[3];
        sendmsg="CONNECT\naccept-version:1.2\nhost:stomp.cs.bgu.ac.il\nlogin:"+login +"\npasscode:"+passcode+"\n";   
    }
  else if(command=="join"){
        std::string commandd = "SUBSCRIBE";
        subIdTochannel[to_string(ID)]=words[1];
        channelTosubId[words[1]]=to_string(ID);
        sendmsg="SUBSCRIBE\ndestination:/"+words[1]+"\nid:"+ std::to_string(ID.fetch_add(1))+"\nreceipt:"+std::to_string( RECIEPT)+"\n";  
        receipt2sentFrame[to_string(RECIEPT.fetch_add(1))] = sendmsg;
    }
  else if(command=="exit"){
        string commandd = "UNSUBSCRIBE";
        string accept_version = "1.2";
        string distination = words[1];
        string id =channelTosubId[words[1]]; 
        sendmsg="UNSUBSCRIBE\nid:"+id+"\nreceipt:"+std::to_string(RECIEPT)+"\n\n";
        receipt2sentFrame[to_string(RECIEPT.fetch_add(1))] = sendmsg;
    }
    else if(command=="report"){
        string path=words[1];
        names_and_events events= parseEventsFile(path);
        SEND(events);
    }
    else if(command=="logout"){ 
        std::string commandd = "DISCONNECTED";
        sendmsg="DISCONNECT\nreceipt:"+std::to_string(RECIEPT)+"\n";
        receipt2sentFrame[to_string(RECIEPT.fetch_add(1))] = sendmsg;
    }
    else if (command == "summary"){
        ofstream file(words[3]);
        string s=Summary.getsummary(words[1],words[2]);
        file << s;
        file.close();
        return "summary";
    }
   return sendmsg;  

}
    // string sub="subscribsion:";
    // string mesgid="message-id:"+messageId++;
void StompProtocol::SEND(names_and_events event){

   for(int i=0;i<event.events.size();i++){
        string command="SEND";
        string destination="/"+event.team_a_name+"_"+event.team_b_name;
        string user = "user:"+UserName;
        string teamA  ="team a:"+event.team_a_name ; 
        string teamB = "team b:"+event.team_b_name;
        string sendmsg=command+"\ndestination:"+destination+"\n\n"+user+"\n"+teamA+"\n"+teamB;
        string eventName = event.events[i].get_name();
        int time = event.events[i].get_time();
        string discription = event.events[i].get_discription();
        map<string,string> m=event.events[i].get_game_updates();
        string GameUpdates="general game updates :\n"+printMap(m);
        map<string,string> m1=event.events[i].get_team_a_updates();
        string TeamAUpdates="team a updates :\n"+printMap(m1);
        map<string,string> m2=event.events[i].get_team_b_updates();
        string TeamBUpdates="team b updates :\n"+printMap(m2);
        string Desc="description:\n"+event.events[i].get_discription();
        sendmsg=sendmsg+"\nevent name: "+eventName+"\ntime: "+to_string(time)+"\n"+GameUpdates+TeamAUpdates+TeamBUpdates+Desc+"\n";
        connectionHandler.sendLine(sendmsg);
    }
}
string StompProtocol::printMap(map<string,string> m){
    string themap="";
     std::map<std::string,string>::iterator it = m.begin();
    for(it=m.begin();it!=m.end();it++){
        themap=themap+it->first +":"+it->second+"\n";
    }
    return themap;
}

string StompProtocol::MsgFromServer(string msg){
    std::vector<std::string> words;
    words = split(msg, "\n");
    std::string command=words[0];
    std::string output ="";
   
   if(command=="CONNECTED"){
        std::cout << "Login successful...\n" << std::endl;
        return "login succeeded";
    }
    else if(command=="RECEIPT"){
        string receiptId=split(words[1],":")[1];
        string sentFrame=receipt2sentFrame[receiptId];
        string sentFrameType=split(sentFrame,"\n")[0];
      //  cout<<"the sentframe from sngserver is:\n"+sentFrame<<endl;
        if(sentFrameType=="SUBSCRIBE"){
            string dest=split(sentFrame,"\n")[0];
            string gameName=split(split(sentFrame,"\n")[1],"/")[1];
            return "Joined channel " + gameName+"\n";
        }
        else if(sentFrameType=="UNSUBSCRIBE"){
            string unsubbedFrom=split(sentFrame,"\n")[1];
            //cout<<"the unsubbedFrom is "+unsubbedFrom<<endl;
            string channel=subIdTochannel[split(unsubbedFrom,":")[1]];
            return "Exited channel "+ channel;

        }
        else if(sentFrameType == "DISCONNECT"){
             return "disconnected";
        }
        // string sentFrame = receipt2sentFrame[split(words[1], ":")[1]];
        // string sentFrameType = split(sentFrame, "\n")[0];
        // if(sentFrameType == "DISCONNECT")
        //     sendmsg = "disconnected";
        // else if(sentFrameType == "UNSUBSCRIBE"){
        //     string channel = subIdTochannel[split(split(sentFrame, "\n")[1], ":")[1]];
        //     sendmsg = "Exited channel " + channel;
        // }
        // else if(sentFrameType == "SUBSCRIBE"){
        //     string channel = subIdTochannel[split(split(sentFrame, "\n")[2], ":")[1]];
        //     sendmsg = "Joined channel " + channel;
        
    }
    else if(command=="MESSAGE"){
        std::cout << "the message come to  me ...\n"<<endl;
        makeEvent(msg);
    }
    else if(command=="ERROR")
        return split(words[1], ":")[1];

  
    return output;
}
void StompProtocol::makeEvent(string msg)
{
        vector<string> words = split(msg, "\n");
        string topic=split(words[3],":")[1];
        topic=topic.substr(1,topic.size());
        string user =split(words[5],":")[1];
        string teamA  =split(words[6],":")[1];
        string teamB = split(words[7],":")[1];
        string eventName = split(words[8],":")[1];
        int time = stoi((split(words[9],":"))[1]);
        //cout << "in make event we make: \n"+ topic+ "\n"+ user+ "\n"+ teamA + "\n"+teamB + "\n"+eventName  <<endl;
        //cout << "the name of topic is topic :"+topic <<endl;   
        int i=11;
        map<string,string>GameUpdates;
        map<string,string>TeamAUpdates;
        map<string,string>TeamBUpdates;
        string desc;
    while(words[i] != "team a updates :"){
        GameUpdates[split(words[i], ":")[0]] = split(words[i], ":")[1];
        i++;
    }
    i++;
    while(words[i] != "team b updates :"){
        TeamAUpdates[split(words[i], ":")[0]] = split(words[i], ":")[1];
        i++;
    }
    i++;
    while(words[i] != "description:"){
        TeamBUpdates[split(words[i], ":")[0]] = split(words[i], ":")[1];
        i++;
    }
    i++;
    desc = words[i];
    Event even(teamA,teamB,eventName,time,GameUpdates,TeamAUpdates,TeamBUpdates,desc);
    Summary.add(even,user,topic);
}

