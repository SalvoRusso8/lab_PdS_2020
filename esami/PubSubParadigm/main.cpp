#include <iostream>
#include <string>
#include "PubSubHub.h"


int main(int argc, char* argv[])
{
    // (copiato dalla soluzione non ufficiale di G. Porcellini)
    std::string data;
    PubSubHub psb;

    psb.subscribe("pd1");
    psb.subscribe("prl");
    psb.subscribe("pds");
    psb.subscribe("softeng");


   std::thread t2 = std::thread([&psb](){
        std::string data;

        psb.subscribe("pds");
        psb.subscribe("softeng");

        psb.publish("pds",  "ma i risultati dei compiti???");
        psb.getData("pds", data);
        psb.unsubscribeAll();
        std::cout << "1 ended" << std::endl;
    });

   std::thread t3 = std::thread([&psb](){
        std::string data;


        psb.subscribe("pds");
        psb.subscribe("prl");

        psb.publish("prl", "Usciti i risultati!");
        psb.getData("pds", data);

        psb.publish("pds",  "voi dite?");
        psb.unsubscribeAll();
        std::cout << "2 ended" << std::endl;
    });


    psb.publish("pds", "nel 2020");
    psb.publish("prl", "Sul portale?");
    psb.getData("prl", data);

    psb.unsubscribeAll();
    t2.join();
    t3.join();

    return 0;
}