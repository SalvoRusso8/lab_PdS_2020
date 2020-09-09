/* Soluzione ufficiale del prof. Malnati: 
 * #include <iostream>
#include <thread>
#include <mutex>
class semaphore {
    int count;
    std::mutex m;
    std::condition_variable cv;public:
    explicit semaphore(int count): count(count) {}
    void acquire() {
        std::unique_lock ul(m);
        cv.wait(ul,[this]() {return this->count>0;});
        count--;
    }
    void release() {
        std::unique_lock<std::mutex> ul(m);
        count++;
        cv.notify_one();
    }
};class raii_guard {
    semaphore& s;
public:
    explicit raii_guard(semaphore& s): s(s) {
        s.acquire();
    }
    ~raii_guard() {
        s.release();
    }
};*/




#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <future>
#include <chrono>
#include <thread>
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
        //std::cout << "max number of threads: "<<&std::thread::hardware_concurrency << std::endl;
        for(int i=0;i< 10;i++){
            vt.push_back(std::thread([this, i](){
                while(true){
                    std::unique_lock<std::mutex> l(this->m);
                    this->cv.wait(l,[this](){return (!this->q.empty() && this->counter<MAX_N) || this->fine;});
                    if(this->fine && this-> q.empty())
                        break;
                    this->counter++;
                    std::packaged_task<void()> p = std::move(this->q.front());
                    this->q.pop();
                    l.unlock();
                    p();
                    std::cout<<"Thread #"<<i<<":"<< std::endl;
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
    std::cout << "Hello, RAM :)" << std::endl;
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