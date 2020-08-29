//
// Created by Giulio De Giorgi on 29/08/2020.
//

#include "PubSubHub.h"


void PubSubHub::publish(std::string topic, std::string data){
    for (auto s:this->subs) {
        if(s.getTopic()==topic) {
            s.putMsg(data);
            cv.notify_all();
        }
    }
}
void PubSubHub::subscribe(std::string topic){
    Subscription s(std::this_thread::get_id(), topic);
    subs.push_back(s);
}
bool PubSubHub::getData(std::string topic, std::string &data){
    bool found=false;
    std::unique_lock giulio(this->m);
    for (auto s:this->subs) {
        if(s.getId()==std::this_thread::get_id() && s.getTopic()==topic) {
            found=true;
            cv.wait(giulio, [&s](){
                return s.getMsg()!= "";
            });
            data=s.getMsg();
            std::cout<<data<<std::endl;
        }
        if(found) break;

    }
    return found;
}
void PubSubHub::unsubscribeAll(){
    for (auto s:this->subs){
        s.setTopic("");
    }
}