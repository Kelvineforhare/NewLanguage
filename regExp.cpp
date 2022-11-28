#include <iostream>
#include <set>
#include <string>
#include <vector>
//CLASS REGEX
using std::cout;
using std::set;
using std::string;
using std::vector;


//fix function protyping
// CHAR chr(char);
// ALT alt(const REGEX &,const REGEX &);
// SEQ seq(const REGEX & ,const REGEX &);
// STAR star(const REGEX & r1);
// PLUS plus(const REGEX & r1);
// NTIMES ntimes(const REGEX &, int);
// RANGE range(const set<char>);


// ID id(string str,const REGEX & r1){
//     ID i(r1,str);
//     return i;
// }


class REGEX{
    public:
        virtual ~REGEX() = default;
        virtual bool nullable() const = 0;
        virtual REGEX & der(char c) const = 0;
};

class ZERO : public REGEX{
    public:
        bool nullable() const{
            return false;
        }

        REGEX & der(char c) const{
            ZERO z;
            return z;
        }
};

class ONE : public REGEX{
    public:
        bool nullable() const{
            return true;
        }

        REGEX & der(char c) const{
            ONE o;
            return o;
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
                ONE o;
                return o;
            }
            ZERO z;
            return z;
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
            ALT ret = alt(r1.der(c),r2.der(c));
            return ret;
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
                alt(seq(r1.der(c),r2),r2.der(c));
            }
            SEQ ret = seq(r1.der(c),r2);
            return ret;
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
            STAR ret = seq(r1.der(c),star(r1));
            return ret;
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
            STAR ret = seq(r1.der(c),star(r1));
            return ret;
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
                ONE o;
                return o;
            }
            ZERO z;
            return z;
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
};

class ID : public REGEX{
    public:
        const REGEX & r1;
        string s;

        ID(const REGEX & re1,string str):r1(re1),s(str){}

        bool nullable() const{
            return r1.nullable();
        }
};

CHAR chr(char a){
    CHAR c(a);
    return c;
}

ALT alt(const REGEX & r1,const REGEX & r2){
    ALT a(r1,r2);
    return a;
}

SEQ seq(const REGEX & r1,const REGEX & r2){
    SEQ a(r1,r2);
    return a;
}

STAR star(const REGEX & r1){
    STAR a(r1);
    return a;
}

PLUS plus(const REGEX & r1){
    PLUS p(r1);
    return p;
}

// NTIMES ntimes(const REGEX & r1, int i){
//     NTIMES n(r1,i);
//     return n;
// }

RANGE range(const set<char> s){
    RANGE r(s);
    return r;
}

// ID id(string str,const REGEX & r1){
//     ID i(r1,str);
//     return i;
// }




// REGEX & ders(vector<char> str,REGEX & r){
//     for (char c : str) {
//         vector<char> rest = vector<char>(str.begin()+1,str.end());
//         ders(rest, der(c,r));
//     }
    
// }


int main(){
    ONE o;
    ALT a(o,o);

    //ALT evil2 = alt(star(star(chr('a'))),chr('b'));   

    cout << a.nullable() << "\n";
    return 0;
}

