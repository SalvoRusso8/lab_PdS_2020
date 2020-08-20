//
// Created by ubuntu on 17/08/20.
//

#ifndef ESERCITAZIONE4_JOBS_H
#define ESERCITAZIONE4_JOBS_H


#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template <class T>
class Jobs {

    std::queue<T> jobs;
    bool active = true;
    std::mutex qm;
    std::condition_variable not_empty;
    std::condition_variable full;

public:
    //inserisce un job in coda in attesa di essere processato, può essere bloccante se la coda dei job è piena
    void put(T job){
        //versione prof
        std::lock_guard _g(qm);
        jobs.push(job);
        not_empty.notify_one();

        //versione mia
        /*std::unique_lock lock(qm);
        full.wait(lock, [this](){ return this->jobs.size()< 100;});
        jobs.push(job);
        not_empty.notify_one();*/

    }

    //legge un job dalla coda e lo rimuove, si blocca se non ci sono valori in coda
    std::optional<T> get(int to) {
        std::unique_lock lock(qm);
        while (true){
            // myout("size: " + std::to_string(jobs.size()));

            if(jobs.size()>0){
                T job = jobs.front();
                jobs.pop();
                full.notify_one();
                return job;
            }
            if(!active){
                return std::optional<T>();
            }
            not_empty.wait(lock, [this](){ return this->jobs.size()>0 || !this->active;});
        }
    }

    void ended(){
        std::lock_guard g(qm);
        active = false;
        not_empty.notify_all();
    }

    int size(){
        return jobs.size();
    }
};


#endif //ESERCITAZIONE4_JOBS_H
