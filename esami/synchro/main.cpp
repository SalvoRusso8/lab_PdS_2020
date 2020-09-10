/* Un sistema embedded riceve su due porte seriali sequenze di dati provenienti
 * da due diversi sensori. Ciascun sensore genera i dati con cadenze variabili
 * nel tempo e non predicibili, in quanto il processo di digitalizzazione al suo
 * interno può richiedere più o meno tempo in funzione del dato letto. Ogni volta
 * che il sistema riceve un nuovo valore su una delle due porte seriali, deve accoppiarlo
 * con il dato più recente ricevuto sull'altra (se già presente) e inviarlo ad una fase
 * successiva di computazione. Il sistema al proprio interno utilizza due thread differenti
 * per leggere dalle due porte seriali e richiede l'uso di un oggetto di sincronizzazione
 * in grado di
 * implementare la logica descritta sopra. Tale oggetto offre la seguente interfaccia pubblica:

class Synchronizer {
public:
 Synchronizer(std::function<void(float d1, float d2)> process);
 void dataFromFirstPort(float d1);
 void dataFromSecondPort(float d2);
}
 All'atto della costruzione, viene fornita la funzione process(...) che rappresenta la fase
 successiva della computazione. Quando vengono invocati i metodi dataFromFirstPort(...) o
 dataFromSecondPort(...), se non è ancora presente il dato dalla porta opposta, questi si bloccano
 al proprio interno senza consumare CPU, in attesa del valore corrispondente. Al suo arrivo, viene
 invocata una sola volta la funzione process(...). Si implementi tale classe utilizzando le funzionalità
 offerte dallo standard C++.
 */

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <zconf.h>

class Synchronizer {
    std::mutex m;
    std::condition_variable cv;
    float d1;
    float d2;
    bool readyd1, readyd2, shutdown;
    std::function<void(float d1, float d2)> f;
    std::thread t;

public:
    Synchronizer(std::function<void(float d1, float d2)> process): readyd1(false), readyd2(false),shutdown(false), f(process){

        /*t=std::thread([this](){
            while (true) {
                std::unique_lock l(m);
                cv.wait(l, [this](){return (readyd1 && readyd2) || shutdown;});
                if(shutdown && (!readyd1 || !readyd2)) break;
                f(d1,d2);
                readyd1=false;
                readyd2=false;
                cv.notify_all();
            }
        });*/
    }

    void end(){
        std::unique_lock l(m);
        shutdown=true;
    }
    ~Synchronizer(){
        std::unique_lock l(m);
        shutdown=true;
        l.unlock();
        if(t.joinable())
            t.join();
        l.lock();
    }
    void dataFromFirstPort(float d1){
        std::unique_lock l(m);
        std::cout<<"Port #1: received data >> "<<d1<<std::endl;
        cv.wait(l, [this](){return !readyd1;});
        this->d1=d1;
        readyd1=true;
        if(readyd2) {
            f(this->d1,this->d2);
            readyd1=false;
            readyd2=false;
        } else
            cv.notify_all();
    }
    void dataFromSecondPort(float d2){
        std::unique_lock l(m);
        std::cout<<"Port #2: received data >> "<<d2<<std::endl;
        cv.wait(l, [this](){return !readyd2;});
        this->d2=d2;
        readyd2=true;
        if(readyd1) {
            f(this->d1, this->d2);
            readyd1 = false;
            readyd2 = false;
        } else
            cv.notify_all();
    }

    bool isEnded(){
        std::unique_lock l(m);
        return shutdown;
    }
};

void f(float d1, float d2) {
    std::cout << d1<<" + "<< d2 << "=" << d1+d2 << std::endl;
}

int main() {
    Synchronizer s(f);
    std::thread t1([&s](){
        int i=10;
        while(i>0) {
        sleep(rand()%5);
        s.dataFromFirstPort(rand()%49);
        i--;
        }
        //s.end();
    });

    std::thread t2([&s](){
        while (true) {
        sleep(rand()%5);
        s.dataFromSecondPort(rand()%49);
        }
    });

    t1.join();
    t2.join();
    return 0;
}
