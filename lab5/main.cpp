//
// Created by ff on 6/13/20.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <list>
#include <functional>
#include <optional>
#include <shared_mutex>
#include <map>
#include <string.h>
#include <sstream>
#include <vector>

#define IDLE_TIMEOUT 60

// chat server composed by a main ChatServer class with there responsibilities:
// - handling incoming connections and spawning ChatClient handlers
// - keeping tracks of opened clients
// - dispatching messages between clients
// - checking idle timeouts

// and a ChatClient class:
// - handling in separate threads incoming e outgoing connections
// - parsing messages and commands, as soon as the connection is established all the communication with the client
//   is handled by the ChatClient
// - there is also a sender thread synchronized with a per client message queue to avoid deadlocks from slow clients
//   (a better implementation could use the select )
// - correctly shutting down the connection

// requires a POSIX environment



using ostring = std::optional<std::string>;
class ClientConn;
using pClient = std::shared_ptr<ClientConn>;

enum ClientStatus {
    starting, active, terminating
};

std::mutex mout;
void mylog(std::string msg){
    std::lock_guard lg(mout);
    std::cout<<msg<<std::endl;
}

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

std::mutex ChatServer::m;

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

std::mutex ClientConn::m;
std::condition_variable ClientConn::new_messages;


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
}


int main() {
    ChatServer server;
    server.startListening(1234);
}

