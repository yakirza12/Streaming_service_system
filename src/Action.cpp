//
// Created by yakir zagron on 23/11/2019.
//

#include "../include/Action.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/User.h"
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;

// base action
BaseAction::BaseAction():errorMsg(""), status(PENDING){

}

ActionStatus BaseAction::getStatus() const {
    return status;
}
string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const string &errorMsg)
{   this->errorMsg=errorMsg;
    status = ERROR;

}



BaseAction::BaseAction(string &errorMsg, ActionStatus &status):
        errorMsg(errorMsg), status(status){}






//create user***** V

CreateUser::CreateUser(const string& name, const string& alg):
        BaseAction(), name(name), alg(alg)
{

}

void CreateUser::act(Session &sess) {
    if (sess.getUserMap().count(name) == 0 )//does not exist on the userMap
    {
        if (alg == "len")
        {
            LengthRecommenderUser* user1 = new LengthRecommenderUser(name);
            sess.getUserMap().insert({name,user1});
            complete();


        }
        else if (alg == "rer")
        {
            RerunRecommenderUser* user2 = new RerunRecommenderUser(name );
            sess.getUserMap().insert({name,user2});
            complete();
        }
        else if (alg == "gen")
        {
            GenreRecommenderUser* user3 = new GenreRecommenderUser(name);
            sess.getUserMap().insert({name,user3});
            complete();

        }
        else {
            error("recommendation algorithm is invalid");
            cout << "Error - " << this->getErrorMsg() << "\n";
        }
    }
    else {
        error("username is already in use");
        cout << "Error - " << this->getErrorMsg() << "\n";
    }
}

string CreateUser::toString() const {
    string sta = "CreateUser ";
    if(getStatus() == ERROR)
        return sta += "ERROR: " + getErrorMsg();
    return sta +=  "COMPLETED ";
}

BaseAction *CreateUser::clone() {
    return new CreateUser(*this);
}



//**********Change Active User**************V

void ChangeActiveUser::act(Session &sess) {
    if (sess.getUserMap().count(name) == 0) {
        error("username does not exist");
        cout << "Error - " << this->getErrorMsg() << "\n";
    }
    else {
        sess.SetActiveUser(name);
        complete();
    }
}

std::string ChangeActiveUser::toString() const {
    string sta = "ChangeActiveUser ";
    if(getStatus() == ERROR)
        return sta += "ERROR :" + getErrorMsg();
    return sta +=  "COMPLETED ";
}

ChangeActiveUser::ChangeActiveUser(string &name) :name(name)
{}

BaseAction *ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}

//*************delete user********** V
void DeleteUser::act(Session &sess) {
    if (sess.getUserMap().count(name) == 0) {
        error("Not Exist User to Delete");
        cout << "Error - " << this->getErrorMsg() << "\n";
    }
    else {
        delete sess.getUserMap().at(name);
        sess.getUserMap().erase(name);
        complete();
    }
}
string DeleteUser::toString() const {
    string sta =" DeleteUser";
    if(getStatus() == ERROR)
        return sta +=    "ERROR :" + getErrorMsg();
    return sta +=  "COMPLETED ";

}
DeleteUser::DeleteUser(string &name): name(name) {
}

BaseAction *DeleteUser::clone() {
    return new DeleteUser(*this);
}


//duplicate user*************V
void DuplicateUser::act(Session &sess) {

    if (sess.getUserMap().count(origName) == 0) {
        error("Not Exist User to Duplicate");
        cout << "Error - " << this->getErrorMsg() << "\n";
    }
    else if (sess.getUserMap().count(newName) != 0) {
        error("username is already in use");
        cout << "Error - " << this->getErrorMsg() << "\n";
    }
    else {
        string algType = sess.getUserMap().at(origName)->getAlg();
        vector<Watchable *> copyHis;

        for (size_t k = 0; k < sess.getUserMap().at(origName)->get_history().size(); ++k) {// end of copy vector
            copyHis.push_back(sess.getUserMap().at(origName)->get_history()[k]);
        }

                if (algType == "len") {
                    LengthRecommenderUser *user1 = new LengthRecommenderUser(newName);
                    user1->setTheHistory(copyHis);
                    sess.getUserMap().insert({newName, user1});
                    complete();
                }
                else if (algType == "rer") {
                    RerunRecommenderUser *user2 = new RerunRecommenderUser(newName);
                    user2->setTheHistory(copyHis);
                    sess.getUserMap().insert({newName, user2});
                    complete();

                }
                else if (algType == "gen") {
                    GenreRecommenderUser *user3 = new GenreRecommenderUser(newName);
                    user3->setTheHistory(copyHis);
                    sess.getUserMap().insert({newName, user3});
                    complete();
                }
            }
    }

