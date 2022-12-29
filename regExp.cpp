#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <sys/resource.h>
#include <memory>
using std::cout;
using std::set;
using std::string;
using std::vector;
using std::unique_ptr;

//use gbd


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

unique_ptr<ZERO> zero();
unique_ptr<ONE> one();
unique_ptr<ALT> alt(unique_ptr<REGEX>,unique_ptr<REGEX>);
SEQ & seq(const REGEX & ,const  REGEX &);
STAR & star(const REGEX & r1);
PLUS & plus(const REGEX & r1);
NTIMES & ntimes(const REGEX &, int);
RANGE & range(const set<char>);
unique_ptr<CHAR> cha(char);
ID & id(string,const REGEX &);


class REGEX{
    
    friend bool operator==( REGEX& a,  REGEX& b){
    if (a.str() == b.str()){
        return true;
    }
    return false;
    }

    friend bool operator==(const REGEX& a,const  REGEX& b){
    if (a.str() == b.str()){
        return true;
    }
    return false;
    }

    // friend bool operator==(unique_ptr<REGEX> a,unique_ptr<REGEX> b){
    // if (a->str() == b->str()){
    //     return true;
    // }
    // return false;
    // }

    public:
        const REGEX * reg;
        virtual ~REGEX() = default;
        virtual bool nullable() const = 0;
        virtual unique_ptr<REGEX> der(char c) const = 0;
        virtual string str() const = 0;
        virtual unique_ptr<REGEX> simp() = 0;
        virtual bool isZero() const {return false;}
        virtual bool isOne() const {return false;}
        //virtual void del() = 0; possibly make fuction to delete pointers
};

class ZERO : public REGEX{
    public:

        bool nullable() const{
            return false;
        }

        unique_ptr<REGEX> der(char c) const{
            return zero();
        }
        
        string str() const {
            return "ZERO";
        }

        unique_ptr<REGEX> simp() {
            return zero();
        }

        bool isZero() const {return true;}
};

class ONE : public REGEX{
    public:

        bool nullable() const{
            return true;
        }

       unique_ptr<REGEX> der(char c) const{
            return zero();
        }

        string str() const{
            return "ONE";
        }

        unique_ptr<REGEX> simp(){
            return one();
        }

        bool isOne()const {return true;}
};

class CHAR : public REGEX{
    private: 
        char c;
        unique_ptr<CHAR> chr;
    public:
        CHAR(char in){
            c = in;
        }

        void setPointer(unique_ptr<CHAR> pointer){
            chr = std::move(pointer);
        }

        CHAR * getPointer(){
            return chr.get();
        }

        bool nullable() const{
            return false;
        }

        unique_ptr<REGEX> der(char d) const{
            if(c == d){
                return one();
            }
            return zero();
        }

        string str() const{
            string s(1, c);
            return s;
        }

        unique_ptr<REGEX> simp() {
            return std::move(chr);
        }
};

class ALT : public REGEX{
    public:
        unique_ptr<REGEX> r1;
        unique_ptr<REGEX> r2;

        ALT(unique_ptr<REGEX> re1,unique_ptr<REGEX> re2):r1(std::move(re1)),r2(std::move(re2)){}

        bool nullable() const{
            return r1->nullable() || r2->nullable();
        }

        unique_ptr<REGEX> der(char c) const{
            cout << r1->str() << "\n";
            cout << r2->str() << "\n";
            return alt(r1->der(c),r2->der(c));
        }

        string str() const{
            return "(" + r1->str() + " || " + r2->str() + ")";
        }

        unique_ptr<REGEX> simp() {
            unique_ptr<REGEX> n1 = r1->simp();
            unique_ptr<REGEX> n2 = r2->simp();

            cout << "p1: " << n1->str() << "\n";
            cout << "p2: " << n2->str() << "\n";
            
            if(n1->isZero()){
                return n2;
            }
            if(n2->isZero()){
                return n1;
            }
            if(n1 == n2){
                return n1;
            }
            return alt(std::move(n1),std::move(n2));
        }
};

// class SEQ : public REGEX{
//     public:
//         const REGEX & r1;
//         const REGEX & r2;


//         SEQ(const REGEX & re1, const REGEX & re2):r1(re1),r2(re2){}

//         bool nullable() const {
//             return r1.nullable() && r2.nullable();
//         }

//         REGEX & der(char c) const{
//             if(r1.nullable()){
//                 return alt(seq(r1.der(c),r2),r2.der(c));
//             }
//             return seq(r1.der(c),r2);
//         }

//         string str() const{
//             return "(" +  r1.str() + " && " + r2.str() + ")";
//         }

//         const REGEX & simp() const{
//             const REGEX & n1 =  r1.simp();
//             const REGEX & n2 = r2.simp();

//             //cout << n1.str() << " " << n2.str() << "\n";
//             if(r1.isZero() || r2.isZero()){
//                 return zero();
//             }
//             if(r1.isOne()){
//                 return n2;
//             }
//             if(r2.isOne()){
//                 return n1;
//             }
//             return seq(n1,n2); 
//         }
// };

// class STAR : public REGEX{
//     public:
//         const REGEX & r1;
//         STAR * s;

//         STAR(const REGEX & re1):r1(re1){}

//         bool nullable() const{
//             return true;
//         }

