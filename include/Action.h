#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "User.h"
using namespace std;

class Session;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction()= default;
    BaseAction(string &errorMsg, ActionStatus &status);
    string getErrorMsg() const ;
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual string toString() const=0;
    virtual BaseAction* clone()=0;
protected:
    void complete();
    void error(const string& errorMsg);
private:
    string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    CreateUser(const string &name, const string &alg);
    virtual void act(Session& sess);
    virtual string toString() const;
    virtual BaseAction* clone();
private:
    string name;
    string alg;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(string& name);
    virtual void act(Session& sess);
    virtual string toString() const;
    virtual BaseAction* clone();
private:
    string name;
};

class DeleteUser : public BaseAction {
public:
    DeleteUser(string& name);
    virtual void act(Session & sess);
    virtual string toString() const;
    virtual BaseAction* clone();
private:
    string name;

};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(string origName, string newName);
    virtual void act(Session& sess);
    virtual string toString() const;
    virtual BaseAction* clone();
private:
    string origName;
    string newName;
};

class PrintContentList : public BaseAction {
public:
    PrintContentList();
    virtual void act (Session& sess);
    virtual string toString() const;
    virtual BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
    PrintWatchHistory();
    virtual void act (Session& sess);
    virtual string toString() const;
    virtual BaseAction* clone();
};


class Watch : public BaseAction {
public:
    Watch(long id);
    virtual void act(Session& sess);
    virtual void act(Session &sess,Watchable* watch);
    virtual string toString() const;
    virtual BaseAction* clone();
private:
    long id;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual void act(Session& sess);
    virtual string toString() const;
    virtual BaseAction* clone();
};

class Exit : public BaseAction {
public:
    Exit();
    virtual void act(Session& sess);
    virtual string toString() const;
    virtual BaseAction* clone();
};
#endif