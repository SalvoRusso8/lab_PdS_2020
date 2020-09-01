#include <iostream>

#include "Challenge.h"

int main() {
    int i=10;
    while (i>0){
        try {
            Challenge c;
            std::thread t1([&c](){
                c.accept(true);
                c.result2(rand()%6);
            });

            std::thread t2([&c](){
                c.result1(rand()%6);
                std::cout<<"and the winner is...: "<<c.winner()<<std::endl;
            });


            if(t1.joinable() && t2.joinable()){
                t1.join();
                t2.join();
            }


        }
        catch (...) {
            std::cerr << "problemino..." << std::endl;
            if(i==0) exit(-1);
            i--;
        }
    }

    return 0;
}
