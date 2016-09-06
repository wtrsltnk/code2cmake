#include "tokenizer.h"
#include <fstream>
#include <iostream>

Tokenizer::Tokenizer(const std::string& filename)
    : _inputstream(filename.c_str()), c('\0'), filepos(0), line(0), linepos(0)
{
    if (this->_inputstream.good())
        this->_inputstream.get(c);
}

Tokenizer::~Tokenizer()
{ }

void Tokenizer::proceed(int count)
{
    while (count-- > 0 && this->_inputstream.good())
    {
        this->_inputstream.get(c);
        filepos++;
        linepos++;
        if (c == '\n') { line++; linepos = 0; }
    }
    if (!this->_inputstream.good())
        c = '\0';
}

Token Tokenizer::returnToken(const std::string& token, TokenTypes::eTokenTypes type)
{
    Token t;
    t.type = type;
    t.filepos = filepos;
    t.line = line;
    t.linepos = linepos;
    t.token = token;

    if (token != "")
    {
        this->_alltokens.push_back(t);
    }

    return t;
}

bool IsDelimChar(char c);
bool IsOperatorChar(char c);
bool IsGroupChar(char c, char& closeOn);

Token Tokenizer::NextToken()
{
    std::string token;

    // Skip all space characters
    if (c <= ' ')
    {
        while (this->_inputstream.good())
        {
            if (c > ' ')
                break;

            this->proceed();
        }
    }

    // Handle comments
    if (c == '/')
    {
        char p = this->_inputstream.peek();
        while (this->_inputstream.good())
        {
            if (p == '/' && c == '\n')
                return returnToken(token, TokenTypes::Comment);

            if (p == '*' && c == '*' && this->_inputstream.peek() == '/')
            {
                this->proceed(); this->proceed();
                token += "*/";
                return returnToken(token, TokenTypes::Comment);
            }

            token += c;
            this->proceed();
        }
    }

    // Handle possible operators
    if (IsOperatorChar(c) && IsOperatorChar(this->_inputstream.peek()))
    {
        while (IsOperatorChar(this->_inputstream.peek()))
        {
            token += c;
            proceed();
        }
        token += c;
        proceed();
        return returnToken(token, TokenTypes::Operator);
    }

    // Handle group chars like quotes etc
    char e;
    if (IsGroupChar(c, e))
    {
        token += c;
        this->proceed();
        while (this->_inputstream.good())
        {
            if (c == e)
            {
                token += c;
                this->proceed();
                return returnToken(token, TokenTypes::String);
            }

            token += c;
            this->proceed();
        }
    }

    // Handle delimiting characters
    if (IsDelimChar(c))
    {
        std::string tok(1, c);
        this->proceed();
        return returnToken(tok);
    }

    // Fill up the token until we see any delimiting characters
    while (c != '\0')
    {
        if (IsDelimChar(c))
        {
            return returnToken(token);
        }
        else
        {
            token += c;
            this->proceed();
        }
    }

    return returnToken(token);
}

const std::vector<Token>& Tokenizer::AllTokens()
{
    if (!this->_inputstream.eof())
    {
        auto token = NextToken();
        while (token.token != "")
        {
            token = NextToken();
        }
    }

    return this->_alltokens;
}

static char delimchars[] = { ' ',
    '(', ')',
    '[', ']',
    '{', '}',
    '<', '>',
    '|', '\\', '/',
    '.', ',', ';', ':',
    '&', '$','?', '!',
     '+', '=', '-', '*', '^', '~', '%'
};

static char groupchars[][2] = {
    { '\"', '\"' },
    { '\'', '\'' }
};

static char operatorchars[] = {
    '+', '=', '-', '*', '!', '<', '>', '&', '|', '^', ':'
};

bool IsDelimChar(char c)
{
    for (unsigned int i = 0; i < sizeof(delimchars); i++)
    {
        if (delimchars[i] == c)
            return true;
    }

    return false;
}

bool IsOperatorChar(char c)
{
    for (unsigned int i = 0; i < sizeof(operatorchars); i++)
    {
        if (operatorchars[i] == c)
            return true;
    }

    return false;
}

bool IsGroupChar(char c, char& closeOn)
{
    int count = sizeof(groupchars) / (sizeof(char) * 2);
    for (int i = 0; i < count; i++)
    {
        if (groupchars[i][0] == c)
        {
            closeOn = groupchars[i][1];
            return true;
        }
    }
    return false;
}
