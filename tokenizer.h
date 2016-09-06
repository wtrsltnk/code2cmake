#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <vector>
#include <string>
#include <fstream>

namespace TokenTypes
{
enum eTokenTypes
{
    Comment,
    String,
    Operator,
    Other
};
}

class Token
{
public:
    TokenTypes::eTokenTypes type;
    std::string token;
    int line;
    int linepos;
    int filepos;
};

class Tokenizer
{
    std::ifstream _inputstream;
    char c;
    int filepos;
    int line;
    int linepos;
    std::vector<Token> _alltokens;

    void proceed(int count = 1);
    Token returnToken(const std::string& token, TokenTypes::eTokenTypes type = TokenTypes::Other);
public:
    Tokenizer(const std::string& filename);
    virtual ~Tokenizer();

    Token NextToken();
    const std::vector<Token>& AllTokens();
};

#endif // _TOKENIZER_H_
