#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

struct product{
    float lat_p;
    float long_p;
    float lat_a;
    float long_a;
    float timestamp_p;
    float timestamp_a;
};

struct consumed {
    float duration;
    float place_p;
    float place_a;
};

class Factory {

    struct product p ;
    struct consumed c;
    std::mutex m;
    std::condition_variable cv;
    std::vector<std::thread> vt;
    bool s_empty;
public:
    Factory():s_empty(true){

    }

    void produce(struct product p){
        std::unique_lock l(m);
        cv.wait(l, [this](){return this->s_empty==true;});
        this->p=p;

    }

    while(true) {
        lock
        cv.wait
        t1(calcola_luogo_partenza);
        t2(calcola_luogo_arrivo);
        t3(calcola_durata);
        t1.join
        t2.join
        t3.join


        raccogli_risultati_nella_struct();
    }


};



int main() {
    Factory f;
    float lat_p;
    float long_p;
    float lat_a;
    float long_a;
    float timestamp_p;
    float timestamp_a;

    p.lat_a=...
            ...
            ...;
    struct consumed c = f.produce(p);
}
