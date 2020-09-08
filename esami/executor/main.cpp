#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

/*
 * È presente una classe Executor che ha il compito di rimanere in ascolto su una coda di funzioni da svolgere
 * e si appoggia su una classe esterna Context (??)

class Executor {
public:
Executor(Context *ctx);
~Executor();
std::future<void> submit(std::function<void(Context*)> f);
void shutdown();
};

Quando viene costruita la classe, viene generato un unico thread che ha il compito di estrarre le funzioni dalla coda,
immesse tramite il metodo submit(), ed eseguirle. Esso termina quando viene chiamato il metodo shutdown() o il distruttore della classe.
Le funzioni da svolgere vengono immesse tramite un metodo submit, che torna un oggetto future col risultato dell'elaborazione.
Se il metodo viene chiamato quando è già stato chiamato il metodo shutdown, viene lanciata un'eccezione di tipo std::logic_error.
Il metodo shutdown viene chiamato per terminare l'esecuzione, e se ci sono ancora delle funzioni in coda, esse vengono comunque svolte.
Implementare la classe blabla
*/

template <class Context>
class Executor {
    std::queue<std::packaged_task<void(Context*)>> q;
    bool sd;
    std::mutex m;
    std::condition_variable cv;
    Context *c;
    std::thread t;

public:
    Executor(Context *ctx): c(ctx), sd(false){
        t = std::thread([this](){
            while (true) {
                std::unique_lock l(m);
                cv.wait(l, [this](){return !q.empty() || sd;});
                if(sd && q.empty()) {
                    break;
                }
                std::packaged_task<void(Context*)> p = std::move(q.front());
                q.pop();
                l.unlock();
                p(c);
                std::cout<<"runno il packaged task!"<<std::endl;
            }
        });

    }
    ~Executor(){
        std::unique_lock l(m);
        if(!sd) {
            sd=true;
            cv.notify_one();
        }
        l.unlock();
        std::cout<<"Waiting for thread to join..."<<std::endl;
        t.join();
    };
    std::future<void> submit(std::function<void(Context*)> f){
        std::unique_lock l(m);
        if (sd) throw(std::logic_error("Error...\n"));
        std::packaged_task<void(Context*)> p(f);
        std::cout<<"packaged task creato!"<<std::endl;
        std::future<void> fut =p.get_future();
        std::cout<<"future creato!"<<std::endl;
        q.push(std::move(p));
        std::cout<<"lista riempita!"<<std::endl;

        cv.notify_one();
        return fut;
    }

    void shutdown(){
        std::unique_lock l(m);
        std::cout<<"Shutting down..."<<std::endl;
        this->sd=true;
        cv.notify_one();
    }
};

void increment(int *i) {
    (*i)++;
    std::cout<<""<<*i<<std::endl;
}

int main() {
    int i=0;
    Executor e(&i);
    std::function<void(int*)> f=increment;
    for(int j=0; j<5;j++) {
        std::future<void> fut = e.submit(f);
        std::cout<<"iterazione #"<<j<<std::endl;
    }

    /*
    std::future_status s = fut.wait_for(std::chrono::milliseconds(1000));
    if(s==std::future_status::ready)
        std::cout << "ready" << std::endl;
    else if(s==std::future_status::deferred)
        std::cout << "deferred"<<std::endl;
    else
        std::cout << "timeout" <<std::endl;
    */

    e.shutdown();
    return 0;
}
