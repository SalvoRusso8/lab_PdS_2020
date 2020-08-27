#include <iostream>
#include <thread>
#include <optional>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <regex>
#include "jobs.h"

std::mutex om;

void myout(std::string msg){
    std::lock_guard og(om);
    std::cout<<msg<<std::endl;
}

//struttura che contiene la riga e informazioni sul file e sul numero della riga nel file
struct riga{
    std::string nameFile;
    std::string row;
    int n_riga;
};




#define N 10000

int main() {
    // Esempio di multiple producer e consumer, con verifica che tutti i messaggi siano processati solo una volta
    // Testare il codice concorrente e' difficile, non si possono usare breakpoint e gli errori spesso non sono
    // riproducibili
    // In genere conviene risolvere un problema equivalente con un risultato predicibile.
    // In questo caso: 10 producer inseriscono in parallelo nei jobs i valori da 1 a N, con N >> 10
    // 10 consumer fanno +1 nell'indice corrispondente di un vector lungo N.
    // alla fine tutte le celle devono valere esattamente 1.

    //Jobs<int> jobs;
    std::vector<int> results(N);
    std::atomic<int> count(0);

    //Jobs<riga> jobs;
    //std::atomic<int> count_row(0);



    Jobs<std::string> fileJobs;
    Jobs<riga> lineJobs;
    std::vector<std::thread> producersLines;
    //std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    std::thread producerFileNames ([&fileJobs](){

        namespace fs = std::filesystem;
        for(auto& p: fs::directory_iterator("../files")){
            fileJobs.put(p.path());
        }
        myout("Producer of fileNames terminated");
    });

    //producers che leggono i file
    for(int i=0; i<10; i++){
        producersLines.push_back(std::thread ([&fileJobs, &lineJobs, i](){
            int count;
            std::string line;
            while(true){
                std::optional<std::string>  idx = fileJobs.get(0);
                if(idx.has_value()){
                    std::ifstream ifs(static_cast<const std::basic_ifstream<char>>(idx.value()));
                    if(!ifs.is_open()){
                        std::cout<<"unable to open file"<<std::endl;
                    }
                    while(getline(ifs, line)){
                        riga row;
                        row.n_riga=count++;
                        row.nameFile=idx.value();
                        row.row=line;
                        lineJobs.put(row);
                    }
                    myout("ProducerLines " + std::to_string(i) + " finished file " + idx.value());
                } else {
                    myout("ProducerLines " + std::to_string(i) + " terminated");
                    break;
                }
            }
        }));
    }

    // consumers che leggono le righe
    for(int i=0; i<10; i++){
        consumers.push_back(std::thread([&lineJobs, &count, i](){
            while(true){
                std::optional<riga>  idx = lineJobs.get(0);
                std::regex e("vita");
                std::match_results< std::string::const_iterator > mr;
                if(idx.has_value()){
                    if(std::regex_search(idx->row,mr,e)){
                        //std::cout <<"match trovato" <<std::endl;
                        count.fetch_add(1);
                        myout("consumer " + std::to_string(i) + " has found match 'vita' in file " + idx->nameFile + " at row " +
                              std::to_string(idx->n_riga));
                    }
                } else {
                    myout("consumer " + std::to_string(i) + " terminated");
                    break;
                }
            }
        }));
    }

    /*
    std::thread producer ([&jobs](){
        int count=0;
        std::string line;

        namespace fs = std::filesystem;
        for(auto& p: fs::directory_iterator("../files")){   //itero sui file nella cartella files per i produttori
            //count_row.store(0);
            std::string nameFile = p.path().filename(); //salvo il nome del file per metterlo eventualmente nella struttura
            std::ifstream ifs(p.path());    //apro il file
            std::cout << p.path() << std::endl;
            if(!ifs.is_open()){
                std::cout<<"unable to open file"<<std::endl;
            }
            //creo il thread che si occuperà di leggere per riga il file e salvarlo nei jobs


            while(getline(ifs, line)){
                int idx = count++;  //conteggio di riga
                riga row;
                row.n_riga=idx;
                row.nameFile=nameFile;
                row.row=line;
                jobs.put(row);
            }
            myout("producer of file " + nameFile + " terminated");

        //t.join();
        };

    });


    for(int i=0; i<10; i++){
        consumers.push_back(std::thread([&jobs, &count, i](){
            while(true){
                std::optional<riga>  idx = jobs.get(0);
                //myout("analizzo riga " + idx->row + " del file " + idx->nameFile + " at row n. ");
                std::regex e("vita");
                std::match_results< std::string::const_iterator > mr;
                if(idx.has_value()){
                    //myout("consumer " + std::to_string(i) + " ha trovato riga " + idx->row + " da controllare");
                    if(std::regex_search(idx->row,mr,e)){
                        //std::cout <<"match trovato" <<std::endl;
                        count.fetch_add(1);
                        myout("consumer " + std::to_string(i) + " has found match 'vita' in file " + idx->nameFile + " at row " +
                                      std::to_string(idx->n_riga));
                    }
                } else {
                    myout("consumer " + std::to_string(i) + " terminated");
                    break;
                }
            }
        }));
    }
    */




    /*for(int i=0; i<10; i++){
        producers.push_back(std::thread([&jobs, &count, i](){
            while(true){
                int idx = count.fetch_add(1);
                if(idx<N)
                    jobs.put(idx);
                else
                    break;
            }
            myout("producer " + std::to_string(i) + " terminated");
        }));
    }

    for(int i=0; i<10; i++){
        consumers.push_back(std::thread([&jobs, &results, i](){
            while(true){
                std::optional<int>  idx = jobs.get(0);
                if(idx.has_value()){
                    // safe perche' solo un consumer ricevera' l'idx
                    results[idx.value()] += 1;
                } else {
                    myout("consumer " + std::to_string(i) + " terminated");
                    break;
                }
            }
        }));
    }
    */

    producerFileNames.join();
    fileJobs.ended();
    for (auto &p: producersLines) {
        if(p.joinable()) p.join();
    }
    myout("producer terminati");
    lineJobs.ended();

    myout("in attesa consumer, job ancora aperti: " + std::to_string(lineJobs.size()));

    for (auto &c: consumers) {
        if(c.joinable()) c.join();
    }

    myout("consumer terminati job ancora aperti: " + std::to_string(lineJobs.size()));
    std::cout << "totale " << count<< std::endl;
    myout("verifica risultati " + count.load());
/*
    bool error = false;
    for(int i=0; i<results.size(); i++){
        if(results[i] != 1){
            myout("error at index: " + std::to_string(i) +  " value: " + std::to_string(results[i]));
        }
    }
    if(!error)
        myout("all ok");*/

}

