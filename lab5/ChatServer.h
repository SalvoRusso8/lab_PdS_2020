//
// Created by ubuntu on 20/08/20.
//
/*
#ifndef LAB5_CHATSERVER_H
#define LAB5_CHATSERVER_H


#include <mutex>
#include <map>
#include <memory>

class ClientConn;

using pClient = std::shared_ptr<ClientConn>;

class ChatServer{
    static std::mutex m;
    int sock = -1;
    bool running;
    std::map<int, pClient> clients; // mapping fd -> pClient

    void checkIdleClients();
public:

    ChatServer();
    int startListening(int port);
    void unregisterClient(int sock);
    pClient findClient(const std::string &nick);
    pClient findClient(int fd);
    void dispatchJoin(pClient c);
    void dispatchAll(std::string message);
    ~ChatServer();
};


#endif //LAB5_CHATSERVER_H
*/