#include "Include/v.h"
#include "Exceptions/LexingError.cpp"


class ZERO;
class ONE;
//class ALT;
class SEQ;
class STAR;
class PLUS;
class NTIMES;
class RANGE;
class ID;

shared_ptr<ZERO> zero();
shared_ptr<ONE> one();
shared_ptr<CHAR> cha(char);
shared_ptr<ALT> alt(shared_ptr<REGEX>,shared_ptr<REGEX>);
shared_ptr<SEQ> seq(shared_ptr<REGEX>,shared_ptr<REGEX>);
shared_ptr<STAR> star(shared_ptr<REGEX>);
shared_ptr<PLUS> plus(shared_ptr<REGEX>);
shared_ptr<NTIMES> ntimes(shared_ptr<REGEX>, int);
shared_ptr<RANGE> range(set<char>);
shared_ptr<ID> id(string,shared_ptr<REGEX>);


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
        virtual shared_ptr<REGEX> der(char c) = 0;
        virtual string str() const = 0;
        virtual shared_ptr<REGEX> simp() = 0;
        virtual bool isZero() const {return false;}
        virtual bool isOne() const {return false;}
        virtual shared_ptr<Val> mkeps() const {
            throw LexingError();
        }
};

class ZERO : public REGEX{
    public:
        bool nullable() const{
            return false;
        }

        shared_ptr<REGEX> der(char c) {
            return {zero()};
        }
        
        string str() const {
            return "ZERO";
        }

        shared_ptr<REGEX> simp() {
            return {zero()};
        }

        bool isZero() const {return true;}

        
};

class ONE : public REGEX{
    public:
        bool nullable() const{
            return true;
        }

       shared_ptr<REGEX> der(char c) {
            return {zero()};
        }

        string str() const{
            return "ONE";
        }

        shared_ptr<REGEX> simp(){
            return {one()};
        }

        bool isOne()const {return true;}

        shared_ptr<Val> mkeps() const {
            shared_ptr<Empty> e(new Empty());
            return e;
        }
};

class CHAR : public REGEX{
    private: 
        char c;
    public:
        CHAR(char in){
            c = in;
        }

        bool nullable() const{
            return false;
        }

        shared_ptr<REGEX> der(char d){
            if(c == d){
                return one();
            }
            return zero();
        }

        string str() const{
            string s(1, c);
            return s;
        }

        shared_ptr<REGEX> simp() {
            return {cha(c)};
        }
};

class ALT : public REGEX{
    private: 
        shared_ptr<REGEX> r1;
        shared_ptr<REGEX> r2;
    public:
        ALT(shared_ptr<REGEX> re1,shared_ptr<REGEX> re2):r1(re1),r2(re2){}

        bool nullable() const{
            return r1->nullable() || r2->nullable();
        }

        shared_ptr<REGEX> der(char c) {
            return {alt(r1->der(c),r2->der(c))};
        }

        string str() const{
            return "(" + r1->str() + " || " + r2->str() + ")";
        }

        shared_ptr<REGEX> simp() {
            shared_ptr<REGEX> n1 = r1->simp();
            shared_ptr<REGEX> n2 = r2->simp();
            
            if(n1->isZero()){
                return n2;
            }
            if(n2->isZero()){
                return n1;                                          
            }
            if(n1 == n2){
                return n1;
            }
            return alt(n1,n2);
        }
        
        shared_ptr<Val> mkeps() const {
            if(r1->nullable()){
                shared_ptr<Left> e(new Left(r1->mkeps()));
                return e;
            }
            shared_ptr<Right> e(new Right(r2->mkeps()));
            return e;
        }

        shared_ptr<REGEX> getr1()
        {
            return r1;
        }

        shared_ptr<REGEX> getr2()
        {
            return r2;
        }
};

class SEQ : public REGEX{
    private: 
        shared_ptr<REGEX> r1;
        shared_ptr<REGEX> r2;
    public:
        SEQ(shared_ptr<REGEX> re1,shared_ptr<REGEX> re2):r1((re1)),r2((re2)){}

        bool nullable() const {
            return r1->nullable() && r2->nullable();
        }

        shared_ptr<REGEX> der(char c){
           
            if(r1->nullable()){
                return alt(seq(r1->der(c),r2),r2->der(c));
            }
            return seq(r1->der(c),(r2));
        }

        string str() const{
            return "(" +  r1->str() + " && " + r2->str() + ")";
        }

        shared_ptr<REGEX> simp() {
            shared_ptr<REGEX> n1 = r1->simp();
            shared_ptr<REGEX> n2 = r2->simp();

            if(n1->isZero() || n2->isZero()){
                
                return zero();
            }
            if(n1->isOne()){
                return n2;
            }
            if(n2->isOne()){
                return n1;
            }
            return seq((n1),(n2)); 
        }

        shared_ptr<Val> mkeps() const {
            shared_ptr<Sequ> e(new Sequ(r1->mkeps(),r2->mkeps()));
            return e;
        }
};

class STAR : public REGEX{
    private:
         shared_ptr<REGEX> r1;

    public:
        STAR(shared_ptr<REGEX> re1):r1((re1)){}

        bool nullable() const{
            return true;
        }

        shared_ptr<REGEX> der(char c) {
            return seq(r1->der(c),star((r1)));
        }

        string str() const{
            return r1->str() + "*";
        }

        shared_ptr<REGEX> simp(){
            return star((r1));
        }

        shared_ptr<Val> mkeps() const {
            shared_ptr<Stars> e(new Stars(vector<shared_ptr<Val>>{}));
            return e;
        }
};

class PLUS : public REGEX{
    private:
         shared_ptr<REGEX> r1;
    public:
        PLUS(shared_ptr<REGEX> re1):r1(re1){}

