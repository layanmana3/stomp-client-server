#include "ConnectionHandler.h"
#include "StompProtocol.h"
#include <boost/lexical_cast.hpp>
#include <thread>
using namespace std;

vector<string> splitz(string s, string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

void run(StompProtocol &protocol, ConnectionHandler &connectionHandler)
{
    while (!protocol.isTerminated())
    {
        string response = "";
        if (connectionHandler.getLine(response))
        {
            cout << "received the frame:\n"
                 << response << endl;
            string output = protocol.MsgFromServer(response);
            if (output == "disconnected")
                protocol.terminate();
            cout << output << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    // std::string host = argv[1];
    // short port = atoi(argv[2]);
    while (1)
    {
        ConnectionHandler connectionHandler("-1", -1);
        summary summary;
        StompProtocol protocol(connectionHandler, summary);
        // login
        bool loggedIn = false;
        while (!loggedIn)
        {
            string loginCommand;
            getline(cin, loginCommand);
            string host = splitz(splitz(loginCommand, " ")[1], ":")[0];
            short port = boost::lexical_cast<short>(splitz(splitz(loginCommand, " ")[1], ":")[1]);
            // se**
            connectionHandler.host_ = host;
            connectionHandler.port_ = port;
            if (!connectionHandler.connect())
            {
                std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
                continue;
            }
            string frameToSend = protocol.toServerMsg(loginCommand);
            connectionHandler.sendLine(frameToSend);
            string response = "";
            connectionHandler.getLine(response);
            string output = protocol.MsgFromServer(response);
            if (output != "login succeeded")
            {
                cout << output << endl;
                connectionHandler.close();
                continue;
            }
            loggedIn = true;
            string command;
            thread t(&run, ref(protocol), ref(connectionHandler));
            while (!protocol.isTerminated())
            {
                getline(cin, command);
                string frameToSend = protocol.toServerMsg(command);
                if (frameToSend == "summary")
                    continue;
                cout << "sending the frame:\n"
                     << frameToSend << endl;
                connectionHandler.sendLine(frameToSend);
                if (splitz(frameToSend, "\n")[0] == "DISCONNECT")
                    t.join();
            }
        }
    }
}
