//
// Created by Salvo-fisso on 22/04/2020.
//

#ifndef LAB1_MESSAGESTORE_H
#define LAB1_MESSAGESTORE_H


#include <tuple>
#include <functional>
#include <experimental/optional>
#include "Message.h"

class MessageStore {
    Message *messages;
    int dim;
    int n;

public:
    MessageStore(int n);
    ~MessageStore();
    void add(Message &m);
    std::experimental::optional<Message> get(long id);
    bool remove(long id);
    std::tuple<int,int> stats();
    void compact();
    int getDim();
    Message* getMessages();



};


#endif //LAB1_MESSAGESTORE_H
