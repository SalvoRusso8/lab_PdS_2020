//
// Created by Salvo-fisso on 30/04/2020.
//

#ifndef LAB2_DIRECTORY_H
#define LAB2_DIRECTORY_H
#include <memory>
#include <map>
#include "Base.h"
#include "File.h"

class Directory: public Base  {
    std::weak_ptr<Directory> self;
    std::string nome;
    std::weak_ptr<Directory> padre;
    std::map<std::string, std::shared_ptr<Base>> figli;
    Directory(){};
public:
    std::shared_ptr<Directory> addDirectory(const std::string& nome);
    static std::shared_ptr<Directory> MakeDir(std::string nome, std::weak_ptr<Directory> padre);
    static std::shared_ptr<Directory> getRoot();
    void ls(int indent) const override;
    //std::shared_ptr<Directory> get(std::string name);
    ~Directory(){
        std::cout<<"~D() @"<<this<<std::endl;
    }
    int mType() const;
    std::shared_ptr<File> addFile(const std::string& nome, uintmax_t size);
    std::shared_ptr<Base> get(const std::string& name);
    std::shared_ptr<Directory> getDir(const std::string& name);
    std::shared_ptr<File> getFile(const std::string& name);
    bool remove(const std::string& nome);
private:
    static std::shared_ptr<Directory> radice;

};


#endif //LAB2_DIRECTORY_H
