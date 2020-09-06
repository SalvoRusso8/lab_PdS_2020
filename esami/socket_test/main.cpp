#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <shared_mutex>
#include <thread>
#include <vector>


class Socket {
    int sockfd;
    Socket(const Socket&)=delete;
    Socket& operator=(const Socket&)=delete;
    Socket(int sockfd): sockfd(sockfd){}

public:
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

int main() {
    ServerSocket server(5000);
    int i=3;
    std::vector<std::thread> v;

    for(i=0; i<2;i++) {
        struct sockaddr_in addr;
        socklen_t len=sizeof(addr);
        Socket s=server.accept(&addr, &len);
        v.push_back(std::thread(f, std::move(s)));

    }
    for(i=0; i<2; i++) {
        if(v.at(i).joinable())
            v.at(i).join();
    }

    return 0;
}

