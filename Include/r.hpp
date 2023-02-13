#ifndef REGEX_HPP
#define REGEX_HPP

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


class Val;
class LexingError;


class REGEX{
    friend bool operator==(shared_ptr<REGEX> a,shared_ptr<REGEX> b){
        if (a->str() == b->str()){
            return true;
        }
        return false;
    }
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
    bool nullable() const override;
    std::shared_ptr<REGEX> der(char c) override;
    std::string str() const override;
    std::shared_ptr<REGEX> simp() override;
    bool isZero() const override;
};

class ONE : public REGEX{
public:
    bool nullable() const override;
    std::shared_ptr<REGEX> der(char c) override;
    std::string str() const override;
    std::shared_ptr<REGEX> simp() override;
    bool isOne() const override;
    std::shared_ptr<Val> mkeps() const override;
};

class CHAR : public REGEX{
private:
    char c;
public:
    CHAR(char in){
         c = in;
    }
    bool nullable() const override;
    std::shared_ptr<REGEX> der(char d) override;
    std::string str() const override;
    std::shared_ptr<REGEX> simp() override;
};

class ALT : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
        std::shared_ptr<REGEX> r2;
    public:
        ALT(std::shared_ptr<REGEX> re1, std::shared_ptr<REGEX> re2): r1(re1), r2(re2) {}
        bool nullable() const override;
        std::shared_ptr<REGEX> der(char c) override;
        std::string str() const override;
        std::shared_ptr<REGEX> simp() override;
        std::shared_ptr<Val> mkeps() const override;
        std::shared_ptr<REGEX> getr1();
        std::shared_ptr<REGEX> getr2();
};

class SEQ : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
        std::shared_ptr<REGEX> r2;
    public:
        SEQ(std::shared_ptr<REGEX> re1, std::shared_ptr<REGEX> re2): r1(re1), r2(re2) {}
        bool nullable() const override;
        std::shared_ptr<REGEX> der(char c) override;
        std::string str() const override;
        std::shared_ptr<REGEX> simp() override;
        std::shared_ptr<Val> mkeps() const override;
        std::shared_ptr<REGEX> getr1();
        std::shared_ptr<REGEX> getr2();
};

class STAR : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
    public:
        STAR(std::shared_ptr<REGEX> re1): r1(re1) {}
        bool nullable() const override;
        std::shared_ptr<REGEX> der(char c) override;
        std::string str() const override;
        std::shared_ptr<REGEX> simp() override;
        std::shared_ptr<Val> mkeps() const override; 
        std::shared_ptr<REGEX> getr() const;
};

class PLUS : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
    public:
        PLUS(std::shared_ptr<REGEX> re1): r1(re1) {}
        bool nullable() const override;
        std::shared_ptr<REGEX> der(char c) override;
        std::string str() const override;
        std::shared_ptr<REGEX> simp() override;
        std::shared_ptr<Val> mkeps() const override;
        std::shared_ptr<REGEX> getr() const;
};

class NTIMES : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
        int i;
    public:
        NTIMES(std::shared_ptr<REGEX> re1,int num): r1(re1) , i(num){}
        bool nullable() const override;
        std::shared_ptr<REGEX> der(char c) override;
        std::string str() const override;
        std::shared_ptr<REGEX> simp() override;
        std::shared_ptr<Val> mkeps() const override;
        std::shared_ptr<REGEX> getr() const;
};

class RANGE : public REGEX{
private:
    std::set<char> s;
public:
    RANGE(std::set<char> in){
        s = in;
    }
    bool nullable() const override;
    std::shared_ptr<REGEX> der(char d) override;
    std::string str() const override;
    std::shared_ptr<REGEX> simp() override;
};

class ID : public REGEX {
    private:
        std::shared_ptr<REGEX> r1;
        std::string s;
    public:
        ID(std::shared_ptr<REGEX> re1,std::string str): r1(re1),s(str) {}
        bool nullable() const override;
        std::shared_ptr<REGEX> der(char c) override;
        std::string str() const override;
        std::shared_ptr<REGEX> simp() override;
        std::shared_ptr<Val> mkeps() const override;
        std::string getStr() const;
        std::shared_ptr<REGEX> getr() const;
};

std::shared_ptr<ONE> one();
std::shared_ptr<ZERO> zero();
std::shared_ptr<CHAR> cha(char c);
std::shared_ptr<ALT> alt(std::shared_ptr<REGEX> r1,std::shared_ptr<REGEX> r2);
std::shared_ptr<SEQ> seq(std::shared_ptr<REGEX> r1,std::shared_ptr<REGEX> r2);
std::shared_ptr<STAR> star(std::shared_ptr<REGEX> r1);
std::shared_ptr<PLUS> plus(std::shared_ptr<REGEX> r1);
std::shared_ptr<NTIMES> ntimes(std::shared_ptr<REGEX> r1, int i);
std::shared_ptr<RANGE> range(set<char> s);
std::shared_ptr<ID> id(string str,std::shared_ptr<REGEX> r1);
shared_ptr<REGEX> der(char c,shared_ptr<REGEX> r);
shared_ptr<REGEX> ders(const vector<char> & str, shared_ptr<REGEX> r);
bool matcher(shared_ptr<REGEX> r,const string & s);
shared_ptr<REGEX> string2rexp(const string & s);
shared_ptr<REGEX> stringList2rexp(const vector<string> & s);

#endif