
#include <string>
#include<fstream>
#include<vector>
#include <unordered_map>
#include "../include/Session.h"
#include "../include/Watchable.h"
#include "../include/json.hpp"
#include "../include/Action.h"
#include "../include/User.h"

using namespace std;


Session::Session(const std::string &configFilePath):
content(), actionsLog(),userMap(),activeUser(),comment(""),firstCMD(""),secondCMD(""),
thirdCMD("")
{

    //creat the default user and make him activeUser.
    User* def = new LengthRecommenderUser("default");
    userMap.insert({"default",def});
    activeUser = def;

    //Read the Json file to the content vector
    using json = nlohmann::json;
    std::ifstream j(configFilePath);
    json filej;
    j >> filej;
    int id = 0;

    for (size_t i = 0; i < filej["movies"].size(); i++) {// size() = how much block in movies
        string name1 = filej["movies"][i]["name"].dump();
        name1 = name1.substr(1, name1.size() - 2);
        int length = stoi(filej["movies"][i]["length"].dump());
        vector<string> tags1;
        for (size_t k = 0; k < filej["movies"][i]["tags"].size(); k++) {
            string str2 = filej["movies"][i]["tags"][k].dump();
            str2 = str2.substr(1,str2.size()-2);
           // tags1.push_back(filej["movies"][i]["tags"][k].dump());
            tags1.push_back(str2);
        }
        id++;
        content.push_back(new Movie(id, name1, length, tags1));
    }

    for (size_t i = 0; i < filej["tv_series"].size(); i++) {// size() = how much block in tv se
        string name2 = filej["tv_series"][i]["name"].dump();
        name2 = name2.substr(1, name2.size() - 2);
        int length1 = stoi(filej["tv_series"][i]["episode_length"].dump());
        vector<string> tags2;
        for (size_t k = 0; k < filej["tv_series"][i]["tags"].size(); k++) {
            string str1 = filej["tv_series"][i]["tags"][k].dump();
            str1 = str1.substr(1,str1.size()-2);
            //tags2.push_back(filej["tv_series"][i]["tags"][k].dump());
            tags2.push_back(str1);
        }

        int numOfSeasons = filej["tv_series"][i]["seasons"].size();
        int numOfEpisods = 0;
        for (int j = 0; j < numOfSeasons; j++) {
            numOfEpisods = stoi(filej["tv_series"][i]["seasons"][j].dump());
            for (int k = 0; k < numOfEpisods; k++) {
                id++;
                Episode *Epi = new Episode(id, name2, length1, j + 1, k + 1, tags2);
                if (j + 1 == numOfSeasons &&
                    k + 1 == numOfEpisods) {// how we can know that it is the last episode on the last session.
                    Epi->setNextEpisode(-1);
                }
                content.push_back(Epi);
            }
        }


    }
}

Session::~Session(){
clean();
}


//********Copy Constructor******
Session::Session(const Session &others):
        content(), actionsLog(),userMap(),activeUser(),
        comment(others.comment),firstCMD(others.firstCMD),secondCMD(others.secondCMD),
        thirdCMD(others.thirdCMD) {
    for (size_t i = 0; i < others.content.size(); i++) {
        this->content.push_back(others.content[i]->clone());
    }

    for (size_t j = 0; j < others.actionsLog.size(); j++) {
        this->actionsLog.push_back(others.actionsLog[j]->clone());
    }

    for (auto &umo: others.userMap) {
        pair<string, User *> p = {umo.first, umo.second->clone()};
        p.second->copyHistory(*this);
        this->userMap.insert(p);
    }
    this->activeUser = userMap.at(others.activeUser->getName());


}




//********Copy Assignment Operator*******
Session &Session::operator=(const Session &others) {
    if (this == &others) {
        return *this;
    }
    clean();
    for (size_t i = 0; i < others.content.size(); i++) {
        this->content.push_back(others.content[i]->clone());
    }
    for (size_t j = 0; j < others.actionsLog.size(); j++) {
        this->actionsLog.push_back(others.actionsLog[j]->clone());
    }
    for (auto &umo: others.userMap) {
        pair<string, User *> p = {umo.first, umo.second->clone()};
        p.second->copyHistory(*this);
        this->userMap.insert(p);
    }
    cout<<others.activeUser->getName()<<"/n";
    this->activeUser = userMap.at(others.activeUser->getName());
    return *this;


}
//*********Move Constructor******
Session::Session(Session &&others):
        content(), actionsLog(),userMap(),activeUser(),
        comment(others.comment),firstCMD(others.firstCMD),secondCMD(others.secondCMD),
        thirdCMD(others.thirdCMD)
        {
    for (size_t i = 0; i < others.content.size(); i++) {
        this->content.push_back(others.content[i]);
    }
    for (size_t j = 0; j < others.actionsLog.size(); j++) {
        this->actionsLog.push_back(others.actionsLog[j]);
    }
    for (auto &umo: others.userMap) {
        pair<string, User *> p = {umo.first, umo.second};
        this->userMap.insert(p);
    }
    activeUser=others.activeUser;
    others.activeUser = nullptr;
    others.userMap.clear();
    others.actionsLog.clear();
    others.content.clear();
}

