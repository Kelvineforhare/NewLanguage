#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

class AExp{
    public:
        virtual string getString() = 0;
};

class Var : public AExp{
    private:
        string s;
    public:
        Var(string sin){
            s = sin;
        }

        string getString() override{
            return s;
        }
};

class Int : public AExp{
    private:
        int i;
    public:
        Int(int x){
            i = x;
        }

        int getInt(){
            return i;
        }

        string getString() override{
            return std::to_string(i);
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

        string getOp() {
            return op;
        }

        shared_ptr<AExp> getExp1(){
            return a1;
        }

        shared_ptr<AExp> getExp2(){
            return a2;
        }

        string getString() override{
            return "Aop( " + op + " " + a1->getString() + " " + a2->getString() + " )";
        }
};