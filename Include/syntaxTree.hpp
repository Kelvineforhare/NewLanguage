#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "../Exceptions/RunTimeError.cpp"

using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

class Def;

class AExp
{
public:
    virtual string getString() = 0;
    virtual int eval_aexp(map<string, int> env, map<string, Def> fun) = 0;
};

class BExp
{
public:
    virtual string getString() = 0;
    virtual bool eval_bexp(map<string, int> env, map<string, Def> fun) = 0;
};

class Stmt
{
public:
    virtual string getString() = 0;
    virtual map<string, int> eval_stmt(map<string, int> env, map<string, Def> fun) = 0;
    virtual bool hasReturn()
    {
        return false;
    }
    virtual shared_ptr<AExp> getReturn()
    {
        return nullptr;
    }
};

class Decl
{
public:
    virtual string getString() = 0;
    virtual map<string, int> eval_dec(map<string, int> env, map<string, Def> fun) = 0;
};

class Def : public Decl
{
private:
    string name;
    vector<string> args;
    vector<shared_ptr<Stmt> > body;
    shared_ptr<AExp> ret;

public:
    Def(string funcName, vector<string> arguments, vector<shared_ptr<Stmt> > execute, shared_ptr<AExp> retIn) : name(funcName), args(arguments), body(execute), ret(retIn) {}

    string getString() override
    {
        string argPrint = "";
        for (int i = 0; i < args.size(); ++i)
        {
            argPrint += args[i] + " , ";
        }
        string bodyPrint = "";
        for (int i = 0; i < body.size(); ++i)
        {
            bodyPrint += body[i]->getString() + ";";
        }

        return "Def " + name + "() ( " + argPrint + " ) " + " { " + bodyPrint + "}";
    }

    string getFunName()
    {
        return name;
    }

    int eval_block(vector<shared_ptr<Stmt>> input, map<string, int> env, map<string, Def> fun)
    {
        for (int i = 0; i < input.size(); ++i)
        {
            env = input[i]->eval_stmt(env, fun);
            if (input[i]->hasReturn())
            {
                return input[i]->getReturn()->eval_aexp(env, fun);
            }
        }
        return ret->eval_aexp(env, fun);
    }

    int runFunc(map<string, int> env, vector<shared_ptr<AExp>> input, map<string, Def> fun)
    {
        if (input.size() != args.size())
        {
            throw RunTimeError("Incorrect number of arguments");
        }

        map<string, int> functionEnv = env;

        for (int i = 0; i < args.size(); ++i)
        {
            functionEnv[args[i]] = input[i]->eval_aexp(env, fun);
        }
        return eval_block(body, functionEnv, fun);
    }

    map<string, int> eval_dec(map<string, int> env, map<string, Def> fun) override
    {
        return env;
    }
};

class Call : public AExp
{
private:
    string name;
    vector<shared_ptr<AExp>> args;

public:
    Call(string func, vector<shared_ptr<AExp>> inArgs) : name(func), args(inArgs) {}

    string getString() override
    {
        string arg = "";
        for (int i = 0; i < args.size(); ++i)
        {
            arg += args[i]->getString() + " , ";
        }
        return "Call: " + name + " ( " + arg + " ) ";
    }

    int eval_aexp(map<string, int> env, map<string, Def> fun) override
    {
        auto it = fun.find(name);
        if (it != fun.end())
        {
            return it->second.runFunc(env, args, fun);
        }
        throw RunTimeError("Function" + name + "Doesn't Exist");
    }
};

class Var : public AExp
{
private:
    string s;

public:
    Var(string sin)
    {
        s = sin;
    }

    string getString() override
    {
        return s;
    }

    int eval_aexp(map<string, int> env, map<string, Def> fun) override
    {
        auto it = env.find(s);
        if (it == env.end())
        {
            throw RunTimeError("Identifier " + s + " does is not defined");
        }
        return it->second;
    }
};

class Int : public AExp
{
private:
    int i;

public:
    Int(int x)
    {
        i = x;
    }

    int getInt()
    {
        return i;
    }

    string getString() override
    {
        return std::to_string(i);
    }

    int eval_aexp(map<string, int> env, map<string, Def> fun) override
    {
        return i;
    }
};

