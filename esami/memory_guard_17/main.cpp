#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>
#include <memory>
#include <algorithm>
#include <future>
#include <chrono>
#include <thread>
#include <memory>
#include <queue>

#define MAX_N 3

class memory_guard{
    std::queue<std::packaged_task<void()>> q;
    std::vector<std::thread> vt;
    std::mutex m;
    std::condition_variable cv;
    int counter;
    bool fine;

public:
    memory_guard():counter(0), fine(false){
        std::cout << "costruttore @ " << this << std::endl;
        std::cout << "max number of threads: "<<std::thread::hardware_concurrency << std::endl;
        for(int i=0;i< std::thread::hardware_concurrency();i++){
            vt.push_back(std::thread([this](){
                while(true){
                    std::unique_lock<std::mutex> l(this->m);
                    this->cv.wait(l,[this](){return (!this->q.empty() && this->counter<MAX_N) || this->fine;});
                    if(this->fine)
                        break;
                    this->counter++;
                    std::packaged_task<void()> p = std::move(this->q.front());
                    this->q.pop();
                    l.unlock();
                    p();
                    std::cout<<"Counter= "<<counter<<std::endl;
                    l.lock();
                    this->counter--;
                    this->cv.notify_one();
                }
            }));
        }
    }

    ~memory_guard(){
        std::cout << "distruttore @ " << this << std::endl;
        std::unique_lock<std::mutex> l(m);
        fine = true;
        cv.notify_all();
        for(auto it=vt.begin(); it!=vt.end(); it++){
            if(it->joinable())
                l.unlock();
            it->join();
            l.lock();
            std::cout << "distruttore @ " << this << " ha finito un thread" << std::endl;
        }
    }

    std::future<void> submit(std::function <void()> f){
        std::unique_lock<std::mutex> l(m);
        std::packaged_task<void()> p(f);
        std::future<void> fut = p.get_future();
        q.push(std::move(p));
        cv.notify_one();
        return fut;
    }

};


void f(){
    std::cout << "Hello, muthafucca ram" << std::endl;
}

int main(){
    memory_guard m;
    //f();
    //std::function<void()> func = f;
    m.submit(f);
    m.submit(f);
    m.submit(f);
    m.submit(f);
    m.submit(f);
    m.submit(f);
    m.submit(f);



    return 0;
}