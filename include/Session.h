
#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
using namespace std;
class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    Session( const Session &other);
    Session&operator=(const Session &other);
    Session(Session &&other);
    Session&operator=(Session &&other);
    ~Session();
    void start();
    unordered_map<std::string,User*>& getUserMap() ;
    vector<BaseAction*>& getActionLog();
    vector<Watchable*> getContent();
    void SetActiveUser(string name);
    User* getActiveUser() const;
    Watchable* GetContentById(long id);
    void printContent();
    void printLog();
    void clean();
   // void deleteUser();//????
private:

    vector<Watchable*> content;
    vector<BaseAction*> actionsLog;
    unordered_map<std::string,User*> userMap;
    User* activeUser;
    string comment;
    string firstCMD; //first command on Ucommend
    string secondCMD;//second command o Ucommend
    string thirdCMD;//third command on Ucommand
};

#endif
