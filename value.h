#ifndef Val_h
#define Val_h

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

#endif