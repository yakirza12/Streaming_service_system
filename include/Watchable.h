#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
using namespace std;

class Session;

class Watchable{
public:
   // Watchable();
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual string toString() const = 0;
    virtual Watchable* getNextWatchable(Session& sess) const = 0;
    long getId() const;
    int getLength() const;
    const vector<string>& getTags() const;
    virtual string toStringFixed() const =0;
    virtual Watchable* clone() = 0;
private:
    const long id;
    int length;
    vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const string& name, int length, const std::vector<std::string>& tags);
    virtual string toString() const;
    virtual string toStringFixed() const;
    virtual Watchable* getNextWatchable(Session&) const;
    virtual string getName() const;
    virtual Watchable* clone();

private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual string toString() const;
    virtual string toStringFixed() const;
    virtual Watchable* getNextWatchable(Session&) const; // צריך לממש
    void setNextEpisode(int i);
    virtual Watchable* clone();
private:
    string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif