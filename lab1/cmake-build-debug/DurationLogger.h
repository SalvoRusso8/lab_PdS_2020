//
// Created by Salvo-fisso on 22/04/2020.
//

#ifndef LAB1_DURATIONLOGGER_H
#define LAB1_DURATIONLOGGER_H


#include <string>
#include <ctime>

class DurationLogger {
public:
    DurationLogger(const std::string& name);
    ~DurationLogger();
    clock_t t;
    std::string functionName;
};


#endif //LAB1_DURATIONLOGGER_H