//         REGEX & der(char c) const{
//             //STAR ret = seq(r1.der(c),star(r1));
//             return seq(r1.der(c),star(r1));
//         }

//          string str() const{
//             return r1.str() + "*";
//         }

//         const REGEX & simp() const{
//             return *s;
//         }
// };

// class PLUS : public REGEX{
//     public:
//         const REGEX & r1;
//         PLUS * pl;

//         PLUS(const REGEX & re1):r1(re1){}

//         bool nullable() const{
//             return r1.nullable();
//         }

//         REGEX & der(char c) const{
//             //STAR ret = seq(r1.der(c),star(r1));
//             return seq(r1.der(c),star(r1));
//         }

//         string str() const {
//             return r1.str() + "+";
//         }

//          REGEX & simp() const{
//             return *pl;
//         }
// };

// class RANGE : public REGEX{
//     public:
//         set<char> s;
//         RANGE * ra;

//         RANGE(set<char> in){
//             s = in;
//         }

//         bool nullable() const{
//             return false;
//         }

//         REGEX & der(char c) const{
//             if(s.find(c) != s.end()){
//                 //ONE o;
//                 return one();
//             }
//             //ZERO z;
//             return zero();
//         }

//         string str() const {
//             string r = "[ ";
//             for (char const& c : s){
//                 r + c + ", ";
//             }
//             return r;
//         }
        
//          REGEX & simp() const{
//             return *ra;
//         }
// };

// class NTIMES : public REGEX{
//     public:
//         const REGEX & r1;
//         int i;
//         NTIMES * nt;

//         NTIMES(const REGEX & re1,int num):r1(re1),i(num){}

//         bool nullable() const{
//             return (i == 0)?true:r1.nullable();
//         }

//         REGEX & der(char c) const{
//             if(i == 0){
//                 //ZERO z;
//                 return zero();
//             }
//             //SEQ s = seq(r1.der(c),ntimes(r1,i-1));
//             return seq(r1.der(c),ntimes(r1,i-1));
//         }   

//         string str() const {
            
//             return r1.str() + "{" + std::to_string(i) + "}";
//         }  

//          REGEX & simp() const{
//             return *nt;
//         }   
// };

// class ID : public REGEX{
//     public:
//         const REGEX & r1;
//         string s;
//         ID * i;

//         ID(const REGEX & re1,string str):r1(re1),s(str){}

//         bool nullable() const{
//             return r1.nullable();
//         }

//         REGEX & der(char c) const{
//             return r1.der(c);
//         } 

//         string str() const {
//             return s + ": (" + r1.str() + ")";
//         }     

//          REGEX & simp() const {
//             return *i;
//         }
// };

unique_ptr<ONE> one(){
    unique_ptr<ONE> one (new ONE());
    return one;
}

unique_ptr<ZERO> zero(){
    unique_ptr<ZERO> zero(new ZERO());
    return zero;
}

unique_ptr<CHAR> cha(char c){
    unique_ptr<CHAR> p(new CHAR(c));
    return p;
}

unique_ptr<ALT> alt(unique_ptr<REGEX> r1,unique_ptr<REGEX> r2){
    unique_ptr<ALT> a (new ALT(std::move(r1),std::move(r2)));
    return a;
}

// SEQ & seq(const REGEX & r1,const REGEX & r2){
//     SEQ * a = new SEQ(r1,r2);
//     return *a;
// }

// STAR & star(const REGEX & r1){
//     STAR * a = new STAR(r1);
//     a->s = a;
//     return *a;
// }

// PLUS & plus(const REGEX & r1){
//     PLUS * p = new PLUS(r1);
//     p->pl = p;
//     return *p;
// }

// NTIMES & ntimes(const REGEX & r1, int i){
//     NTIMES * n = new NTIMES(r1,i);
//     n->nt = n;
//     return *n;
// }

// RANGE & range(set<char> s){
//     RANGE * r = new RANGE(s);
//     r->ra = r;
//     return *r;
// }

// ID & id(string str,const REGEX & r1){
//     ID * id = new ID(r1,str);
//     id->i = id;
//     return *id;
// }

unique_ptr<REGEX> der(char c,unique_ptr<REGEX> r){
    return r->der(c);
}


unique_ptr<REGEX> ders(const vector<char> & str, unique_ptr<REGEX> r,int i){
    if(i >= str.size()){
        return r;
    }
    unique_ptr<REGEX> s = der(str[i],std::move(r))->simp();
    ++i;
    for(;i < str.size();++i){
        //cout << "before: " << s->str() << " " << str[i] <<  "\n ";
        s = der(str[i],std::move(s))->simp();
        //cout << "simp: " << s->str() <<  "\n ";
    }
    return s;
}

bool matcher(unique_ptr<REGEX> r,const string & s)
{
    vector<char> v(s.begin(), s.end());
    return ders(v,std::move(r),0)->nullable();
}

//check memory
int main(){
    auto test = alt(cha('c'),cha('b'));
    auto test1 = alt(cha('c'),cha('b'));
    cout << (test1 == test) << "\n";
    cout << matcher(std::move(test), string(3, 'b')) << "\n";

    // ALT evil2 = alt(star(star(cha('a'))),cha('b'));  

    // for(int i = 0; i < 6000000;i+=500000) {
    //    cout << matcher(evil2, string(i, 'a')) << "\n";
    // }
    return 0;
}

