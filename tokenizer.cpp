#include "tokenizer.h"
#include <fstream>
#include <iostream>

Token::Token()
{ }

Token::Token(const Token& other)
    : type(other.type), token(other.token), line(other.line), linepos(other.linepos), filepos(other.filepos)
{ }

Token::Token(TokenTypes::eTokenTypes type, const std::string& token, int line, int linepos, int filepos)
    : type(type), token(token), line(line), linepos(linepos), filepos(filepos)
{ }

Token::~Token()
{ }

Token& Token::operator = (const Token& other)
{
    this->type = other.type;
    this->token = other.token;
    this->line = other.line;
    this->linepos = other.linepos;
    this->filepos = other.filepos;

    return *this;
}

Tokenizer::Tokenizer(const std::string& filename)
    : _in(filename.c_str()), _ch('\0'), filepos(0), line(0), linepos(0)
{
    if (this->_in.good())
        this->_in.get(this->_ch);
}

Tokenizer::~Tokenizer()
{ }

void Tokenizer::proceed(int count)
{
    while (count-- > 0 && this->_in.good())
    {
        this->_in.get(this->_ch);
        filepos++;
        linepos++;
        if (this->_ch == '\n') { line++; linepos = 0; }
    }
    if (!this->_in.good())
        this->_ch = '\0';
}

Token Tokenizer::returnToken(const std::string& token, TokenTypes::eTokenTypes type)
{
    Token t(type, token, line, linepos, filepos);

    if (token != "")
    {
        this->_alltokens.push_back(t);
    }

    return t;
}

Token Tokenizer::NextToken()
{
    std::string token;

    // Skip all space characters
    if (this->_ch <= ' ')
    {
        while (this->_in.good())
        {
            if (this->_ch > ' ')
                break;

            this->proceed();
        }
    }

    // Handle comments
    if (this->_ch == '/')
    {
        char p = static_cast<char>(this->_in.peek());
        while (this->_in.good())
        {
            if (p == '/' && this->_ch == '\n')
                return returnToken(token, TokenTypes::Comment);

            if (p == '*' && this->_ch == '*' && static_cast<char>(this->_in.peek()) == '/')
            {
                this->proceed(); this->proceed();
                token += "*/";
                return returnToken(token, TokenTypes::Comment);
            }

            token += this->_ch;
            this->proceed();
        }
    }

    // Handle possible operators
    if (Tokenizer::isOperatorChar(this->_ch) && Tokenizer::isOperatorChar(static_cast<char>(this->_in.peek())))
    {
        while (Tokenizer::isOperatorChar(static_cast<char>(this->_in.peek())))
        {
            token += this->_ch;
            proceed();
        }
        token += this->_ch;
        proceed();
        return returnToken(token, TokenTypes::Operator);
    }

    // Handle group chars like quotes etc
    char e;
    if (Tokenizer::isGroupChar(this->_ch, e))
    {
        token += this->_ch;
        this->proceed();
        while (this->_in.good())
        {
            if (this->_ch == e)
            {
                token += this->_ch;
                this->proceed();
                return returnToken(token, TokenTypes::String);
            }

            token += this->_ch;
            this->proceed();
        }
    }

    // Handle delimiting characters
    if (Tokenizer::isDelimiterChar(this->_ch))
    {
        std::string tok(1, this->_ch);
        this->proceed();
        return returnToken(tok);
    }

    // Fill up the token until we see any delimiting characters
    while (this->_ch != '\0')
    {
        if (Tokenizer::isDelimiterChar(this->_ch))
        {
            return returnToken(token);
        }
        else
        {
            token += this->_ch;
            this->proceed();
        }
    }

    return returnToken(token);
}

const std::vector<Token>& Tokenizer::AllTokens()
{
    if (!this->_in.eof())
    {
        auto token = NextToken();
        while (token.token != "")
        {
            token = NextToken();
        }
    }

    return this->_alltokens;
}

static char delimiterChars[] = { ' ',
    '(', ')', '[', ']', '{', '}', '<', '>',
    '|', '\\', '/', '&', '$',
    '.', ',', ';', ':', '?', '!',
    // '+', '=', '-', '*', '^', '~', '%'
};

bool Tokenizer::isDelimiterChar(char c)
{
    auto count = sizeof(delimiterChars);
    for (unsigned int i = 0; i < count; i++)
    {
        if (delimiterChars[i] == c) return true;
    }
    return false;
}

static char operatorChars[] = {
    '+', '=', '-', '*', '!', '<', '>', '&', '|', '^', ':'
};

bool Tokenizer::isOperatorChar(char c)
{
    auto count = sizeof(operatorChars);
    for (unsigned int i = 0; i < count; i++)
    {
        if (operatorChars[i] == c) return true;
    }
    return false;
}

static char groupChars[][2] = {
    { '\"', '\"' },
    { '\'', '\'' }
};

bool Tokenizer::isGroupChar(char c, char& closeOn)
{
    auto count = sizeof(groupChars) / 2;
    for (unsigned int i = 0; i < count; i++)
    {
        if (groupChars[i][0] == c)
        {
            closeOn = groupChars[i][1];
            return true;
        }
    }
    return false;
}