class Aop : public AExp
{
private:
    string op;
    shared_ptr<AExp> a1;
    shared_ptr<AExp> a2;

public:
    Aop(string opin, shared_ptr<AExp> a1in, shared_ptr<AExp> a2in)
    {
        op = opin;
        a1 = a1in;
        a2 = a2in;
    }

    string getOp()
    {
        return op;
    }

    shared_ptr<AExp> getExp1()
    {
        return a1;
    }

    shared_ptr<AExp> getExp2()
    {
        return a2;
    }

    string getString() override
    {
        return "Aop( " + op + " " + a1->getString() + " " + a2->getString() + " )";
    }

    int eval_aexp(map<string, int> env, map<string, Def> fun) override
    {
        int num1 = a1->eval_aexp(env, fun);
        int num2 = a2->eval_aexp(env, fun);

        if (op == "+")
        {
            return num1 + num2;
        }
        if (op == "-")
        {
            return num1 - num2;
        }
        if (op == "*")
        {
            return num1 * num2;
        }
        if (op == "/")
        {
            return num1 / num2;
        }
        if (op == "%")
        {
            return num1 % num2;
        }
        throw RunTimeError("operator: " + op + " does not exist");
        return 0;
    }
};

class Assign : public Stmt
{
private:
    string s;
    shared_ptr<AExp> a;

public:
    Assign(string sin, shared_ptr<AExp> ain)
    {
        s = sin;
        a = ain;
    }

    map<string, int> eval_stmt(map<string, int> env, map<string, Def> fun) override
    {
        env[s] = a->eval_aexp(env, fun);
        return env;
    }

    string getString() override
    {
        string ret = "Assign( " + s + " , " + a->getString() + " )";
        return ret;
    }
};

class WriteVar : public Stmt
{
private:
    string var;

public:
    WriteVar(string str)
    {
        var = str;
    }

    map<string, int> eval_stmt(map<string, int> env, map<string, Def> fun) override
    {
        auto it = env.find(var);
        if (it == env.end())
        {
            throw RunTimeError("Identifier " + var + " does is not defined");
        }
        cout << std::to_string(it->second) << "\n";
        return env;
    }

    string getString() override
    {
        string ret = "print( " + var + " )";
        return ret;
    }
};

class If : public Stmt
{
private:
    shared_ptr<BExp> bexp;
    vector<shared_ptr<Stmt>> ifBlock;
    vector<shared_ptr<Stmt>> elseBlock;
    shared_ptr<AExp> ifRet;
    bool hasIfReturn = false;

public:
    If(shared_ptr<BExp> boolean, vector<shared_ptr<Stmt>> b1, vector<shared_ptr<Stmt>> b2) : bexp(boolean), ifBlock(b1), elseBlock(b2) {}
    If(shared_ptr<BExp> boolean, vector<shared_ptr<Stmt>> b1, vector<shared_ptr<Stmt>> b2, shared_ptr<AExp> retIn) : bexp(boolean), ifBlock(b1), elseBlock(b2), ifRet(retIn) {}

    string getString() override
    {
        string ret = "If( " + bexp->getString() + " | ";
        for (int i = 0; i < ifBlock.size(); ++i)
        {
            ret += ifBlock[i]->getString() + " ";
        }
        ret += " | ";
        for (int i = 0; i < elseBlock.size(); ++i)
        {
            ret += elseBlock[i]->getString() + " ";
        }
        ret += " )";
        return ret;
    }

    bool hasReturn() override
    {
        return hasIfReturn;
    }

    shared_ptr<AExp> getReturn() override
    {
        return ifRet;
    }

    map<string, int> eval_block(vector<shared_ptr<Stmt>> input, map<string, int> env, map<string, Def> fun)
    {
        for (int i = 0; i < input.size(); ++i)
        {
            env = input[i]->eval_stmt(env, fun);
        }
        return env;
    }

    map<string, int> eval_stmt(map<string, int> env, map<string, Def> fun) override
    {
        if (bexp->eval_bexp(env, fun))
        {
            if (ifRet != nullptr)
            {
                hasIfReturn = true;
            }
            return eval_block(ifBlock, env, fun);
        }
        return eval_block(elseBlock, env, fun);
    }
};

class While : public Stmt
{
private:
    shared_ptr<BExp> bexp;
    vector<shared_ptr<Stmt>> block;

public:
    While(shared_ptr<BExp> b, vector<shared_ptr<Stmt>> stmt) : bexp(b), block(stmt) {}

