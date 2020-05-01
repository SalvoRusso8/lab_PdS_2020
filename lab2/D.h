//
// Created by Salvo-fisso on 30/04/2020.
//

#ifndef LAB2_D_H
#define LAB2_D_H
#include <memory>


class D {
    std::weak_ptr<D> parent;
public:
    D(std::weak_ptr<D> parent): parent(parent){
        std::cout<<"D() @"<<this<<std::endl;
    }
    std::shared_ptr<D> addChild() {
        std::shared_ptr <D> child = std::make_shared<D>(
                std::shared_ptr<D>(this));
        return child;
    }
    ~D(){
        std::cout<<"~D() @"<<this<<std::endl;
    }
};


#endif //LAB2_D_H
