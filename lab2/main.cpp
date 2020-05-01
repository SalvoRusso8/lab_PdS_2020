#include <iostream>
#include "D.h"
#include "Directory.h"

int main() {
    /*D root(std::shared_ptr<D>(nullptr));
    std::shared_ptr<D> child = root.addChild();*/
    /*std::shared_ptr<Directory> root = Directory::getRoot();
    std::cout << "Indirizzo di root inizializzato " << root << std::endl;
    std::shared_ptr<Directory> a = root->addDirectory("a");
    std::cout << "Indirizzo di a inizializzato " << a << std::endl;
    std::shared_ptr<Directory> a1 = a->addDirectory("a1");
    std::cout << "Indirizzo di a1 inizializzato " << a1 << std::endl;
    std::shared_ptr<Directory> a2 = a->addDirectory("a2");
    std::cout << "Indirizzo di a2 inizializzato " << a2 << std::endl;
    std::shared_ptr<Directory> b = root->addDirectory("b");
    std::cout << "Indirizzo di b inizializzato " << b << std::endl;
    std::shared_ptr<Directory> b1 = b->addDirectory("b1");
    std::cout << "Indirizzo di b1 inizializzato " << b1 << std::endl;
    std::shared_ptr<Directory> b2 =b1->addDirectory("b2");
    std::cout << "Indirizzo di b2 inizializzato " << b2 << std::endl;
    root->ls(0);
    std::cout << "Indirizzo di b1 con get "<< a->get("a1/../../b/b1") << std::endl;*/

    std::shared_ptr<Directory> root = Directory::getRoot();
    auto alfa = root->addDirectory("alfa");
    alfa->addDirectory("beta")->addFile("beta1",100);
    alfa->getDir("beta")->addFile("beta2",200);
    alfa->getDir("..")->ls(0);
    alfa->remove("beta");
    root->ls(0);

    return 0;
}
