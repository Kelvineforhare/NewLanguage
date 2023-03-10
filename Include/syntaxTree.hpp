#include <string>
#include <memory>
#include <map>
#include "../Exceptions/RunTimeError.cpp"

using std::string;
using std::shared_ptr;
using std::map;

class AExp{
    public:
        virtual string getString() = 0;
        virtual int eval_aexp(map<string,int> env) = 0;

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

        int eval_aexp(map<string,int> env) override{
            auto it = env.find(s);
            if(it == env.end()){
                throw RunTimeError("Identifier" + s + "does is not defined");
            }
            return it->second;
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

        int eval_aexp(map<string,int> env)override{
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

        int eval_aexp(map<string,int> env)override{
            int num1 = a1->eval_aexp(env);
            int num2 = a2->eval_aexp(env);

            if(op == "+"){
                return num1 + num2;
            }
            if(op == "-"){
                return num1 - num2;
            }
            if(op == "*"){
                return num1 * num2;
            }
            if(op == "/"){
                return num1 / num2;
            }
            throw RunTimeError("operator: " + op + " does not exist");
            return 0;
        }
};




class Stmt{
    public:
        virtual string getString() = 0;
        virtual map<string,int> eval_stmt(map<string,int>) = 0;
};

class Assign: public Stmt{
    private:
        string s;
        shared_ptr<AExp> a;
    public:
        Assign(string sin, shared_ptr<AExp> ain){
            s = sin;
            a = ain;
        }

        map<string,int> eval_stmt(map<string,int> env)override{
            env[s] = a->eval_aexp(env);
            return env;
        }

        string getString()override{
            string ret = "Assign( " + s + " , " + a->getString() + " )";
            return ret;
        }
};

class WriteVar: public Stmt{
    private:
        string var;

    public:
        WriteVar(string str){
            var = str;
        }


        map<string,int> eval_stmt(map<string,int> env)override{
            auto it = env.find(var);
            if(it == env.end()){
                throw RunTimeError("Identifier" + var + "does is not defined");
            }
            cout << it->second << "\n";
            return env;
        }


        string getString()override{
                string ret = "WriteVar( " + var + " )";
                return ret;
        }
};



class BExp{
    public:
        virtual string getString() = 0;
};

class True: public BExp{
    public:
        string getString(){
            return "True";
        }
};

class False: public BExp{
    public:
        string getString(){
            return "False";
        }
};

class Bop: public BExp{
    private:
        string op;
        shared_ptr<AExp> a1;
        shared_ptr<AExp> a2;
    public:
        Bop(string oper,shared_ptr<AExp> exp1,shared_ptr<AExp> exp2){
            op = oper;
            a1 = exp1;
            a2 = exp2;
        }
        string getString(){
            return "False";
        }
};