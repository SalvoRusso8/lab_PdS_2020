#include <iostream>
#include <future>

#include "Challenge.h"

int main() {
    int i=0;
    while (i<10){
            std::promise<int> p1, p2;
            std::future<int> f1=p1.get_future();
            std::future<int> f2=p2.get_future();
            Challenge c;

            std::cout<<"Challenge #"<<i+1<<std::endl;
            std::thread t1([&c, &p1](){
                try {
                c.accept(true);
                c.result2(rand()%6);
                p1.set_value(0);
                }
                catch(...){
                    std::cout<<"oh shit!"<<std::endl;
                    try {
                        // store anything thrown in the promise
                        p1.set_exception(std::current_exception());
                    } catch(...) {
                        std::cout << "thread #1: exc" << std::endl;
                    } // set_exception() may throw too
                }
            });

            std::thread t2([&c, &p2](){
                try {
                    c.result1(rand() % 6);
                    int esito=c.winner();
                    p2.set_value(esito);
                    std::cout << "\tand the winner is...: " << esito << std::endl;
                }
                catch(...){
                    std::cout<<"ops i did it again"<<std::endl;
                    try {
                        // store anything thrown in the promise
                        p2.set_exception(std::current_exception());
                    } catch(...) {
                        std::cout << "thread #2: exc" << std::endl;
                    } // set_exception() may throw too
                }
            });


            try {
                //std::cout << "trying to get something..."<<std::endl;
                f1.get();
                f2.get();
                //std::cout << "got something big"<<std::endl;
            } catch(const char* s) {
                std::cout << "Exception from the thread: " << s << '\n';
                break;
            }

            if(t1.joinable() && t2.joinable()){
                t1.join();
                t2.join();
            }
        i++;
        }

    return 0;
}
