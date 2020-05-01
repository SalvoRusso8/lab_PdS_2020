//
// Created by Salvo-fisso on 21/04/2020.
//

#include <string>
#include <iostream>
#include <cstring>
#include "Message.h"


#define debug true
long Message::id_univ=0;

Message::Message(int n) {
    //if (debug) std::cout<<"Message::constructor"<<std::endl;
    this->id=id_univ++;
    this->size=n;
    this->data=mkMessage(n);
}

Message::Message() {
    //if (debug) std::cout<<"Message::constructor no parameter"<<std::endl;
    this->id=-1;
    this->size=0;
    this->data= nullptr;
}

Message::Message(const Message& m) {
    //if (debug) std::cout<<"Message::copy constructor"<<std::endl;
    this->id=m.getId();
    this->size=m.getSize();
    this->data=new char[size+1];
    memcpy(this->data,m.getData(),m.getSize()+1);
}

 Message::Message( Message&& m) {
    //if (debug) std::cout<<"Message::move constructor"<<std::endl;
    this->id=m.getId();
    this->size=m.getSize();
    this->data=new char[size+1];
    this->data=m.getData();
    m.setData(nullptr);
}

Message::~Message() {
    //if (debug) std::cout<<"Message::distructor"<<std::endl;
    this->setId(-1);
    this->setSize(0);
    delete[] this->data;
}

long Message::getId() const {
    //if (debug) std::cout<<"Message::getId"<<std::endl;
    return id;
}

char *Message::getData() const {
    //if (debug) std::cout<<"Message::getData"<<std::endl;
    if(this->data== nullptr)
        return "";
    return data;
}

int Message::getSize() const {
    //if (debug) std::cout<<"Message::getSize"<<std::endl;
    return size;
}

void Message::setId(long id) {
    //if (debug) std::cout<<"Message::setId"<<std::endl;
    this->id = id;
}

void Message::setData(char *data) {
   // if (debug) std::cout<<"Message::setData"<<std::endl;
    this->data = data;
}

void Message::setSize(int size) {
    //if (debug) std::cout<<"Message::setSize"<<std::endl;
    this->size = size;
}

char* Message::mkMessage(int n) {
    //if (debug) std::cout<<"Message::mkMessage"<<std::endl;
    std::string vowels = "aeiou";
    std::string consonants = "bcdfghlmnpqrstvz";
    char* m = new char[n+1];
    for(int i=0; i<n; i++){
        m[i] = i%2 ? vowels[rand()%vowels.size()] :
                consonants[rand()%consonants.size()];
    }
    m[n] = 0;
    return m;
}

Message& Message::operator=(const Message &m) {
    //if (debug) std::cout<<"Message::operator= copy"<<std::endl;
    if(this!= &m){
        delete[] this->data;
        this->data=nullptr;
        this->id=m.getId();
        this->size=m.getSize();
        this->data=new char[size+1];
        memcpy(this->data,m.getData(),m.getSize()+1);
    }
    return *this;
}

Message& Message::operator=(Message &&m) {
    //if (debug) std::cout<<"Message::operator= move"<<std::endl;
    if(this!= &m){
        delete[] this->data;
        this->id=m.getId();
        this->size=m.getSize();
        this->data=m.getData();
        m.setSize(0);
        m.setData(nullptr);
    }
    return *this;
}



std::ostream& operator<<(std::ostream& out, const Message& m){

    out << "[id:" << m.getId() << "], [size:" << m.getSize() << "], [data:" << m.getData() << "]";
    return out;
}


























