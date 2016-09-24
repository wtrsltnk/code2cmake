#include "interpreter.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>

Interpreter::Interpreter(Tokenizer& t)
{
    auto itr = t.AllTokens().cbegin();
}

Interpreter::~Interpreter()
{ }

bool Interpreter::Equals(const std::string& a, const std::string& b)
{
    std::string aa = a, bb = b;
    std::transform(aa.begin(), aa.end(), aa.begin(), ::tolower);
    std::transform(bb.begin(), bb.end(), bb.begin(), ::tolower);
    return aa == bb;
}
