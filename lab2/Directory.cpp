//
// Created by Salvo-fisso on 30/04/2020.
//

#include <mutex>
#include <iostream>
#include "Directory.h"

std::shared_ptr<Directory> Directory::radice= nullptr;

std::shared_ptr<Directory> Directory::addDirectory(const std::string& nome){
        if(figli.find(nome)== figli.end() && nome!="." && nome!=".." && nome!="/"){
            std::shared_ptr<Directory> son = MakeDir(nome, this->self);
            figli.insert(std::pair<std::string, std::shared_ptr<Directory>>(nome, son));
            return son;
        }
        else
            return nullptr;
    }

std::shared_ptr<Directory> Directory::MakeDir(std::string nome, std::weak_ptr<Directory> padre){
        Directory* d = new Directory();
        std::shared_ptr<Directory> ptr = std::shared_ptr<Directory>(d);
        d->nome = nome;
        d->padre = padre;
        d->self = ptr;
        return ptr;
}

std::shared_ptr<Directory> Directory::getRoot(){
    if(radice== nullptr){
        radice = std::shared_ptr<Directory>(new Directory());
        radice->nome = "/";
        radice->padre = std::shared_ptr<Directory> (nullptr);
        radice->self = radice;
        return radice;
    } else
        return radice;
}

void Directory::ls(int indent) const {
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << this->nome << std::endl;

    for (auto j = this->figli.begin(); j != this->figli.end(); j++) {
        if(j->second->mType()==DIRECTORY)
            std::dynamic_pointer_cast<Directory>(j->second)->ls(indent+4);
        else std::dynamic_pointer_cast<File>(j->second)->ls(indent+4);
    }
}

/*std::shared_ptr<Directory> Directory::get(std::string name) {
    std::string path=name;
    std::cout << "get(" << name << ")" << std::endl;
    if(name=="")
        return this->self.lock();
    std::shared_ptr<Directory> cur_ptr;
    std::string token = name.substr(0,name.find("/"));
    if(token=="..")
        cur_ptr=this->padre.lock();
    else
        if(token==".")
            cur_ptr = this->self.lock();
        else
            if(this->figli.find(token) == this->figli.end())
                std::cout << "figlio non trovato" << std::endl;
            else
                cur_ptr = this->figli.find(token)->second;
    std::cout << "cur_ptr=" << cur_ptr << std::endl;
    if(name.find("/")>=name.length())
        return cur_ptr->self.lock();
    return cur_ptr->get(path.erase(0,name.find("/")+1));
}*/

int Directory::mType() const{
    return DIRECTORY;
}

std::shared_ptr<File> Directory::addFile(const std::string& nome, uintmax_t size){
    if(figli.find(nome)== figli.end() && nome!="." && nome!=".." && nome!="/"){
        std::shared_ptr<File> son = std::shared_ptr<File> (new File(nome,size));
        figli.insert(std::pair<std::string, std::shared_ptr<File>>(nome, son));
        return son;
    }
    else
        return nullptr;
}

std::shared_ptr<Base> Directory::get(const std::string& name){
    if(name=="..")
        return padre.lock();
    if(name==".")
        return self.lock();
    if(figli.find(name)!=figli.end())
        return figli.find(name)->second;
    else return nullptr;
}

std::shared_ptr<Directory> Directory::getDir(const std::string& name){
    return std::dynamic_pointer_cast<Directory>(this->get(name));
}

std::shared_ptr<File> Directory::getFile(const std::string& name){
    return std::dynamic_pointer_cast<File>(this->get(name));
}

bool Directory::remove(const std::string& nome){
    if(nome==".." || nome=="." || figli.find(nome)==figli.end())
        return false;
    else{
        figli.erase(nome);
        return true;
    }
}