//********Move Assignment Operator****
Session &Session::operator=(Session &&others) {
    if (this == &others) {
        return *this;
    }
    clean();
    for (size_t i = 0; i < others.content.size(); i++) {
        this->content.push_back(others.content[i]);
    }
    for (size_t j = 0; j < others.actionsLog.size(); j++) {
        this->actionsLog.push_back(others.actionsLog[j]);
    }
    for (auto &umo: others.userMap) {
        pair<string, User *> p = {umo.first, umo.second};
        this->userMap.insert(p);
    }
    activeUser=others.activeUser;
    others.activeUser = nullptr;
    others.userMap.clear();
    others.actionsLog.clear();
    others.content.clear();

    return *this;

}




void Session::start() {
    cout << "SPLFLIX is now on!\n";
    bool NotExitYet=true;
    while(NotExitYet) {
        getline(cin, comment); ///scan the commend from the User.
        int pos1 = comment.find(" ");
        firstCMD = comment.substr(0, pos1 );// the first commend between the char 0 to the first " "
        comment = comment.substr(pos1 + 1);
        int pos2 = comment.find(" ");
        secondCMD = comment.substr(0, pos2 );
        thirdCMD = comment.substr(pos2 + 1);


        //Crete user*****V
        if (firstCMD == "createuser") {
            CreateUser* createuser = new CreateUser(secondCMD,thirdCMD);
            createuser->CreateUser::act(*this);//this session
            actionsLog.push_back(createuser);

        }
            //Change Active User*****V
        else if (firstCMD == "changeuser") {
                ChangeActiveUser* change = new ChangeActiveUser(secondCMD);
                change->act(*this);
                actionsLog.push_back(change);
        }

            //Delete User*******V
        else if (firstCMD == "deleteuser") {
                DeleteUser* deleted = new DeleteUser(secondCMD);
                deleted->act(*this);
                actionsLog.push_back(deleted);
        }
            //Print  Content List***V
        else if (firstCMD == "content") {
            PrintContentList *content1 = new PrintContentList();
            content1->act(*this);
            actionsLog.push_back(content1);
        }
            //Print ActionLog*****V
        else if( firstCMD == "log") {
            PrintActionsLog *logger = new PrintActionsLog();
            logger->act(*this);
            actionsLog.push_back(logger);
        }

            //Print Watch history****V
        else if (firstCMD == "watchhist") {
           PrintWatchHistory* print1 = new PrintWatchHistory();
            print1->act(*this);
            actionsLog.push_back(print1);

        }
            //Duplicate User*****V
        else if (firstCMD == "dupuser") {
            DuplicateUser* dupe = new DuplicateUser(secondCMD,thirdCMD);
            dupe->act(*this);
            actionsLog.push_back(dupe);
        }
            //Watch******V
        else if (firstCMD == "watch") {
            long id = stol(secondCMD);
            Watch* watch = new Watch(id);
            watch->act(*this);
            actionsLog.push_back(watch);
        }

        else if (firstCMD == "exit"){
            NotExitYet=false;
            Exit *ex = new Exit();
            ex->act(*this);
            actionsLog.push_back(ex);
        }

    }
}

unordered_map<string ,User*> &Session::getUserMap() {
    return userMap;
}

vector<BaseAction *> &Session::getActionLog() {
    return actionsLog;
}

vector<Watchable *> Session::getContent() {
    return content;
}

void Session::SetActiveUser(string name) {
    activeUser = userMap.at(name);
}

User *Session::getActiveUser() const {
    return activeUser;
}



Watchable *Session::GetContentById(long id) {// the content with id i can be at index i- to 0 (if we are remove element from the vector.
    long idVec = id - 1;
    size_t i = id;
    while(idVec > -1 && i<content.size()){
        if (content[idVec]->getId() == id)
            return content[idVec];
        idVec--;
    }
    return nullptr;
}

void Session::printContent() {
    for(size_t s =0; s<content.size();s++)
        cout<<content[s]->toString()+"\n";
}

void Session::printLog() {
   for(size_t i =0 ;i<actionsLog.size();i++)
       cout<<actionsLog[i]->toString()<<"\n";

}

void Session::clean() {
    for(size_t j = 0;j<content.size();j++)
    {
        if(content[j]!= nullptr)
            delete content[j];
    }

    for(auto it =  userMap.begin();it != userMap.end();it++){
        if (it->second!= nullptr)
             delete(it->second);
    }

    for(size_t k=0;k<actionsLog.size();k++){
        delete actionsLog[k];
    }


}







