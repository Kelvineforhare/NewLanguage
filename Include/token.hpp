#pragma once
#include <string>
#include "syntaxTree.hpp"

class Token
{
    friend bool operator==(shared_ptr<Token> a, shared_ptr<Token> b)
    {
        if (a.get() && b.get())
        {
            auto& tok1 = *a.get();
            auto& tok2 = *b.get();
            if (typeid(tok1) == typeid(tok2) && a->toString() == b->toString())
            {
                return true;
            }
        }

        return false;
    }

public:
    virtual std::string toString() = 0;
};

class T_SEMI : public Token
{
    std::string toString() override
    {
        return "; ";
    }
};

class T_LPAREN : public Token
{
    std::string toString() override
    {
        return "( ";
    }
};

class T_RPAREN : public Token
{
    std::string toString() override
    {
        return ") ";
    }
};

class T_LBRACK : public Token
{
    std::string toString() override
    {
        return "{ ";
    }
};

class T_RBRACK : public Token
{
    std::string toString() override
    {
        return "} ";
    }
};

class T_COMMA : public Token
{
    std::string toString() override
    {
        return " , ";
    }
};

class T_ID : public Token
{
private:
    std::string s;

public:
    T_ID(std::string in)
    {
        s = in;
    }

    std::string getID()
    {
        return s;
    }
    std::string toString() override
    {
        return s + " ";
    }
};

class T_OP : public Token
{
private:
    std::string s;

public:
    T_OP(std::string in)
    {
        s = in;
    }
    std::string getOP()
    {
        return s;
    }
    std::string toString() override
    {
        return s + " ";
    }
};

class T_INT : public Token
{
private:
    int i;

public:
    T_INT(int in)
    {
        i = in;
    }
    int getInt()
    {
        return i;
    }
    std::string toString() override
    {
        return std::to_string(i) + " ";
    }
};

class T_KWD : public Token
{
private:
    std::string s;

public:
    T_KWD(std::string in)
    {
        s = in;
    }
    std::string getKWD()
    {
        return s;
    }
    std::string toString() override
    {
        return s + " ";
    }
};

class T_STR : public Token
{
private:
    std::string s;

public:
    T_STR(std::string in)
    {
        s = in;
    }
    std::string getStr()
    {
        return s;
    }
    std::string toString() override
    {
        return "'" + s + "' ";
    }
};

std::shared_ptr<Val> lex(std::shared_ptr<REGEX> r, string s, int i);
vector<pair<string, string>> lexing(std::shared_ptr<REGEX> r, string s);
shared_ptr<Token> getTokFromStr(pair<string, string> s);
vector<shared_ptr<Token>> tokenise(vector<pair<string, string>> input);
void printVector(vector<pair<string, string>> vec);
void printTokVector(vector<shared_ptr<Token>> vec);
vector<shared_ptr<Token>> getTokensFromLang(string input);

std::ostream &operator<<(std::ostream &os, vector<shared_ptr<Stmt>> const &p);
std::ostream &operator<<(std::ostream &os, map<string, int> const &p);
