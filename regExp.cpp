#include <iostream>
#include <set>
#include <string>
//CLASS REGEX
using std::cout;
using std::set;
using std::string;

class REGEX{
    public:
        virtual ~REGEX() = default;
        virtual bool nullable() const = 0;
};

class ZERO : public REGEX{
    public:
        bool nullable() const{
            return false;
        }
};

class ONE : public REGEX{
    public:
        bool nullable() const{
            return true;
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
};

class ALT : public REGEX{
    public:
        const REGEX & r1;
        const REGEX & r2;

        ALT(const REGEX & re1,const REGEX & re2):r1(re1),r2(re2){}

        bool nullable() const{
            return r1.nullable() || r2.nullable();
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
};

class STAR : public REGEX{
    public:
        const REGEX & r1;

        STAR(const REGEX & re1):r1(re1){}

        bool nullable() const{
            return true;
        }
};

class PLUS : public REGEX{
    public:
        const REGEX & r1;

        PLUS(const REGEX & re1):r1(re1){}

        bool nullable() const{
            return r1.nullable();
        }
};

class RANGE : public REGEX{
    public:
        set<char> c;

        RANGE(set<char> in){
            c = in;
        }

        bool nullable() const{
            return false;
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

NTIMES ntimes(const REGEX & r1, int i){
    NTIMES n(r1,i);
    return n;
}

RANGE range(const set<char> s){
    RANGE r(s);
    return r;
}

ID id(string str,const REGEX & r1){
    ID i(r1,str);
    return i;
}


int main(){
    ONE x = ONE();
    CHAR y = CHAR('d');
    SEQ xy = SEQ(x,y);
    STAR s = STAR(xy);
    set<char> a;
    a.insert('G');
    a.insert('F');
    a.insert('G');
    RANGE r = RANGE(a);
    NTIMES n = NTIMES(r,0);
    PLUS p = PLUS(s);

    ALT evil2 = alt(star(star(chr('a'))),chr('b'));

    plus(evil2);    

    cout << evil2.nullable() << "\n";
    return 0;
}


    // ZERO, 
    // ONE,
    // CHAR(c: Char), ALT(r1: Rexp, r2: Rexp), SEQ(r1: Rexp, r2: Rexp), STAR(r: Rexp), NTIMES(r: Rexp, n: Int),RANGE(r: Set[Char]),PLUS(r: Rexp), RECD(x: String, r: Rexp)};