string DuplicateUser::toString() const {
    string sta = "Duplicate ";
    if(getStatus() == ERROR)
        return sta += "ERROR: " + getErrorMsg();
    return sta +=  "COMPLETED ";
}

DuplicateUser::DuplicateUser(string origName, string newName):
        origName(origName),newName(newName)
{}

BaseAction *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}


//print contentlist******V
void PrintContentList::act(Session &sess) {
    sess.printContent();
    complete();
}

std::string PrintContentList::toString() const {
    string sta ="PrintContentList ";
    if(getStatus() == ERROR)
        return sta +=  "Error";
    return sta+= "COMPLETED"  ;
}

PrintContentList::PrintContentList() {}

BaseAction *PrintContentList::clone() {
    return new PrintContentList(*this);
}

// print watch history****V

PrintWatchHistory::PrintWatchHistory() {

}

void PrintWatchHistory::act(Session &sess) {
    cout << "Watch history for " << sess.getActiveUser()->getName() << "\n";
    for (size_t i = 0; i < sess.getActiveUser()->get_history().size(); i++)
        cout << i+1 << ". " << sess.getActiveUser()->get_history()[i]->toStringFixed() << "\n";
    complete();
   }



std::string PrintWatchHistory::toString() const {
    string s;
    return s += "COMPLETED";
}

BaseAction *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}

// *******************watch act***********************V /** need to check after algorhem

void Watch::act(Session &sess, Watchable *watch) { // from the next watch.
    cout<<"Watching " + watch->toStringFixed()<< "\n";
    sess.getActiveUser()->addWatch(watch);

    Watchable* watchN = sess.getActiveUser()->getRecommendation(sess);
    if (watchN== nullptr){
        error("no more content to watch");
        cout << "Error - " << this->getErrorMsg() << "\n";
        return;
    }
    string ans;
    cout << "We recommend watching " << watchN->toStringFixed() << ", continue watching? [y/n] " << "\n";
    cin >> ans;
    while (ans != "n" && ans !="y" ){
        cout << "Please enter valid answer - [y/n]";
        cin >> ans;
    }
    if (ans == "y"){
        Watch* watch2 = new Watch(id++); //id++
        watch2->act(sess,watchN);
        sess.getActionLog().push_back(watch2);
    }
}
void Watch::act(Session &sess) {
    Watchable* watch1 = sess.GetContentById(id);
    if(id<0 || id>= static_cast<long>(sess.getContent().size())) {
        error("not exist content on that Id");
        cout << "Error - " << this->getErrorMsg() << "\n";
        return;
    }
    cout<<"Watching " + watch1->toStringFixed()<< "\n";
    sess.getActiveUser()->addWatch(watch1);
    complete();

   Watchable* watchN = sess.getActiveUser()->getRecommendation(sess);
    if (watchN== nullptr){
        error("No more content to watch");
        cout << "Error - " << this->getErrorMsg() << "\n";
        return;
    }

    string ans;
    cout << "We recommend watching " << watchN->toStringFixed() << ", continue watching? [y/n] " << "\n";
    cin >> ans;
    while (ans != "n" && ans !="y" ){
        cout << "Please enter valid answer - [y/n]";
        cin >> ans;
    }
    if (ans == "y"){
        Watch* watch = new Watch(id++);
        watch->act(sess,watchN);
        sess.getActionLog().push_back(watch);
        }
    }


string Watch::toString() const {
    string sta  = "Watch ";
    if(getStatus() == ERROR)
        return sta +=  "Error: " + getErrorMsg();
    return sta+= "COMPLETED"  ;
}

Watch::Watch(long id):id(id) {
}

BaseAction *Watch::clone() {
    return new Watch(*this);
}


// print action log****V
void PrintActionsLog::act(Session &sess) {
    sess.printLog();
    complete();
}
std::string PrintActionsLog::toString() const {
    string s  = "PrintActionsLog ";
    return s += "COMPLETED";
}
PrintActionsLog::PrintActionsLog() {

}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

//exit*******V
void Exit::act(Session &sess) {
}
string Exit::toString() const {
    string sta ="Exit ";
    if(getStatus() == ERROR)
        sta+= "ERROR";
    sta += "COMPLETED";
    return sta;
}
Exit::Exit() {
}

BaseAction *Exit::clone() {
    return new Exit(*this);
}
