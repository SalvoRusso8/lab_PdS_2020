//
// Created by Salvo-fisso on 01/05/2020.
//

#ifndef LAB2_FILE_H
#define LAB2_FILE_H
#include "Base.h"

class File:public Base {
    std::string nome;
    uintmax_t size;
public:
    int mType() const;
    void ls(int indent) const override;

    File(const std::string basicString, uintmax_t i);
    uintmax_t getSize() const;
    ~File(){
        std::cout<<"~F() @"<<this<<std::endl;
    }
};


#endif //LAB2_FILE_H
