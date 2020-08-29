//
// Created by Giulio De Giorgi on 29/08/2020.
//

#ifndef PUBSUBPARADIGM_PUBSUBHUB_H
#define PUBSUBPARADIGM_PUBSUBHUB_H
#include <stdio.h>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include "Subscription.h"


class PubSubHub {

    std::vector<Subscription> subs;
public:
    void publish(std::string topic, std::string data);
    void subscribe(std::string topic);
    bool getData(std::string topic, std::string &data);
    void unsubscribeAll();
    PubSubHub(){

    };


private:
    std::condition_variable cv;
    std::mutex m;

};


#endif //PUBSUBPARADIGM_PUBSUBHUB_H
