#ifndef REGEX_H
#define REGEX_H

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <iostream>
#include "Exceptions/LexingError.cpp"

using std::cout;
using std::set;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::move;

class REGEX;
class ZERO;
class ONE;
class CHAR;
class RANGE;
class ALT;
class SEQ;
class STAR;
class PLUS;
class NTIMES;
class ID;


class Val{
    public:
        virtual string str() const = 0;
        virtual shared_ptr<Val> inj(shared_ptr<REGEX> r, char c) const;
};

class Empty : public Val{
    public:
        string str()const;
        shared_ptr<Val> inj(shared_ptr<CHAR> r,char c) const;
};

class Chr : public Val{
    private:
        char c;
    public:
        Chr(char in);

        string str()const;
};

class Left : public Val{
    private:
        shared_ptr<Val> v;
    public:
        Left(shared_ptr<Val> in);

        string str()const;
};

class Right : public Val{
    private:
        shared_ptr<Val> v;
    public: 
        Right(shared_ptr<Val> in);

        string str()const;
};

class Sequ : public Val{
    private:
        shared_ptr<Val> v1;
        shared_ptr<Val> v2;
    public:
        Sequ(shared_ptr<Val> in,shared_ptr<Val> in2);

        string str()const;
};

class Stars : public Val{
    private:
        vector<shared_ptr<Val>> v;
    public: 
        Stars(vector<shared_ptr<Val>> in);

        string str()const;
};

class Rec : public Val{
    private:
        shared_ptr<Val> v;
        string s;
    public: 
        Rec(string sin,shared_ptr<Val> in);

        string str()const;
};

class Plus : public Val{
    private:
        vector<shared_ptr<Val>> v;
    public: 
        Plus(vector<shared_ptr<Val>> in);

        string str()const;
};

class Ntimes : public Val{
     private:
        vector<shared_ptr<Val>> v;
    public: 
        Ntimes(vector<shared_ptr<Val>> in);

        string str()const;
};

class REGEX{
    friend bool operator==(std::shared_ptr<REGEX> a,std::shared_ptr<REGEX> b);
    public:
        virtual ~REGEX() = default;
        virtual bool nullable() const = 0;
        virtual std::shared_ptr<REGEX> der(char c) = 0;
        virtual std::string str() const = 0;
        virtual std::shared_ptr<REGEX> simp() = 0;
        virtual bool isZero() const {return false;}
        virtual bool isOne() const {return false;}
        virtual std::shared_ptr<Val> mkeps() const;
};

class ZERO : public REGEX{
public:
    bool nullable() const;
    std::shared_ptr<REGEX> der(char c);
    std::string str() const;
    std::shared_ptr<REGEX> simp();
    bool isZero() const;
};

class ONE : public REGEX{
public:
    bool nullable() const;
    std::shared_ptr<REGEX> der(char c);
    std::string str() const;
    std::shared_ptr<REGEX> simp();
    bool isOne() const;
    std::shared_ptr<Val> mkeps() const;
};

class CHAR : public REGEX{
private:
    char c;
    public:
    CHAR(char in);
    bool nullable() const;
    std::shared_ptr<REGEX> der(char d);
    std::string str() const;
    std::shared_ptr<REGEX> simp();
};

class ALT : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
        std::shared_ptr<REGEX> r2;
    public:
        ALT(std::shared_ptr<REGEX> re1, std::shared_ptr<REGEX> re2): r1(re1), r2(re2) {}
        bool nullable() const;
        std::shared_ptr<REGEX> der(char c);
        std::string str() const;
        std::shared_ptr<REGEX> simp();
        std::shared_ptr<Val> mkeps() const;
};

class SEQ : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
        std::shared_ptr<REGEX> r2;
    public:
        SEQ(std::shared_ptr<REGEX> re1, std::shared_ptr<REGEX> re2): r1(re1), r2(re2) {}
        bool nullable() const;
        std::shared_ptr<REGEX> der(char c);
        std::string str() const;
        std::shared_ptr<REGEX> simp();
        std::shared_ptr<Val> mkeps() const;
};

class STAR : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
    public:
        STAR(std::shared_ptr<REGEX> re1): r1(re1) {}
        bool nullable() const;
        std::shared_ptr<REGEX> der(char c);
        std::string str() const;
        std::shared_ptr<REGEX> simp();
        std::shared_ptr<Val> mkeps() const;
};

class PLUS : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
    public:
        PLUS(std::shared_ptr<REGEX> re1): r1(re1) {}
        bool nullable() const;
        std::shared_ptr<REGEX> der(char c);
        std::string str() const;
        std::shared_ptr<REGEX> simp();
        std::shared_ptr<Val> mkeps() const;
};

class NTIMES : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
        int i;
    public:
        NTIMES(std::shared_ptr<REGEX> re1): r1(re1) {}
        bool nullable() const;
        std::shared_ptr<REGEX> der(char c);
        std::string str() const;
        std::shared_ptr<REGEX> simp();
        std::shared_ptr<Val> mkeps() const;
};

class RANGE : public REGEX{
private:
    std::set<char> c;
public:
    RANGE(std::set<char> in);
    bool nullable() const;
    std::shared_ptr<REGEX> der(char d);
    std::string str() const;
    std::shared_ptr<REGEX> simp();
};

class ID : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
        std::string s;
    public:
        ID(std::shared_ptr<REGEX> re1): r1(re1) {}
        bool nullable() const;
        std::shared_ptr<REGEX> der(char c);
        std::string str() const;
        std::shared_ptr<REGEX> simp();
        std::shared_ptr<Val> mkeps() const;
};

#endif