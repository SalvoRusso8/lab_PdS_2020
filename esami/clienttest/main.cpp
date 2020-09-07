#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <mutex>


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

    friend class ClientSocket;
};


class ClientSocket: public Socket {
private:
    struct sockaddr_in addr;


public:
    ClientSocket(u_int16_t portNumber, const char *ipaddress) {

        std::cout<<"porta: "<<portNumber<<" ipaddr non convertito: "<<ipaddress<<" ipaddr convertito: "<<inet_addr(ipaddress)<<std::endl;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(portNumber);
        addr.sin_addr.s_addr = inet_addr(ipaddress);
        std::cout << "Creating client socket " << sockfd << " @" << this << std::endl;

    }

    ~ClientSocket() {
        std::cout << "Destroying client socket " << sockfd << " @" << this << std::endl;
        ::close(sockfd);
    }

    void connect() {
        std::cout << "Connecting to server..." << std::endl;
        int res = ::connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));
        if (res < 0) {
            std::cout<<"problemino..."<<std::endl;
            throw std::runtime_error(strerror(errno));
        }
    }
};

int main() {

    u_int16_t port;
    char ipaddr[16];
    char buf[1024];

    std::cout<<"inserisci indirizzo e porta:\n";
    std::cin>>ipaddr;
    std::cin>>port;

    ClientSocket cs(port, ipaddr);
    cs.connect();

    char exit[1024]="exit\n";
    scanf("%*c");

    for(int i=0; i<3; i++) {
        std::cout<<"parla con il server:\n";
        std::cin.getline(buf, 1024);
        std::cout<<"\tHo letto sta roba: "<<buf<<std::endl;

        std::string str(buf);
        cs.write(const_cast<char *>(str.c_str()), str.length(), 0);
        ssize_t read_size=cs.read(buf, sizeof(buf)-1,0);
        buf[read_size]='\0';
        std::cout<<"il server ha risposto: "<<buf<<std::endl;

    }
    cs.write(exit, sizeof(exit), 0);




    return 0;
}
