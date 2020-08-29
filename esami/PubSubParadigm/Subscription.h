//
// Created by Giulio De Giorgi on 29/08/2020.
//

#ifndef PUBSUBPARADIGM_SUBSCRIPTION_H
#define PUBSUBPARADIGM_SUBSCRIPTION_H
#include <string>
#include <queue>
#include <thread>
#include <map>

class Subscription {

    std::string topic;
    std::queue<std::string> msg;
    std::thread::id id;

public:
    Subscription(std::thread::id id, std::string topic):id(id), topic(topic) {
    };

    void putMsg(std::string &msg) {
        this->msg.push(msg);
    }
    std::string getMsg(){
        if(msg.empty()) return "";
        std::string tmp = this->msg.front();
        msg.pop();
        return tmp;
    }

    std::string getTopic(){
        return this->topic;
    }
    void setTopic(std::string topic) {
        this->topic=topic;
    }


    std::thread::id getId(){
        return this->id;
    }


};
#endif //PUBSUBPARADIGM_SUBSCRIPTION_H
