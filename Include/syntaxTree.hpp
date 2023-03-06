#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

class AExp{

};

class Var : public AExp{
    private:
        string s;
    public:
        Var(string sin){
            s = sin;
        }

        string getString(){
            return s;
        }
};

class Num : public AExp{
    private:
        int i;
    public:
        Num(int x){
            i = x;
        }

        int getInt(){
            return i;
        }
};

class Aop : public AExp{
    private:
        string op;
        shared_ptr<AExp> a1;
        shared_ptr<AExp> a2;
    public:
        Aop(string opin, shared_ptr<AExp> a1in, shared_ptr<AExp> a2in){
            op = opin;
            a1 = a1in;
            a2 = a2in;
        }

        string getOp(){
            return op;
        }

        shared_ptr<AExp> getExp1(){
            return a1;
        }

        shared_ptr<AExp> getExp2(){
            return a2;
        }
};