#include <iostream>
#include <set>
#include <string>
#include <vector>
using std::cout;
using std::set;
using std::string;
using std::vector;

//CHANGE SIMP SO THAT IT DOESNT CREATE NEW REGEX IN MEMEORY 


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
    friend bool operator==(const REGEX& a,const  REGEX& b){
    if (a.str() == b.str()){
        return true;
    }
    return false;
    }
    


    public:
        virtual ~REGEX() = default;
        virtual bool nullable() const = 0;
        virtual REGEX & der(char c) const = 0;
        virtual string str() const = 0;
        virtual REGEX & simp() const = 0;
        virtual bool isZero() const {return false;}
        virtual bool isOne() const {return false;}
        //virtual void del() = 0; make fuction to delete pointers
};

class ZERO : public REGEX{
    public:
        ZERO * z;

        bool nullable() const{
            return false;
        }

        REGEX & der(char c) const{
            return zero();
        }
        
        string str() const {
            return "ZERO";
        }

        REGEX & simp()const{
            return *z;
        }

        bool isZero() const {return true;}
};

class ONE : public REGEX{
    public:
        ONE * o;

        bool nullable() const{
            return true;
        }

        REGEX & der(char c) const{
            return zero();
        }

        string str() const{
            return "ONE";
        }

        REGEX & simp() const{
            return *o;
        }

        bool isOne()const {return true;}
};

class CHAR : public REGEX{
    public:
        CHAR * chr;
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

        REGEX & simp() const{
            return *chr;
        }
};

class ALT : public REGEX{
    public:
        REGEX & r1;
        REGEX & r2;
        ALT * al; 

        ALT( REGEX & re1, REGEX & re2):r1(re1),r2(re2){}

        bool nullable() const{
            return r1.nullable() || r2.nullable();
        }

        REGEX & der(char c) const{
            //ALT ret = alt(r1.der(c),r2.der(c));
            return alt(r1.der(c),r2.der(c));
        }

        string str() const{
            return "(" + r1.str() + " || " + r2.str() + ")";
        }

        REGEX & simp() const{
            REGEX & n1 = r1.simp();
            REGEX & n2 = r2.simp();

            cout << "n1" << n1.str() << "\n";
            cout << "n2" << n2.str() << "\n";

            if(n1.isZero()){
                return n2;
            }
            if(n2.isZero()){
                return n1;
            }
            if(n1 == n2){
                cout << "test";
                return n1;
            }
            al->r1 = n1;
            al->r2 = n2;
            //change so that r1 and r2 are pointer so you delete and change pointer
            return alt(n1,n2);
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
            return "(" +  r1.str() + " && " + r2.str() + ")";
        }

        REGEX & simp() const{
            REGEX & n1 = r1.simp();
            REGEX & n2 = r2.simp();

            if(n1.isZero() || n2.isZero()){
                return zero();
            }
            if(n1.isOne()){
                return n2;
            }
            if(n2.isOne()){
                return n1;
            }
            return seq(n1,n2);
        }
};

class STAR : public REGEX{
    public:
        const REGEX & r1;
        STAR * s;

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

        REGEX & simp() const{
            return *s;
        }
};

class PLUS : public REGEX{
    public:
        const REGEX & r1;
        PLUS * pl;

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

        REGEX & simp() const{
            return *pl;
        }
};

class RANGE : public REGEX{
    public:
        set<char> s;
        RANGE * ra;

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
        
        REGEX & simp() const{
            return *ra;
        }
};

class NTIMES : public REGEX{
    public:
        const REGEX & r1;
        int i;
        NTIMES * nt;

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

        REGEX & simp() const{
            return *nt;
        }   
};

class ID : public REGEX{
    public:
        const REGEX & r1;
        string s;
        ID * i;

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

        REGEX & simp() const{
            return *i;
        }
};

ONE & one(){
    ONE * one = new ONE();
    one->o = one;
    return *one;
}

ZERO & zero(){
    ZERO * o = new ZERO();
    o->z = o;
    return *o;
}

CHAR & cha(char c){
    CHAR * ret = new CHAR(c);
    ret->chr = ret;
    return *(ret);
}

ALT & alt( REGEX & r1, REGEX & r2){
    ALT * a = new ALT(r1,r2);
    return *a;
}

SEQ & seq(const REGEX & r1,const REGEX & r2){
    SEQ * a = new SEQ(r1,r2);
    return *a;
}

STAR & star(const REGEX & r1){
    STAR * a = new STAR(r1);
    a->s = a;
    return *a;
}

PLUS & plus(const REGEX & r1){
    PLUS * p = new PLUS(r1);
    p->pl = p;
    return *p;
}

NTIMES & ntimes(const REGEX & r1, int i){
    NTIMES * n = new NTIMES(r1,i);
    n->nt = n;
    return *n;
}

RANGE & range(set<char> s){
    RANGE * r = new RANGE(s);
    r->ra = r;
    return *r;
}

ID & id(string str,const REGEX & r1){
    ID * id = new ID(r1,str);
    id->i = id;
    return *id;
}

REGEX & der(char c,REGEX & r){
    return r.der(c);
}


REGEX & ders(vector<char> str,REGEX & r){
    for (char c : str) {
        vector<char> rest = vector<char>(str.begin()+1,str.end());
        // REGEX & ret =  der(c,r);
        // cout << c << " " << ret.str() << "\n ";
        REGEX & s = der(c,r).simp();
        //cout << s.str() << "\n";
        return ders(rest,s);
    }
    return r;
}

bool matcher(REGEX & r,string s)
{
    vector<char> v(s.begin(), s.end());
    return ders(v,r).nullable();
}


int main(){
    //ONE o;
    //cout << matcher(cha('c'), "c") << "\n";
    ALT evil2 = alt(star(star(cha('a'))),cha('b'));   
    //set<char> v{'A','B','C'};
    //cout << z.isZero();
    for(int i = 0; i < 6000000;i+=500000) {
       cout << matcher(evil2, string(i, 'a')) << "\n";
    }
    //cout <<  << std::endl;
    //cout << matcher(evil2, "aaaaa") << "\n";
    return 0;
}

