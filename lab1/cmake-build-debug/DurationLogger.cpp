//
// Created by Salvo-fisso on 22/04/2020.
//

#include "DurationLogger.h"
#include <ctime>
#include <iostream>

DurationLogger::DurationLogger(const std::string &ma): functionName{ma} {
    t = clock();
}
DurationLogger::~DurationLogger() {
    t = clock() - t;
    std::cout << "Tempo impiegato per l'esecuzione di " << functionName << ":" << t << "ms" << std::endl;
}