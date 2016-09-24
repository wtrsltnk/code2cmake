#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "tokenizer.h"

class Interpreter
{
    static bool Equals(const std::string& a, const std::string& b);

public:
    Interpreter(Tokenizer& t);
    virtual ~Interpreter();

};

#endif // _INTERPRETER_H_
