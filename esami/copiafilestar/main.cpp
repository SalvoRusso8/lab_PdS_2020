#include <iostream>

class Classe {
    FILE* f;
public:
    /* costruttore di movimento */
    Classe(Classe&& source) {
        this->f=source.f;
        source.f=nullptr;
    }
    /* costruttore di copia */
    Classe(const Classe& source){
        this->f=new FILE();
        this->f=source.f; // ??
    }

    /* operatore di assegnazione per movimento */
    Classe& operator=(Classe&& source){
        if(this!=&source){
            delete[] this->f;
            this->f=source.f;
            source.f=nullptr;
        }
        return *this;
    }


    /* operatore di assegnazione per copia */

    Classe& operator=(const Classe& source) {
        if(this!=&source){
            delete this->f;
            this->f=nullptr;
            this->f=new FILE();
            this->f=source.f; // ??
        }
        return *this;
    }
};

int main(){


    return 0;
}