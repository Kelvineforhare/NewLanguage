#include <iostream>
//CLASS REGEX
using std::cout;

class REGEX{
    public:
        virtual ~REGEX() = default;
        virtual bool nullable(char a = '\0') = 0;
};

class ZERO : public REGEX{
    public:
        bool nullable(){
            return false;
        }
};

class ONE : public REGEX{
    public:
        bool nullable(char in = 'a'){
            return true;
        }
};

class CHAR : public REGEX{
    public:
        char c;

        CHAR(char in){
            c = in;
        }

        bool nullable(char d){
            return false;
        }
};

class ALT : public REGEX{
    public:
        REGEX & r1;
        REGEX & r2;

        ALT(REGEX & re1, REGEX & re2):r1(re1),r2(re2){}

        bool nullable(char in = 'a'){
            return r1.nullable() || r2.nullable();
        }
};

class SEQ : public REGEX{
    public:
        REGEX & r1;
        REGEX & r2;

        SEQ(REGEX & re1, REGEX & re2):r1(re1),r2(re2){}

        bool nullable(char in = 'a'){
            return r1.nullable() && r2.nullable();
        }
};

class STAR : public REGEX{
    public:
        REGEX & r1;

        STAR(REGEX & re1):r1(re1){}

        bool nullable(char in = 'a'){
            return true;
        }
};


int main(){
    ONE x = ONE();
    CHAR y = CHAR('d');
    SEQ xy = SEQ(x,y);
    
    cout << xy.nullable();
    return 0;
}


    // ZERO, 
    // ONE,
    // CHAR(c: Char), ALT(r1: Rexp, r2: Rexp), SEQ(r1: Rexp, r2: Rexp), STAR(r: Rexp), NTIMES(r: Rexp, n: Int),RANGE(r: Set[Char]),PLUS(r: Rexp), RECD(x: String, r: Rexp)};