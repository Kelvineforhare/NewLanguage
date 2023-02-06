// #ifndef REGEX_H
// #define REGEX_H

// #include <iostream>
// #include <set>
// #include <string>
// #include <vector>
// #include <memory>


// class REGEX{
//     friend bool operator==(std::shared_ptr<REGEX> a,std::shared_ptr<REGEX> b);
//     public:
//     const REGEX * reg;
//     virtual ~REGEX() = default;
//     virtual bool nullable() const = 0;
//     virtual std::shared_ptr<REGEX> der(char c) = 0;
//     virtual std::string str() const = 0;
//     virtual std::shared_ptr<REGEX> simp() = 0;
//     virtual bool isZero() const {return false;}
//     virtual bool isOne() const {return false;}
//     virtual std::shared_ptr<Val> mkeps() const;
// };

// class ZERO : public REGEX{
// public:
//     bool nullable() const;
//     std::shared_ptr<REGEX> der(char c);
//     std::string str() const;
//     std::shared_ptr<REGEX> simp();
//     bool isZero() const;
// };

// class ONE : public REGEX{
// public:
//     bool nullable() const;
//     std::shared_ptr<REGEX> der(char c);
//     std::string str() const;
//     std::shared_ptr<REGEX> simp();
//     bool isOne() const;
//     std::shared_ptr<Val> mkeps() const;
// };

// class CHAR : public REGEX{
// private:
//     char c;
//     public:
//     CHAR(char in);
//     bool nullable() const;
//     std::shared_ptr<REGEX> der(char d);
//     std::string str() const;
//     std::shared_ptr<REGEX> simp();
// };

// #endif