#include <vector>
#include <string>
#include <map>
using namespace std;
#include "event.h"
class summary
{
private:
    /* data */
    map<string,map<string,vector<Event>>> MapOfReports;
    

public:
    summary();
    void add(Event &event,string user,string topic);
    // void print(vector <string> words);
    string getsummary(string game_name,string user);
    vector<string> split (string s, string delimiter) ;
    string tostring(map<string,string> &m);
    string printMap(map<string,string> m);
};
