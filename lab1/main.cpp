#include <iostream>
#include "Message.h"
#include "DurationLogger.h"
#include "MessageStore.h"


int main() {
    /*Message m1(10);
    Message m2;
    std::cout << "m1\n " << m1 << std::endl << " m2\n " << m2 << std::endl;
    Message m3(m1);
    std::cout << "m3\n " << m3 << std::endl << " m1\n " << m1 << std::endl;
    Message m31(std::move(m3));
    std::cout << "m31\n " << m1 << std::endl << " m1\n " << m3 << std::endl;
    Message m4(10);
    std::cout << "m4\n " << m4 << std::endl;
    m4=m1;
    std::cout << "m4\n " << m4 << std::endl << " m1\n " << m1 << std::endl;
    Message m5;
    m5=std::move(m1);
    std::cout << "m5\n " << m5 << std::endl << " m1\n " << m1 << std::endl;


    Message buff1[10];
    Message *buff2 = new Message[10];
    for (int i = 0; i < 10; i++)
        buff1[i] = Message(1 * 1024 * 1024);
    {
        DurationLogger dl1 = DurationLogger("clock1()");
        for (int i = 0; i < 10; i++)
            buff2[i] = buff1[i];
    }
    {
        DurationLogger dl2 = DurationLogger("clock2()");
    for (int i = 0; i < 10; i++)
        buff2[i] = std::move(buff1[i]);
}
    return 0;

    //std::cout << "buff1[0]\n" << buff1[0] <<std::endl;*/
    MessageStore ms(10);
    for(int i=0;i<100;i++) {
        Message m(1*1024*1024);
        ms.add(m);
    }
    std::cout << "stats after add dim " << ms.getDim() << " valid" << std::get<0>(ms.stats()) << " empty " << std::get<1>(ms.stats()) <<std::endl;
    for(int i=0; i<50;i++){
        ms.remove(i);
    }
    std::cout << "stats after remove dim " << ms.getDim() << " valid" << std::get<0>(ms.stats()) << " empty " << std::get<1>(ms.stats()) <<std::endl;
    ms.compact();
    std::cout << "stats after compact dim " << ms.getDim() << " valid" << std::get<0>(ms.stats()) << " empty " << std::get<1>(ms.stats()) <<std::endl;
    for(int i=0;i<50;i++) {
        std::string s(ms.getMessages()[i].getData());
        std::cout << "m" << i << " --------------------------------------------------------------- " << s.substr(0,50) << std::endl;
    }

    return 0;
}