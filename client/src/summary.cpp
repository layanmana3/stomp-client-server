#include "../include/summary.h"
#include <vector>
summary::summary(){}



void summary::add(Event &event,string user,string topic){
    MapOfReports[user][topic].push_back(event);
}

string summary::getsummary(string game_name,string user){
    vector<Event> eve = MapOfReports[user][game_name]; 
    string body= "";
    string teamA = split(game_name,"_")[0];
    string teamB = split(game_name,"_")[1];
    string summ = teamA +" vs "+ teamB +"\n" + "Game stats:" + "\n" +"General stats:\n";
    map <string,string> General_stats;
    map <string,string> teamA_stats;
    map <string,string> teamB_stats;
    for(Event &e: eve){
        for(auto &pair: e.get_game_updates())
            General_stats[pair.first] = pair.second;
        for(auto &pair: e.get_team_a_updates())
            teamA_stats[pair.first] = pair.second;
        for(auto &pair: e.get_team_a_updates())
            teamB_stats[pair.first] = pair.second;
        body = body + to_string(e.get_time()) + " - " + e.get_name() + ":\n\n" + e.get_discription() + "\n\n\n";
    }
    string Generalstats = printMap(General_stats);
    string GeneralTeamA = printMap(teamA_stats);
    string GeneralTeamB = printMap(teamB_stats);
    summ =summ+ Generalstats+"\n"+ teamA + " stats: \n" + GeneralTeamA +"\n"+ teamB + " stats:\n "" "+ GeneralTeamB +"\n"+ body; 
    //cout<<" the summ"+ summ<<endl;
    return summ;    
}

string summary::printMap(map<string,string> m){
    string themap="\t";
    map<string,string>::iterator it = m.begin();
    for(it=m.begin();it!=m.end();it++){
        themap=themap+it->first +":"+it->second+" \n\t";
    }
    return themap;
}
// void summary::print(string topic , string user){
//     vector <string> headers = split(topic,"_");
//     string summary = headers[0] +"vs"+ headers[1] +"\n" + "Game stats:" + "\n" ;
//     string General_stats ="" ;   
// }
vector<string> summary::split (string s, string delimiter) {
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

