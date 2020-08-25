/* Si scriva un programma concorrente che, basandosi sull'algoritmo di fusione, generi un file di stringhe ordinate a
 * partire da due file di stringhe ordinate, secondo le seguenti specifiche. Il programma riceve il nome di tre file
 * sulla riga di comando. I primi due file sono di ingresso al programma: essi contengono un numero di stringhe
 * indefinito di lunghezza massima uguale a 50 caratteri, disposte in ragione di una stringa su ciascuna riga. Il
 * programma crea due nuovi processi figli. Il primo si occupa di leggere il primo file, trasferendo al padre una
 * stringa alla volta e quindi attendendo di essere risvegliato da quest'ultimo. Il secondo figlio si comporta nello
 * stesso modo leggendo il secondo file. Il padre, per ogni coppia di stringhe ricevuta dai figli, scrive la stringa
 * alfabeticamente inferiore sul file di uscita (con lo stesso formato del file di ingresso), mantiene l'altra stringa
 * per il confronto successivo, e sveglia il figlio di cui ha consumato la stringa letta. Al completamento della lettura
 * del proprio file figlio trasferisce al padre un identificatore specifico e termina. Al termine del procedimento il
 * file di uscita deve contenere tutte le stringhe dei due file di ingresso ordinate alfabeticamente.*/

#include <iostream>
#include <fstream>
#include <condition_variable>
#include <thread>
#include <optional>

template <class T>
class stringContainer{
private:
    std::string s;
    std::condition_variable empty;
    std::condition_variable full;
    std::mutex m;
    bool finish=false;
public:
    void put(T new_s){
        std::unique_lock lock(m);
        empty.wait(lock,[this](){return this->s.empty();});
        this->s=new_s;
        full.notify_one();
    }

    std::optional<T> get(){
        std::unique_lock lock(m);
        full.wait(lock,[this](){return !this->s.empty() || this->finish;});
        if(!this->s.empty()){
            std::string ret = this->s;
            this->s.clear();
            empty.notify_one();
            return ret;
        } else
            return std::optional<T>();
    }

    void end(){
        std::lock_guard l(m);
        this->finish=true;
        full.notify_all();
    }



};

int main(int argc, char* argv[]) {
    if(argc!=4){
        std::cout << "Numero di argomenti insufficiente" << std::endl;
        return -1;
    }

    stringContainer<std::string> container1;
    stringContainer<std::string> container2;
    std::thread readerFile1([&container1,&argv](){
        std::ifstream ifs(argv[1]);
        if(!ifs.is_open()){
            std::cout<<"unable to open file 1"<<std::endl;
        }
        std::string row;
        while(getline(ifs,row)){
            container1.put(row);
        }
        container1.end();
        ifs.close();
    });

    std::thread readerFile2([&container2,&argv](){
        std::ifstream ifs(argv[2]);
        if(!ifs.is_open()){
            std::cout<<"unable to open file 2"<<std::endl;
        }
        std::string row;
        while(getline(ifs,row)){
            container2.put(row);
        }
        container2.end();
        ifs.close();
    });

    readerFile1.detach();
    readerFile2.detach();

    std::ofstream fileOut(argv[3]);
    if(!fileOut.is_open()){
        std::cout<<"unable to open file out"<<std::endl;
    }
    bool stop1=false;
    bool stop2=false;
    std::optional<std::string> s1 = container1.get();
    std::optional<std::string> s2 = container2.get();
    do {
        if(!s1.has_value())
            stop1=true;
        if(!s2.has_value())
            stop2=true;
        if(!stop1 && !stop2){
            if(s1.value().compare(s2.value())>0)
                fileOut << s1.value() << std::endl;
            else
                fileOut << s2.value() << std::endl;
            s1 = container1.get();
            s2 = container2.get();
        }
    } while(!stop1 && !stop2);
    if(!stop1){
        while (s1.has_value()){
            fileOut << s1.value() << std::endl;
            s1 = container1.get();
        }
    }
    if(!stop2)
        while (s2.has_value()){
            fileOut << s2.value() << std::endl;
            s2 = container2.get();
        }
    std::cout << "Programma finito" << std::endl;
    return 0;
}
