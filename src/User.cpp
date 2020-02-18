
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

User::User(const std::string &name): history(), name(name) {

}

User::~User() {

}


std::string User::getName() const {
    return name;
}

vector<Watchable *> User::get_history() const {
    return history;
}

void User::addWatch(Watchable *watched) {
    this->history.push_back(watched);
}

void User::setTheHistory(vector<Watchable *> hist) {
    this->history=hist;

}

void User::copyHistory(Session &sess) {//made for session copy constructor.
for(size_t j= 0;j<history.size();j++){
    history[j]=sess.getContent()[history[j]->getId()-1];
}

}




//***********Len Rec********// V
LengthRecommenderUser::LengthRecommenderUser(const string &name) : User(name)
,NotSeenContent()
{}

Watchable *LengthRecommenderUser::getRecommendation(Session &s) {
    int size = s.getActiveUser()->get_history().size() - 1;
    if (s.getActiveUser()->get_history()[size]->getNextWatchable(s) != nullptr)
        return s.getActiveUser()->get_history()[size]->getNextWatchable(s);
    else {


            for (size_t i = 0; i < s.getContent().size(); i++) {//initilizing the notSeenContent venctor
                if (!isInHistory(s.getContent()[i])) //if not exist on history copy it from content.
                    NotSeenContent.push_back(s.getContent()[i]);
            }
         //   cout<<to_string(averageWatched())<<"\n";
            double average = averageWatched();
            Watchable *content = nullptr;
          //  cout<<to_string(NotSeenContent.size())<<"\n";
            if(NotSeenContent.size()==0)
                return nullptr;
          //  double comparison = abs(NotSeenContent[0]->getLength() - average);
             double comparison = 100000000000;
            for (size_t k = 0; k < NotSeenContent.size(); k++) {
                if (labs(NotSeenContent[k]->getLength() - average) < comparison) {
                    comparison = labs(NotSeenContent[k]->getLength() - average);
                    content = NotSeenContent[k];
                }
            }
                NotSeenContent.clear();
                return content;


         }

    }


std::vector<Watchable *> LengthRecommenderUser::get_NotSeenContent() const {
    return NotSeenContent;
}



double LengthRecommenderUser::averageWatched() {
    double sum=0;
    double size  =  get_history().size();
    for (size_t i = 0; i < get_history().size(); i++)
    {
        sum += get_history()[i]->getLength();
    }

    return sum/size ;
}

bool LengthRecommenderUser::isInHistory(Watchable *watch) {
    bool ex = false;
    for(size_t k=0;k<get_history().size(); k++)
        if (watch->getId()==get_history()[k]->getId())
            ex = true;
    return ex;
}

string LengthRecommenderUser::getAlg() {
    return "len";
}

User *LengthRecommenderUser::clone() {
    return new LengthRecommenderUser(*this);
}



//***********Rerun Rec********//
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name)
, count()
{
    count = 0;
}



Watchable *RerunRecommenderUser::getRecommendation(Session &s) {
    int size = s.getActiveUser()->get_history().size() - 1;
    if (s.getActiveUser()->get_history()[size]->getNextWatchable(s) != nullptr)
        return s.getActiveUser()->get_history()[size]->getNextWatchable(s);
    else {

        int copy = count;
        count++;
        return (get_history()[(copy) % get_history().size()]);
     }
}


string RerunRecommenderUser::getAlg() {
    return "rer";
}

User *RerunRecommenderUser::clone() {
    return new RerunRecommenderUser(*this);
}

//***********genre Rec********//
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name),NotSeenContent(), MaxTag()

{}





Watchable *GenreRecommenderUser::getRecommendation(Session &s) {
    int size = s.getActiveUser()->get_history().size() - 1;
    if (s.getActiveUser()->get_history()[size]->getNextWatchable(s) != nullptr)
        return s.getActiveUser()->get_history()[size]->getNextWatchable(s);

    else {
        for (size_t i = 0; i < s.getContent().size(); i++) {//initilizing the notSeenContent venctor
            if (!isInHistory(s.getContent()[i])) //if not exist on history copy it from content.
                NotSeenContent.push_back(s.getContent()[i]);
        }

        for (size_t i = 0; i < get_history().size(); i++) {// create the usermap
            for (size_t j = 0; j < history[i]->getTags().size(); j++) {
                if (MaxTag.count(history[i]->getTags()[j]) == 0)// if not exists on the map.
                    MaxTag.insert({history[i]->getTags()[j], 1});
                else
                    MaxTag.find(history[i]->getTags()[j])->second += 1;// add counter+=1 if tag exists.
            }

        }

        string max = MaxTag.begin()->first;

        Watchable *content = nullptr;
        content = ExistInCont(MaxTag.begin()->first);
        while (MaxTag.size() != 0) {//finish all tags.
            for (auto &it: MaxTag) {
                if (MaxTag.size() == 1) {//only 1 tag in the max tags, return the first content with that tag.
                    content = ExistInCont(MaxTag.begin()->first);
                    NotSeenContent.clear();
                    MaxTag.clear();
                    return content;// same if he is nullptr.
                }
                if (MaxTag.find(max)->second < it.second)
                    max = it.first;
            }

               //cout<<"max =" <<max<<"\n";//deleteafter
            content = ExistInCont(max);

            if (content != nullptr) {//we have content with that tag on nos// eencontet
                NotSeenContent.clear();
                MaxTag.clear();
                return content;
            } else {
             //   cout<<"max =" <<max<<"\n";//print if null
                MaxTag.erase(max);//remove this tag from the map, we dont have any content from it.
                max = MaxTag.begin()->first;
               // cout<<"max =" <<max<<"\n";//check the new
            }
        }
        NotSeenContent.clear();
        MaxTag.clear();
        return nullptr;
    }

}




Watchable *GenreRecommenderUser::ExistInCont(string str) {
    for(size_t v=0;v<NotSeenContent.size();v++) {
        for (size_t k = 0; k < NotSeenContent[v]->getTags().size(); k++)
            if (str == NotSeenContent[v]->getTags()[k])
                return NotSeenContent[v];
     }
    return nullptr;
}



bool GenreRecommenderUser::isInHistory(Watchable *watch) {
    bool ex = false;
    for(size_t k=0;k<get_history().size(); k++)
        if (watch->getId()==get_history()[k]->getId())
            ex = true;
    return ex;
}


bool GenreRecommenderUser::isInMaxTag(string s) {
    bool ex = false;
    for (auto &it: MaxTag) {
        if (s == it.first)
            ex = true;
        return ex;
    }
return ex;
}

string GenreRecommenderUser::getAlg() {
    return "gen";
}

User *GenreRecommenderUser::clone() {
    return new GenreRecommenderUser(*this) ;
}
