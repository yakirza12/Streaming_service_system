//
// Created by yakir zagron on 18/11/2019.
//

#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>

using namespace std;

class Watchable;
class Session;

class User{
public:

    // Constructor
    User(const std::string& name);
    // Destructor
    virtual ~User();
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    void addWatch(Watchable* watched);
    virtual string getAlg() = 0;
    void setTheHistory(vector <Watchable*>);
    virtual User* clone() = 0;
    void copyHistory(Session& sess);
protected:
    std::vector<Watchable*> history;

private:
    const string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
   std::vector<Watchable*> get_NotSeenContent() const; //dvir, const?
   double averageWatched();
   bool isInHistory(Watchable *watch);
   virtual string getAlg();
    virtual User* clone();
private:
    std::vector <Watchable*> NotSeenContent;

};

class RerunRecommenderUser : public User {
public:

    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    int getCount() const;
    virtual string getAlg();
    virtual User* clone();
private:
    int count;
   // string name;


};

class GenreRecommenderUser : public User {
public:

    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
   // vector<string> getNotAvailableTag();
  //  vector< pair<int,string> > getMaxTag();
    bool isInHistory(Watchable *watch);
    bool isInMaxTag(string s);
    Watchable* ExistInCont(string str);
    virtual string getAlg();
    virtual User* clone();

private:
    std::vector <Watchable*> NotSeenContent;
  //  vector< pair<int,string> > maxTag;
    map<string,int> MaxTag;

};



#endif