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
    std::mutex m;

public:
    Socket(int sockfd): sockfd(sockfd){}
    Socket(){
        sockfd= ::socket(AF_INET, SOCK_STREAM, 0);
        std::cout <<"Creating socket "<<sockfd<<" @"<< this<<std::endl;

        if(sockfd<0)
            throw std::runtime_error(strerror(errno));
    }

    Socket (Socket&& other):sockfd(other.sockfd){
        std::lock_guard l(m);
        other.sockfd = 0;
    }

    virtual ~Socket(){
        std::unique_lock l(m);
        std::cout << "Sono il distruttore -> "<<sockfd<<"@"<<this<<std::endl;
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
    int getfd(){ return sockfd;}

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
        std::unique_lock l(m);
        std::cout <<"Destroying server socket "<<sockfd<<" @"<< this<<std::endl;
        ::close(sockfd);
    }

    Socket accept(struct sockaddr_in* addr, socklen_t* len){
        std::cout<<"Accepting..."<<std::endl;
        int fd=::accept(sockfd, (struct sockaddr*) &addr, len);
        std::cout << "Accepted socket: "<<fd << "..."<<std::endl;
        if (fd<0)
            throw std::runtime_error(strerror(errno));
        return Socket(fd);
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

class Threadpool {
    std::vector<std::thread> v;
    std::queue<std::packaged_task<void()>> pool;
    std::mutex m;
    std::condition_variable cv;

public:

    Threadpool(){
        startloop();
    }

    void submit(std::packaged_task<void()> p){
        std::lock_guard l(m);
        pool.emplace(std::move(p));
        std::cout<<"metto il packaged task nella coda"<<std::endl;
        cv.notify_one();
    }

    void startloop(){
        for (int i=0; i<10; i++) {
            v.emplace_back((std::thread([this](){
                while(true){
                    std::packaged_task<void()> tmp;
                    {
                        std::unique_lock l(m);
                        cv.wait(l, [this]() { return !this->pool.empty(); });
                        std::cout << "Il pollo è grande: "<<pool.size()<<std::endl;
                        tmp=std::move(pool.front());
                        std::cout<<"prendo il packaged task dall coda"<<std::endl;
                        pool.pop();
                        std::cout<<"poppato"<<std::endl;
                    }
                    std::cout<<"sto per runnare"<<std::endl;
                    tmp();
                    std::cout<<"runno il tmp"<<std::endl;
                }
            })));
        }
    }

};


int main() {
    try {
        ServerSocket server(1234);

    int i;
    std::vector<std::thread> v;

    for(i=0; i<3;i++) {
        std::cout<<"\tIterazione #"<<i+1<<std::endl;
        struct sockaddr_in addr;
        socklen_t len=sizeof(addr);
        Socket s= server.accept(&addr, &len);


        Threadpool tp;
        std::cout<<"sto per submittare il packaged task"<<std::endl;
        std::packaged_task<void()> p([s=std::move(s)]() mutable {
            char buf[1024];
            while(true) {
                std::cout<<"sono vivo fragola, il socket e': "<<s.getfd() << std::endl;
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

        tp.submit(std::move(p));
        std::cout<<"ho submittato il packaged task #"<<i+1<<std::endl;
        std::cout<<"Il socket e':"<< s.getfd()<<std::endl;
    }


    for(i=0; i<3; i++) {
        if(v.at(i).joinable())
            v.at(i).join();
    }

    }
    catch (std::exception const& e) {
        std::cout<<"oh shit"<<e.what()<<std::endl;
    }




    return 0;
}