        bool nullable() const{
            return r1->nullable();
        }

        shared_ptr<REGEX> der(char c){
            return seq(r1->der(c),star(r1));
        }

        string str() const {
            return r1->str() + "+";
        }

        shared_ptr<REGEX> simp(){
            return {plus(r1)};
        }

        shared_ptr<Val> mkeps() const {
            shared_ptr<Plus> e(new Plus(vector<shared_ptr<Val>>{r1->mkeps()}));
            return e;
        }
};

class NTIMES : public REGEX{
    private:
         shared_ptr<REGEX> r1;
         int i;

    public:
        NTIMES(shared_ptr<REGEX> re1,int num):r1(re1),i(num){}

        bool nullable() const{
            return (i == 0)?true:r1->nullable();
        }

        shared_ptr<REGEX> der(char c){
            if(i == 0){
                return zero();
            }
            return seq(r1->der(c),ntimes(r1,i-1));
        }   

        string str() const {
            return r1->str() + "{" + std::to_string(i) + "}";
        }  

        shared_ptr<REGEX> simp(){
            return ntimes(r1,i);
        }   

        shared_ptr<Val> mkeps() const {
            if(i == 0){
                shared_ptr<Ntimes> e(new Ntimes(vector<shared_ptr<Val>>{}));
                return e;
            }
            shared_ptr<Ntimes> e(new Ntimes(vector<shared_ptr<Val>>{r1->mkeps()}));
            return e;
        }
};

class RANGE : public REGEX{
    private:
        set<char> s;

    public:
        RANGE(set<char> in){
            s = in;
        }

        bool nullable() const{
            return false;
        }

        shared_ptr<REGEX> der(char c){
            if(s.find(c) != s.end()){
                return one();
            }
            return zero();
        }

        string str() const {
            string r = "[ ";
            for (char const& c : s){
                r = r + c + ", ";
            }
            r = r + "]";
            return r;
        }
        
        shared_ptr<REGEX> simp() {
            return range(s);
        }
};



class ID : public REGEX{
    private: 
        shared_ptr<REGEX> r1;
        string s;
    public:
        ID(shared_ptr<REGEX> re1,string str):r1(re1),s(str){}

        bool nullable() const{
            return r1->nullable();
        }

        shared_ptr<REGEX> der(char c) {
            return r1->der(c);
        } 

        string str() const {
            return s + ": (" + r1->str() + ")";
        }     

        shared_ptr<REGEX> simp()  {
            return {id(s,r1)};
        }

        shared_ptr<Val> mkeps() const {
            shared_ptr<Rec> e(new Rec(s,r1->mkeps()));
            return e;
        }
};

shared_ptr<ONE> one(){
    shared_ptr<ONE> one (new ONE());
    return one;
}

shared_ptr<ZERO> zero(){
    shared_ptr<ZERO> zero(new ZERO());
    return zero;
}

shared_ptr<CHAR> cha(char c){
    shared_ptr<CHAR> p(new CHAR(c));
    return p;
}

shared_ptr<ALT> alt(shared_ptr<REGEX> r1,shared_ptr<REGEX> r2){
    shared_ptr<ALT> a (new ALT((r1),(r2)));
    return a;
}

shared_ptr<SEQ> seq(shared_ptr<REGEX> r1,shared_ptr<REGEX> r2){
    unique_ptr<SEQ> seq(new SEQ((r1),(r2)));
    return seq;
}

shared_ptr<STAR> star(shared_ptr<REGEX> r1){
    shared_ptr<STAR> s(new STAR((r1)));
    return s;
}

shared_ptr<PLUS> plus(shared_ptr<REGEX> r1){
    shared_ptr<PLUS> p(new PLUS(r1));
    return p;
}

shared_ptr<NTIMES> ntimes(shared_ptr<REGEX> r1, int i){
    shared_ptr<NTIMES> n(new NTIMES(r1,i));
    return n;
}

shared_ptr<RANGE> range(set<char> s){
    shared_ptr<RANGE> r(new RANGE(s));
    return r;
}

shared_ptr<ID> id(string str,shared_ptr<REGEX> r1){
    shared_ptr<ID> id(new ID(r1,str));
    return id;
}


shared_ptr<REGEX> der(char c,shared_ptr<REGEX> r){
    return r->der(c);
}

shared_ptr<REGEX> ders(const vector<char> & str, shared_ptr<REGEX> r){
    int i = 0;
    if(i >= str.size()){
        return r;
    }
    shared_ptr<REGEX> s = der(str[i],r)->simp();
    ++i;
    for(;i < str.size();++i){
        s = der(str[i],s)->simp();
    }
    return s;
}

bool matcher(shared_ptr<REGEX> r,const string & s)
{
    vector<char> v(s.begin(), s.end());
    return ders(v,r)->nullable();
}

shared_ptr<REGEX> string2rexp(const string & s){
  if(s.size() == 0){
    return one();
  }
  if(s.size() == 1){
    return cha(s[0]);
  }
  shared_ptr<REGEX> ret = seq(cha(s[0]),cha(s[1]));
  for(int i = 2; i < s.size();++i){
        ret = seq(ret,cha(s[i]));
  }
  return ret;
}

shared_ptr<REGEX> stringList2rexp(const vector<string> & s){
  if(s.size() == 0){
    return one();
  }
  if(s.size() == 1){
    return string2rexp(s[0]);
  }
  shared_ptr<REGEX> ret = alt(string2rexp(s[0]),string2rexp(s[1]));
  for(int i = 2; i < s.size();++i){
        ret = alt(ret,string2rexp(s[i]));
  }
  return ret;
}