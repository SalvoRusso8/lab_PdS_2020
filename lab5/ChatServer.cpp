//
// Created by ubuntu on 20/08/20.
//
/*
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include "ChatServer.h"

ChatServer::ChatServer(){
    if((sock=::socket(AF_INET, SOCK_STREAM, 0))==-1){
        throw std::runtime_error(strerror(errno));
    }
};

int ChatServer::startListening(int port){

    running = true;
    int opt = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt))<0){
        mylog("sockopt error");
        mylog(strerror(errno));
        return -1;
    }

    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr *)&saddr, sizeof(saddr))<0){
        mylog("bind error");
        mylog(strerror(errno));
        return -1;
    }

    if(::listen(sock, 10)<0){
        mylog("listen error");
        mylog(strerror(errno));
        return -1;
    }

    // start idle clients loop
    checkIdleClients();

    // we and the server with SIGINT
    while(running) {
        sockaddr_in caddr;
        socklen_t addrlen = sizeof(caddr);

        int csock = accept(sock, (struct sockaddr*) &caddr, &addrlen);
        if(csock<0){
            mylog("accept error");
            mylog(strerror(errno));
        } else {
            char buff[8];
            //mylog(inet_ntop(AF_INET,(struct sockaddr*) &caddr, buff,8));
            std::lock_guard lg(m);
            pClient client = pClient(new ClientConn(csock, this));
            // this keeps the client alive until it's destroyed
            clients[csock] = client;

            // handle connection should return immediately
            client->handleConnection();
        }

    }
    return 0;
}

// it's save to destroy the client more than once (the method is idempotent)
void ChatServer::unregisterClient(int csock){
    std::lock_guard lg(m);
    clients.erase(csock);
}

pClient ChatServer::findClient(const std::string &nick){
    std::lock_guard lg(m);
    for(auto it: clients){
        if(it.second->nick == nick) {
            return it.second;
        }
    }
    return nullptr;
}

pClient ChatServer::findClient(int fd) {
    return clients[fd];
}

void  ChatServer::dispatchJoin(pClient c) {

    std::scoped_lock lg(m, ClientConn::m);
    std::ostringstream oss;
    oss<<"participans: ";

    // copy the message to all clients
    for(auto [k, client]: clients){
        if(client->status == active && client != c){
            client->messages.push_back(std::string("joined: ") + c->nick);
            oss<<client->nick<<" ";
        }
    }
    c->messages.push_back(oss.str());
    ClientConn::new_messages.notify_all();
}

void ChatServer::dispatchAll(std::string message){
    std::scoped_lock lg(m, ClientConn::m);

    // copy the message to all clients
    for(auto [k, client]: clients){
        if(client->status == active){
            client->messages.push_back(message);
        }
    }
    ClientConn::new_messages.notify_all();
}

void  ChatServer::checkIdleClients() {

    // we terminate the server with SIGINT
    std::thread idleClients([this](){
        while(running){
            // che idle client each seconds
            sleep(1);

            std::time_t now = time(nullptr);
            // store terminating clients and after terminate them to avoid a deadlock
            std::vector<pClient> terminating;
            {
                std::lock_guard lg(m);
                for(auto [k, c]: clients){
                    if (c->status == active && now-c->touch > IDLE_TIMEOUT){
                        terminating.push_back(c);
                    }
                }
            }
            for(auto c: terminating)
                c->close();

        }
    });
    idleClients.detach();

}

ChatServer::~ChatServer(){
    if(sock!=-1)
        close(sock);
}*/