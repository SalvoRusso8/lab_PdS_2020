//
// Created by Salvo-fisso on 01/05/2020.
//

#include <iostream>
#include "File.h"

File::File(std::string basicString, uintmax_t i) {
    nome=basicString;
    size=i;
}

int File::mType() const {
    return FILE;
}

uintmax_t File::getSize() const{
    return size;
}

void File::ls(int indent) const{
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << this->nome << " " << this->size << std::endl;
    return;
}




