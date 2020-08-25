//
// Created by ubuntu on 20/08/20.
//
/*
#ifndef LAB5_CLIENTCONN_H
#define LAB5_CLIENTCONN_H


#include <mutex>
#include <condition_variable>
#include <list>
#include "ChatServer.h"

enum ClientStatus {
    starting, active, terminating
};

class ClientConn {

    int sock;
    static std::mutex m; // messages mutex
    static std::condition_variable new_messages;

    std::string nick;
    std::list<std::string> messages;
    ClientStatus status;
    std::time_t touch;

    ChatServer *server;

    ClientConn(const ClientConn&) = delete;
    ClientConn& operator=(const ClientConn&) = delete;
    ClientConn(int sock, ChatServer *server): sock(sock), server(server), status(starting) {};

    std::pair<std::string, std::string> parseLine(std::string line){
        // empy line
        if(line.length() == 0) {
            return std::pair("", "");
        }

        if(line[0] == '/') {
            int idx = line.find(' ');
            return std::pair(line.substr(0, idx), line.substr(idx+1));
        } else {
            return std::pair("", line);
        }
    }


    int send(std::string msg){
        touch = std::time(nullptr);
        return ::send(sock, (msg+"\n").c_str(), msg.length()+1, 0);
    };

    ostring readline(){
        std::string buf;
        char c;
        while( recv(sock, &c, 1, 0) > 0){
            if(c == '\n') {
                return buf;
            } else {
                if (c!='\r')
                    buf += c;
            }
        }
        touch = std::time(nullptr);
        return ostring();
    }

    void close() {
        this->status = terminating;
        this->server->unregisterClient(this->sock);
        // after the dispatch all the connections are awakened
        this->server->dispatchAll(this->nick + " has left");
        // shutdown closes both read and write streams, even if they are blocked
        ::shutdown(sock, SHUT_RDWR);
    }

    void handleConnection(){

        std::thread inboundChannel([this](){
            // this keeps the ChatClient alive until the we dont't exit the thread
            // even if it's removed from the clients map (we "abuse" of RAII)
            pClient me = server->findClient(this->sock);

            // read and parse helo
            ostring line = this->readline();

            if(!line){
                mylog("empty line, closing client in");
                close();
                return;
            }
            std::pair helo = parseLine(*line);

            // check if nicks is taken
            if(helo.first == "/helo"  && server->findClient(helo.second) == nullptr){
                this->nick = helo.second;
                send("200");
                this->status = active;
            } else {
                // nick taken or other error
                mylog("nick taken, closing client in");
                send("400");
                close();
                return;
            }

            mylog("starting client for\n  ");
            mylog(nick);
            // helo ok, send participants
            server->dispatchJoin(me);

            // start reading incoming messages
            while(true){
                line = this->readline();

                // the socket is closed?
                if(!line.has_value()){
                    close();
                    break;
                }
                std::pair toks = parseLine(*line);
                if(toks.first != "" && toks.first[0] == '/'){
                    if(toks.first == "/private"){
                        int idx = toks.second.find(' ');
                        if(idx>0){
                            std::string nick = toks.second.substr(0, idx);
                            std::string msg = toks.second.substr(idx);

                            pClient target = this->server->findClient(nick);
                            if(target != nullptr){
                                std::string outmsg("from ");
                                outmsg += this->nick + ": " + msg;
                                target->messages.push_back(outmsg);
                                new_messages.notify_all();
                            } else
                                mylog(std::string(nick)+" not found");
                        }
                    } else if(toks.first == "/quit") {
                        close();
                        break;
                    }
                } else {
                    server->dispatchAll((this->nick + ": ") + *line);
                }

            }

            std::string msg("receiver exited: ");
            msg += this->nick;
            mylog(msg);
        });

        inboundChannel.detach();

        std::thread outbounChannel([this](){

            // this keeps the object alive until the we dont't exit the thread
            pClient me = server->findClient(this->sock);

            std::unique_lock lg(m);
            while(true){
                this->new_messages.wait(lg, [this](){
                    return (messages.size()>0 && status == active) || status == terminating  ;
                });

                if(status == terminating) {
                    break;
                }

                for(auto msg: messages){
                    this->send(msg);
                }
                messages.erase(messages.begin(), messages.end());
            }

            std::string msg("sender exited: ");
            msg += nick;
            mylog(msg);
        });

        outbounChannel.detach();

    }

public:
    ~ClientConn() {
        std::string msg("deleting client: ");
        msg += nick;
        mylog(msg);
        ::close(sock);
    }

    // only the ChatServer can use this class
    friend class ChatServer;

};


#endif //LAB5_CLIENTCONN_H
*/