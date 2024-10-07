#pragma once
#include <algorithm>
#include <vector>
using std::vector;

class Observer {
public:
    virtual void update() = 0;
};

class Observable {
    vector<Observer*> interested;
protected:
    void notify() const {
        for (auto obs : interested)
            obs->update();
    }
public:
    void addObserver(Observer* obs) {
        interested.push_back(obs);
    }
    void removeObserver(Observer* obs) {
        interested.erase(remove(interested.begin(), interested.end(), obs));
    }
};