//
// Created by Giulio De Giorgi on 01/09/2020.
//

/*
Gioco online tra due giocatori, gestione di più thread

Implementare classe Challenge che aveva, oltre al costruttore, i seguenti metodi:

-void accept( Boolean response)
Indica il responso dello sfidato alla richiesta di giocare

-void result1( int score)
Indica lo score dello sfidante

-void result2( int score)
Indica lo score dello sfidato

-int winner()
Ritorna:
-0 se è pari
-1 se ha vinto lo sfidante
-2 se ha vinto lo sfidato
- -1 se la richiesta è stata rifiutata

Accept deve precedere result1 e result2,  che invece non hanno ordine di chiamata tra loro, e tutti e 3 possono essere chiamati una sola volta, la violazione dei vincoli genera eccezione.

Winner si mette in attesa senza consumare CPU se il gioco è in corso o non è ancora cominciato*/

#ifndef GIOCO_CHALLENGE_H
#define GIOCO_CHALLENGE_H

#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>


class Challenge {

private:
    int score1, score2;
    bool response, accepted;
    std::mutex m;
    std::condition_variable cv;


public:
    Challenge():score1(-1), score2(-1), response(false), accepted(false){

    };

    void accept(bool response){
        std::lock_guard l(m);
        if(accepted) throw "Accept chiamata più volte, rip"; /* il gioco e' gia' stato accettato*/
        this->accepted=true; /* indica che accept e' stata chiamata */
        this->response=response; /* indica se il giocato re ha scelto di partecipare al gioco o no */
        cv.notify_one();
    };

    void result1(int score){
        std::lock_guard l(m);
        if(!response) {
            std::cout << "problema con response"<<std::endl;
            throw "Challenge rifiutata o accept non ancora effettuata";}
        if(score1>=0) {
            std::cout<<"problema con score1"<<std::endl;
            throw "result1 gia' chiamata";
        }
        this->score1=score;
        cv.notify_one();
    };
    void result2(int score){
        std::lock_guard l(m);
        if(!response) {
            std::cout << "problema con response"<<std::endl;
            throw "Challenge rifiutata o accept non ancora effettuata";
        }
        if(score2>=0) {
            std::cout<<"problema con score2"<<std::endl;
            throw "result2 gia' chiamata";
        }
        this->score2=score;
        cv.notify_one();
    };
    int winner(){
        std::unique_lock l(m);
        cv.wait(l, [this](){return (this->response && this->score1 >=0 && this->score2 >=0) ||
                (this->accepted && !this->response);});
        if(!this->response) return -1;
        if(this->score1>this->score2) return 1;
        if(this->score2>this->score1) return 2;
        else return 0;
    };




};

#endif //GIOCO_CHALLENGE_H
