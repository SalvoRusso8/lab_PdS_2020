#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <map>
#include <vector>
#include <thread>
std::mutex om;

void myout(std::string msg){
    std::lock_guard og(om);
    std::cout<<msg<<std::endl;
}
double get_measurement();
class Joiner {
private:
    int N;
    std::map<int,double> m;
    int counter=0;
    std::condition_variable cv;
    std::condition_variable cv_full;
    std::mutex mutex;




public:
    Joiner(int N):N(N){};
    std::map<int,double> supply(int key, double value){

        std::unique_lock lock(mutex);

        cv_full.wait(lock, [this](){
            return this->m.size()<this->N;
        });
        m.insert({key, value});
        counter++;
        cv.wait(lock, [this](){
            return this->m.size()==this->N;
        });

        std::map<int, double> tmp=m;
        counter--;
        if(counter==0) {
            /* i'm the last so i make the map empty and wake everyone up in
            order to start filling the map up again*/
            myout("#### measurements are ended! ###\n");
            m.clear(); //make map empty
            cv_full.notify_all();
        }
        else{
            /* i'm not the last (N-1 thread will enter) and i will wake everyone up
            in order to make them return the current map value*/


            cv.notify_all();
        }
        return tmp;
    }

};


int main(){
    int n=3;

    int i=0;
    Joiner j(n);




    std::vector<std::thread> thread;
    while(i<n) {
        thread.push_back(std::thread([i, n, &j]() {
            while (true) {
                int key = i;
                double value = get_measurement();
                std::map<int, double> m = j.supply(key, value);

                for (auto const &pair: m) {
                    std::string msg= "Thread #" + std::to_string(i) + ": " +"{" + std::to_string(pair.first) +
                            ": " + std::to_string(pair.second)+ "}\n";
                    myout(msg);
                }
            }
        }));
        i++;
    }

    for (auto &t:thread) {
        if(t.joinable())
            t.join();
    }
    return 1;

}




double get_measurement(){
    return rand()%4092;
}