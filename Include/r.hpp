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
        std::shared_ptr<REGEX> getr1();
        std::shared_ptr<REGEX> getr2();
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