#ifndef V_HPP
#define V_HPP

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <iostream>


using std::cout;
using std::set;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::move;

//https://www.youtube.com/watch?v=JVFjEJidilE
//https://stackoverflow.com/questions/56842570/how-to-use-derived-parameter-in-an-overriding-function-without-dynamic-casting
class REGEX;

class Val{
    public:
        virtual std::string str() const = 0;
        
        virtual std::shared_ptr<Val> inj(std::shared_ptr<REGEX> r, char c) const;
};


class Empty : public Val{
    public:
        std::string str()const;
        //Cannot let inj take subclass of regex as input for other classes so have to use dynamic casting
        std::shared_ptr<Val> inj(std::shared_ptr<REGEX> r, char c) const override;
};


class Chr : public Val{
    private:
        char c;
    public:
        Chr(char in){
            c = in;
        }

        std::string str()const;
};


class Left : public Val{
    private:
        std::shared_ptr<Val> v;
    public:
        Left(std::shared_ptr<Val> in):v(in){}

        std::string str()const;
        std::shared_ptr<Val> inj(std::shared_ptr<REGEX> r, char c) const override;
};


class Right : public Val{
    private:
        std::shared_ptr<Val> v;
    public: 
        Right(std::shared_ptr<Val> in):v(in){}

        std::string str()const;
        std::shared_ptr<Val> inj(std::shared_ptr<REGEX> r,char c) const override;
};


class Sequ : public Val{
    private:
        std::shared_ptr<Val> v1;
        std::shared_ptr<Val> v2;
    public:
        Sequ(std::shared_ptr<Val> in,std::shared_ptr<Val> in2):v1(in),v2(in2){}

        std::string str()const;
        std::shared_ptr<Val> getr1();
        std::shared_ptr<Val> getr2();
};


class Stars : public Val{
    private:
        std::vector<std::shared_ptr<Val>> v;
    public: 
        Stars(std::vector<std::shared_ptr<Val>> in):v(in){}

        std::string str()const;
};


class Rec : public Val{
    private:
        std::shared_ptr<Val> v;
        std::string s;
    public: 
        Rec(std::string sin,std::shared_ptr<Val> in):v(in),s(sin){}

        std::string str()const;
};


class Plus : public Val{
    private:
        std::vector<std::shared_ptr<Val>> v;
    public: 
        Plus(std::vector<std::shared_ptr<Val>> in):v(in){}

        std::string str()const;
};


class Ntimes : public Val{
     private:
        std::vector<std::shared_ptr<Val>> v;
    public: 
        Ntimes(std::vector<std::shared_ptr<Val>> in):v(in){}

        std::string str()const;
};




#endif