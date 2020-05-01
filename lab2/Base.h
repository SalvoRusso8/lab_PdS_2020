//
// Created by Salvo-fisso on 01/05/2020.
//

#ifndef LAB2_BASE_H
#define LAB2_BASE_H


#include <string>
#define DIRECTORY 0
#define FILE 1

class Base {
    std::string name;
public:
    std::string getName() const{
        return name;
    };
    virtual int mType() const = 0;
    virtual void ls(int indent) const = 0;
};


#endif //LAB2_BASE_H
