#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <future>
#include <queue>
#include <map>

class Socket {
    int sockfd;
    Socket(const Socket&)=delete;
    Socket& operator=(const Socket&)=delete;

public:
    Socket(int sockfd): sockfd(sockfd){}
    Socket(){
        sockfd= ::socket(AF_INET, SOCK_STREAM, 0);
        std::cout <<"Creating socket "<<sockfd<<" @"<< this<<std::endl;

        if(sockfd<0)
            throw std::runtime_error(strerror(errno));
    }

    Socket (Socket&& other):sockfd(other.sockfd){
        other.sockfd = 0;
    }

    virtual ~Socket(){
        if(sockfd!=0) {
            ::close(sockfd);
            std::cout << "Destroying socket " << sockfd << " @" << this << std::endl;
        }

    }

    ssize_t read(char* buf, ssize_t len, int options){
        ssize_t res;
        res=::recv(sockfd, buf, len, options);
        if(res<0) {
            std::cout<<"Read error"<<std::endl;
            throw std::runtime_error(strerror(errno));
        }
        return res;
    }

    ssize_t write(char* buf, ssize_t len, int options){
        ssize_t res=::send(sockfd, buf, len, options);
        if(res<0) {
            std::cout<<"Read error"<<std::endl;
            throw std::runtime_error(strerror(errno));
        }
        return res;

    }

    friend class ServerSocket;
};

class ServerSocket: private Socket{
public:
    ServerSocket(u_int16_t portNumber) {
        struct sockaddr_in addr;
        addr.sin_family=AF_INET;
        addr.sin_port=htons(portNumber);
        addr.sin_addr.s_addr=htonl(INADDR_ANY);
        std::cout <<"Creating server socket "<<sockfd<<" @"<< this<<std::endl;


        if(::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr))!=0)
            throw std::runtime_error(strerror(errno));
        std::cout << "Binding..."<<std::endl;
        if(::listen(sockfd, 8)!=0)
            throw std::runtime_error(strerror(errno));
        std::cout << "Listening..."<<std::endl;

    }

    ~ServerSocket(){
        std::cout <<"Destroying server socket "<<sockfd<<" @"<< this<<std::endl;
        ::close(sockfd);}

    int accept(struct sockaddr_in* addr, socklen_t* len){
        std::cout<<"Accepting..."<<std::endl;
        int fd=::accept(sockfd, (struct sockaddr*) &addr, len);
        std::cout << "Accepted socket: "<<fd << "..."<<std::endl;
        if (fd<0)
            throw std::runtime_error(strerror(errno));
        return fd;
    }



};

void f(Socket s){
    char buf[1024];
    while(true) {

        ssize_t read_size=s.read(buf, sizeof(buf)-1,0);
        buf[read_size]='\0';
        std::cout<<"Client said: \n\t" << buf << "\t\tsize=" << read_size<< std::endl;
        std::string str(buf);

        ssize_t write_size=s.write(const_cast<char *>(str.c_str()), str.length(), 0);
        std::cout<<"Server replied: \n\t" << buf << "\t\tsize=" << write_size<< std::endl;
        if(strcmp(buf, "exit\n")==0) break;
    }
    std::cout<<"Exiting..."<<std::endl;
}

/*
class threadpool {
    std::vector<std::thread> v;
    std::queue<std::packaged_task<void(Socket)>> pool;
    std::mutex m;
    std::condition_variable cv;


    void submit(std::function<void(Socket)> f, Socket s){
        std::packaged_task<void(Socket)> p([&s](){
            char buf[1024];
            while(true) {

                ssize_t read_size=s.read(buf, sizeof(buf)-1,0);
                buf[read_size]='\0';
                std::cout<<"Client said: \n\t" << buf << "\t\tsize=" << read_size<< std::endl;
                std::string str(buf);

                ssize_t write_size=s.write(const_cast<char *>(str.c_str()), str.length(), 0);
                std::cout<<"Server replied: \n\t" << buf << "\t\tsize=" << write_size<< std::endl;
                if(strcmp(buf, "exit\n")==0) break;
            }
            std::cout<<"Exiting..."<<std::endl;

        });
        std::lock_guard l(m);
        pool.emplace(p);
    }

    void startloop(){
        for (int i=0; i<10; i++) {
            v.emplace_back((std::thread([this](){
                while(true){
                    std::unique_lock l(m);
                    cv.wait(l, [this](){return !this->pool.empty();});
                    std::packaged_task<void(Socket)> tmp(std::move(pool.front()));
                    pool.pop();
                    //tmp(Socket);

                }
            })));
        }
    }

};
*/

int main() {
    ServerSocket server(5000);
    int i;
    std::vector<std::thread> v;
    //std::map<Socket, std::thread> v;

    for(i=0; i<2;i++) {
        struct sockaddr_in addr;
        socklen_t len=sizeof(addr);
        int fd= server.accept(&addr, &len);

        //std::packaged_task<void(Socket)> p(f);

        /*v.insert({std::move(s), std::thread([&s](){
                                    char buf[1024];
                                    while(true) {

                                        ssize_t read_size=s.read(buf, sizeof(buf)-1,0);
                                        buf[read_size]='\0';
                                        std::cout<<"Client said: \n\t" << buf << "\t\tsize=" << read_size<< std::endl;
                                        std::string str(buf);

                                        ssize_t write_size=s.write(const_cast<char *>(str.c_str()), str.length(), 0);
                                        std::cout<<"Server replied: \n\t" << buf << "\t\tsize=" << write_size<< std::endl;
                                        if(strcmp(buf, "exit\n")==0) break;
                                    }
                                    std::cout<<"Exiting..."<<std::endl;
                                }
        )});*/


        v.push_back(std::thread([&fd](){
                                    char buf[1024];
                                    while(true) {

                                        Socket s(fd);
                                        ssize_t read_size=s.read(buf, sizeof(buf)-1,0);
                                        buf[read_size]='\0';
                                        std::cout<<"Client said: \n\t" << buf << "\t\tsize=" << read_size<< std::endl;
                                        std::string str(buf);

                                        ssize_t write_size=s.write(const_cast<char *>(str.c_str()), str.length(), 0);
                                        std::cout<<"Server replied: \n\t" << buf << "\t\tsize=" << write_size<< std::endl;
                                        if(strcmp(buf, "exit\n")==0) break;
                                    }
                                    std::cout<<"Exiting..."<<std::endl;
                                }
        )); // <---- con il threadpool sostitueremo questa cosa qui
        /*
         * 0 - i thread aspettano sulla coda
         * 1 - creazione packaged task con dentro f
         * 2 - aggiungo alla coda il pt appena creato (submit?)
         * 3 - un thread si sveglia e prende il packaged task
         * 4 - il thread che ha preso il pt - se ce n'è uno  che l'ha preso - inizia ad eseguire
         * 5 - finita l'esecuzione, si rimette in attesa sulla coda
         * */

    }


    for(i=0; i<2; i++) {
        if(v.at(i).joinable())
            v.at(i).join();
    }



    return 0;
}

