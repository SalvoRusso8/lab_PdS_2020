//
// Created by Salvo-fisso on 22/04/2020.
//

#include <cstring>
#include <iostream>
#include "MessageStore.h"
#include "DurationLogger.h"
#define debug true

MessageStore::MessageStore(int n) {
    messages = new Message[n];
    dim=n;
    this->n = n;
}

MessageStore::~MessageStore() {
    this->dim=0;
    this->n=0;
    messages=nullptr;
}
void MessageStore::add(Message &m) {
    if (debug) std::cout<<"MessageStore::add"<<std::endl;
    bool trovato=false;
    for(int i=0;i<dim && !trovato;i++){
        if(messages[i].getId()==-1 /*|| messages[i].getId()==m.getId()*/){
            if (debug) std::cout<<"MessageStore::add->trovato"<<std::endl;
            messages[i]=m;
            trovato=true;
        }
    }
    if(!trovato){
        if (debug) std::cout<<"MessageStore::add->non trovato"<<std::endl;
        Message *messages2 = new Message[dim+n];
        {
            DurationLogger dl1 = DurationLogger("clock1()");
            for (int i = 0; i < dim; i++)
                messages2[i] = messages[i];
        }
        {
            DurationLogger dl2 = DurationLogger("clock2()");
            for (int i = 0; i < dim; i++)
                messages2[i] = std::move(messages[i]);
        }
        messages=messages2;
        messages[dim]=m;
        dim=dim+n;

    }
}

std::experimental::optional<Message> MessageStore::get (long id){
    if (debug) std::cout<<"MessageStore::get"<<std::endl;
    for (int i=0;i<dim;i++){
        if(messages[i].getId()==id)
            return messages[i];
    }
    return std::experimental::nullopt;
}

bool MessageStore::remove(long id){
    if (debug) std::cout<<"MessageStore::remove"<<std::endl;
    for(int i=0;i<dim;i++){
        if(messages[i].getId()==id) {
            messages[i].~Message();
            return true;
        }
    }
    return false;
}

std::tuple<int, int> MessageStore::stats() {
   // if (debug) std::cout<<"MessageStore::stats() vedo dim "<< dim << "abbondoooooo" << dim << n << std::endl;
    int valid=0, empty=0;
    for(int i=0;i<dim;i++){
        if(messages[i].getId()==-1)
            empty++;
        else
            valid++;
    }
    return std::make_tuple(valid,empty);
}

void MessageStore::compact() {
    if (debug) std::cout<<"MessageStore::compact()"<<std::endl;
    int valid = std::get<0>(this->stats());
    int dim_old=dim, j=0;
    while(dim>=valid+n)
        dim-=n;
    Message *messages2 = new Message[dim];
    {
        DurationLogger dl1 = DurationLogger("clock1()");
        for (int i = 0; i < dim_old; i++) {
            if(messages[i].getId()!=-1)
                //messages2[j++] = messages[i];
                messages2[j++] = std::move(messages[i]);
        }
    }
    messages=messages2;

}

int MessageStore::getDim(){
    return dim;
}

Message* MessageStore::getMessages(){
    return messages;
}