    string getString() override
    {
        string ret = "While( " + bexp->getString();
        for (int i = 0; i < block.size(); ++i)
        {
            ret += block[i]->getString() + " ";
        }
        ret += " )";
        return ret;
    }

    map<string, int> eval_block(vector<shared_ptr<Stmt>> input, map<string, int> env, map<string, Def> fun)
    {
        for (int i = 0; i < input.size(); ++i)
        {
            env = input[i]->eval_stmt(env, fun);
        }
        return env;
    }

    map<string, int> eval_stmt(map<string, int> env, map<string, Def> fun) override
    {
        while (bexp->eval_bexp(env, fun))
        {
            env = eval_block(block, env, fun);
        }
        return env;
    }
};

class Pass : public Stmt
{
public:
    Pass() {}

    string getString() override
    {
        return "pass";
    }

    map<string, int> eval_stmt(map<string, int> env, map<string, Def> fun) override
    {
        return env;
    }
};

class True : public BExp
{
public:
    string getString() override
    {
        return "True";
    }

    bool eval_bexp(map<string, int> env, map<string, Def> fun) override
    {
        return true;
    }
};

class False : public BExp
{
public:
    string getString() override
    {
        return "False";
    }

    bool eval_bexp(map<string, int> env, map<string, Def> fun) override
    {
        return false;
    }
};

class Bop : public BExp
{
private:
    string op;
    shared_ptr<AExp> a1;
    shared_ptr<AExp> a2;

public:
    Bop(string oper, shared_ptr<AExp> exp1, shared_ptr<AExp> exp2)
    {
        op = oper;
        a1 = exp1;
        a2 = exp2;
    }
    string getString() override
    {
        return "Bop( " + op + " " + a1->getString() + " " + a2->getString() + ")";
    }

    bool eval_bexp(map<string, int> env, map<string, Def> fun) override
    {
        int i = a1->eval_aexp(env, fun);
        int j = a2->eval_aexp(env, fun);
        if (op == "==")
        {
            return i == j;
        }
        if (op == "!=")
        {
            return i != j;
        }
        if (op == "<")
        {
            return i < j;
        }
        if (op == ">")
        {
            return i > j;
        }
        if (op == "<=")
        {
            return i <= j;
        }
        if (op == ">=")
        {
            return i >= j;
        }
        throw RunTimeError("operator: " + op + " does not exist");
        return false;
    }
};

class And : public BExp
{
private:
    shared_ptr<BExp> a1;
    shared_ptr<BExp> a2;

public:
    And(shared_ptr<BExp> a, shared_ptr<BExp> b)
    {
        a1 = a;
        a2 = b;
    }

    string getString() override
    {
        return "And( " + a1->getString() + " " + a2->getString() + ")";
    }

    bool eval_bexp(map<string, int> env, map<string, Def> fun) override
    {
        return a1->eval_bexp(env, fun) && a2->eval_bexp(env, fun);
    }
};

class Or : public BExp
{
private:
    shared_ptr<BExp> a1;
    shared_ptr<BExp> a2;

public:
    Or(shared_ptr<BExp> a, shared_ptr<BExp> b)
    {
        a1 = a;
        a2 = b;
    }

    string getString() override
    {
        return "Or( " + a1->getString() + " " + a2->getString() + ")";
    }

    bool eval_bexp(map<string, int> env, map<string, Def> fun) override
    {
        return a1->eval_bexp(env, fun) || a2->eval_bexp(env, fun);
    }
};

class Main : public Decl
{
private:
    vector<shared_ptr<Stmt>> body;

public:
    Main(vector<shared_ptr<Stmt>> execute)
    {
        body = execute;
    }

    Main() {}

    Main operator=(Main &m2)
    {
        body = m2.body;
        return *this;
    }

    string getString() override
    {
        string bodyPrint = "";
        for (int i = 0; i < body.size(); ++i)
        {
            bodyPrint += body[i]->getString() + ";";
        }

        return "Main()  { " + bodyPrint + "}";
    }

    map<string, int> eval_dec(map<string, int> env, map<string, Def> fun) override
    {
        for (int i = 0; i < body.size(); ++i)
        {
            env = body[i]->eval_stmt(env, fun);
            if(body[i]->hasReturn()){
                throw RunTimeError("Return present outside of a function");
            }
        }
        return env;
    }
};
