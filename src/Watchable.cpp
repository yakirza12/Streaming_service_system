
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/Action.h"
#include "../include/User.h"

using namespace std;
Watchable::Watchable(long id, int length, const vector<string> &tags):
        id(id), length(length),tags(tags){

}

Watchable::~Watchable() {
}



long Watchable::getId() const {
    return id;
}

int Watchable::getLength() const {
    return length;
}

const vector<string> &Watchable::getTags() const {
    return tags;
}




Movie::Movie(long id, const string &name, int length, const std::vector<std::string> &tags):
        Watchable(id,length,tags),name(name){

}



Watchable *Movie::getNextWatchable(Session& sess) const {

    return nullptr;
}

std::string Movie::toString() const {
    string tags = "[";
    for(size_t i=0 ; i<getTags().size();i++)
        if (i!=getTags().size()-1)
            tags += getTags()[i] + ", ";
        else
            tags+=getTags()[i];
    tags += "]" ;
    string str = to_string(getId()) + ". " + name + " " + to_string(getLength()) + " minutes " + tags;
    return str;
}

string Movie::toStringFixed() const {
    string str =   name + " ";
    return str;
}

string Movie::getName() const {
    return name;
}

Watchable *Movie::clone() {
    return new Movie(*this);
}


Episode::Episode(long id, const string &seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags):
        Watchable(id,length,tags), seriesName(seriesName), season(season), episode(episode),nextEpisodeId(id +1){

}



Watchable *Episode::getNextWatchable(Session &sess) const {
    if (nextEpisodeId == -1)
        return nullptr;
    else
        return sess.getContent()[nextEpisodeId-1];

}


string Episode::toString() const {
    string tags = "[";
    for(size_t i=0 ; i<getTags().size();i++)
        if (i!=getTags().size()-1)
            tags += getTags()[i] + ", ";
        else
            tags+=getTags()[i];
    tags += "]" ;


    string str = to_string(getId()) + ". " + seriesName + " S" +  to_string(season) + "E" + to_string(episode) +  " " + to_string(getLength()) + " minutes " + tags;
    return str;


}

void Episode::setNextEpisode(int id) {

    nextEpisodeId = id;

}

string Episode::toStringFixed() const {
    string str = seriesName + " S" +  to_string(season) + "E" + to_string(episode) +  " ";
    return str;
}


Watchable *Episode::clone() {
    return new Episode(*this);
}

