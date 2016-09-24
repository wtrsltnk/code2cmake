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
    Token();
    Token(const Token& other);
    Token(TokenTypes::eTokenTypes type, const std::string& token, int line, int linepos, int filepos);
    virtual ~Token();

    Token& operator = (const Token& other);

    TokenTypes::eTokenTypes type;
    std::string token;
    int line;
    int linepos;
    int filepos;
};

class Tokenizer
{
    std::ifstream _in;
    char _ch;
    int filepos;
    int line;
    int linepos;
    std::vector<Token> _alltokens;

    void proceed(int count = 1);
    Token returnToken(const std::string& token, TokenTypes::eTokenTypes type = TokenTypes::Other);
    static bool isDelimiterChar(char c);
    static bool isOperatorChar(char c);
    static bool isGroupChar(char c, char& closeOn);

public:
    Tokenizer(const std::string& filename);
    virtual ~Tokenizer();

    Token NextToken();
    const std::vector<Token>& AllTokens();
};

#endif // _TOKENIZER_H_
