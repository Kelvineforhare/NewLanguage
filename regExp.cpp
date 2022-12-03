#include <iostream>
#include <set>
#include <string>
#include <vector>
//CLASS REGEX
using std::cout;
using std::set;
using std::string;
using std::vector;


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

//fix function protyping
ZERO & zero();
ONE & one();
ALT & alt(const REGEX &,const REGEX &);
SEQ & seq(const REGEX & ,const REGEX &);
STAR & star(const REGEX & r1);
PLUS & plus(const REGEX & r1);
NTIMES & ntimes(const REGEX &, int);
RANGE & range(const set<char>);
CHAR & cha(char);
ID & id(string,const REGEX &);


class REGEX{
    public:
        virtual ~REGEX() = default;
        virtual bool nullable() const = 0;
        virtual REGEX & der(char c) const = 0;
        virtual string str() const = 0;
};

class ZERO : public REGEX{
    public:
        bool nullable() const{
            return false;
        }

        REGEX & der(char c) const{
            ZERO * z = new ZERO();
            return *(z);
        }
        
        string str() const {
            return "ZERO";
        }
};

class ONE : public REGEX{
    public:
        bool nullable() const{
            return true;
        }

        REGEX & der(char c) const{
            ZERO * z = new ZERO();
            return *(z);
        }

        string str() const{
            return "ONE";
        }
};

class CHAR : public REGEX{
    public:
        char c;

        CHAR(char in){
            c = in;
        }

        bool nullable() const{
            return false;
        }

        REGEX & der(char d) const{
            if(c == d){
                return one();
            }
            return zero();
        }

        string str() const{
            string s(1, c);
            return s;
        }
};

class ALT : public REGEX{
    public:
        const REGEX & r1;
        const REGEX & r2;

        ALT(const REGEX & re1,const REGEX & re2):r1(re1),r2(re2){}

        bool nullable() const{
            return r1.nullable() || r2.nullable();
        }

        REGEX & der(char c) const{
            //ALT ret = alt(r1.der(c),r2.der(c));
            return alt(r1.der(c),r2.der(c));
        }

        string str() const{
            return r1.str() + " || " + r2.str();
        }
};

class SEQ : public REGEX{
    public:
        const REGEX & r1;
        const REGEX & r2;

        SEQ(const REGEX & re1, const REGEX & re2):r1(re1),r2(re2){}

        bool nullable() const {
            return r1.nullable() && r2.nullable();
        }

        REGEX & der(char c) const{
            if(r1.nullable()){
                return alt(seq(r1.der(c),r2),r2.der(c));
            }
            //SEQ ret = seq(r1.der(c),r2);
            return seq(r1.der(c),r2);
        }

         string str() const{
            return r1.str() + " && " + r2.str();
        }
};

class STAR : public REGEX{
    public:
        const REGEX & r1;

        STAR(const REGEX & re1):r1(re1){}

        bool nullable() const{
            return true;
        }

        REGEX & der(char c) const{
            //STAR ret = seq(r1.der(c),star(r1));
            return seq(r1.der(c),star(r1));
        }

         string str() const{
            return r1.str() + "*";
        }
};

class PLUS : public REGEX{
    public:
        const REGEX & r1;

        PLUS(const REGEX & re1):r1(re1){}

        bool nullable() const{
            return r1.nullable();
        }

        REGEX & der(char c) const{
            //STAR ret = seq(r1.der(c),star(r1));
            return seq(r1.der(c),star(r1));
        }

        string str() const {
            return r1.str() + "+";
        }
};

class RANGE : public REGEX{
    public:
        set<char> s;

        RANGE(set<char> in){
            s = in;
        }

        bool nullable() const{
            return false;
        }

         REGEX & der(char c) const{
            if(s.find(c) != s.end()){
                //ONE o;
                return one();
            }
            //ZERO z;
            return zero();
        }

        string str() const {
            string r = "[ ";
            for (char const& c : s){
                r + c + ", ";
            }
            return r;
        }
        
};

class NTIMES : public REGEX{
    public:
        const REGEX & r1;
        int i;

        NTIMES(const REGEX & re1,int num):r1(re1),i(num){}

        bool nullable() const{
            return (i == 0)?true:r1.nullable();
        }

        REGEX & der(char c) const{
            if(i == 0){
                //ZERO z;
                return zero();
            }
            //SEQ s = seq(r1.der(c),ntimes(r1,i-1));
            return seq(r1.der(c),ntimes(r1,i-1));
        }   

        string str() const {
            
            return r1.str() + "{" + std::to_string(i) + "}";
        }     
};

class ID : public REGEX{
    public:
        const REGEX & r1;
        string s;

        ID(const REGEX & re1,string str):r1(re1),s(str){}

        bool nullable() const{
            return r1.nullable();
        }

        REGEX & der(char c) const{
            return r1.der(c);
        } 

        string str() const {
            return s + ": (" + r1.str() + ")";
        }     
};

ONE & one(){
    ONE * o = new ONE();
    return *(o);
}

ZERO & zero(){
    ZERO * o = new ZERO();
    return *(o);
}

CHAR & cha(char c){
    CHAR * ret = new CHAR(c);
    return *(ret);
}

ALT & alt(const REGEX & r1,const REGEX & r2){
    ALT * a = new ALT(r1,r2);
    return *(a);
}

SEQ & seq(const REGEX & r1,const REGEX & r2){
    SEQ * a = new SEQ(r1,r2);
    return *(a);
}

STAR & star(const REGEX & r1){
    STAR * a = new STAR(r1);
    return * (a);
}

PLUS & plus(const REGEX & r1){
    PLUS * p = new PLUS(r1);
    return *(p);
}

NTIMES & ntimes(const REGEX & r1, int i){
    NTIMES * n = new NTIMES(r1,i);
    return *(n);
}

RANGE & range(const set<char> s){
    RANGE * r = new RANGE(s);
    return *(r);
}

ID & id(string str,const REGEX & r1){
    ID * i = new ID(r1,str);
    return *(i);
}

REGEX & der(char c,REGEX & r){
    return r.der(c);
}


REGEX & ders(vector<char> str,REGEX & r){
    for (char c : str) {
        vector<char> rest = vector<char>(str.begin()+1,str.end());
        // REGEX & ret =  der(c,r);
        // cout << c << " " << ret.str() << "\n ";
        return ders(rest,der(c,r));
    }
    return r;
}

bool matcher(REGEX & r,string s)
{
    vector<char> v(s.begin(), s.end());
    return ders(v,r).nullable();
}


int main(){
    ONE o;
    

    //ALT evil2 = alt(star(star(chr('a'))),chr('b'));   
    vector<char> v{'a'};
    cout << matcher(star(cha('a')),"aaaaaaa")<< "\n";
    return 0;
}

