#pragma once

#include <string>
#include <memory>
#include <map>
#include "../Exceptions/RunTimeError.cpp"

using std::map;
using std::shared_ptr;
using std::string;

class AExp
{
public:
    virtual string getString() = 0;
    virtual int eval_aexp(map<string, int> env) = 0;
};

class BExp
{
public:
    virtual string getString() = 0;
    virtual bool eval_bexp(map<string, int> env) = 0;
};

class Stmt
{
public:
    virtual string getString() = 0;
    virtual map<string, int> eval_stmt(map<string, int>) = 0;
};

class Decl
{
public:
    virtual string getString() = 0;
    virtual map<string, int> eval_dec(map<string, int>) = 0;
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

    int eval_aexp(map<string, int> env) override
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

    int eval_aexp(map<string, int> env) override
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

    int eval_aexp(map<string, int> env) override
    {
        int num1 = a1->eval_aexp(env);
        int num2 = a2->eval_aexp(env);

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

    map<string, int> eval_stmt(map<string, int> env) override
    {
        env[s] = a->eval_aexp(env);
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

    map<string, int> eval_stmt(map<string, int> env) override
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

public:
    If(shared_ptr<BExp> boolean, vector<shared_ptr<Stmt>> b1, vector<shared_ptr<Stmt>> b2) : bexp(boolean), ifBlock(b1), elseBlock(b2) {}

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

    map<string, int> eval_block(vector<shared_ptr<Stmt>> input, map<string, int> env)
    {
        for (int i = 0; i < input.size(); ++i)
        {
            env = input[i]->eval_stmt(env);
        }
        return env;
    }

    map<string, int> eval_stmt(map<string, int> env) override
    {
        if (bexp->eval_bexp(env))
        {
            return eval_block(ifBlock, env);
        }
        return eval_block(elseBlock, env);
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

    map<string, int> eval_block(vector<shared_ptr<Stmt>> input, map<string, int> env)
    {
        for (int i = 0; i < input.size(); ++i)
        {
            env = input[i]->eval_stmt(env);
        }
        return env;
    }

    map<string, int> eval_stmt(map<string, int> env) override
    {
        while (bexp->eval_bexp(env))
        {
            env = eval_block(block, env);
        }
        return env;
    }
};

class Call : public Stmt
{
private:
    string name;
    vector<string> args;
public:
    Call(string func,vector<string> inArgs):name(func),args(inArgs){}


    string getString() override
    {
        string arg = "";
        for(int i = 0; i < args.size();++i){
            arg += args[i] + " , ";
        }
        return "Call: " + name + " ( " + arg + " ) ";
    }

    map<string, int> eval_stmt(map<string, int> env) override
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

    bool eval_bexp(map<string, int> env) override
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

    bool eval_bexp(map<string, int> env) override
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

    bool eval_bexp(map<string, int> env) override
    {
        int i = a1->eval_aexp(env);
        int j = a2->eval_aexp(env);
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

    bool eval_bexp(map<string, int> env) override
    {
        return a1->eval_bexp(env) && a2->eval_bexp(env);
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

    bool eval_bexp(map<string, int> env) override
    {
        return a1->eval_bexp(env) || a2->eval_bexp(env);
    }
};



class Def : public Decl
{
private:
    string name;
    vector<string> args;
    vector<shared_ptr<Stmt>> body;

public:
    Def(string funcName, vector<string> arguments, vector<shared_ptr<Stmt>> execute) : name(funcName), args(arguments), body(execute) {}

    string getString()
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

        return "Def" + name + "() ( " + argPrint + " ) " + " { " + bodyPrint + "}";
    }

    // map<string,vector<shared_ptr<Stmt>>> getFunName()
    // {
    //     map<string,vector<shared_ptr<Stmt>>> ret;
    //     ret.insert(name,body);
    //     return name;
    // }

    map<string, int> eval_block(vector<shared_ptr<Stmt>> input, map<string, int> env)
    {
        for (int i = 0; i < input.size(); ++i)
        {
            env = input[i]->eval_stmt(env);
        }
        return env;
    }
    
    map<string, int> runFunc(map<string, int> env)
    {
        return eval_block(body,env);
    }

    map<string, int> eval_dec(map<string, int> env) 
    {
        return env;
    }
};


class Main : public Decl
{
private:
    vector<shared_ptr<Stmt>> body;

public:
    Main(vector<shared_ptr<Stmt>> execute): body(execute) {}

    string getString()
    {
        string bodyPrint = "";
        for (int i = 0; i < body.size(); ++i)
        {
            bodyPrint += body[i]->getString() + ";";
        }

        return "Main()  { " + bodyPrint + "}";
    }

    map<string, int> eval_dec(map<string, int> env) 
    {
        return env;
    }
};
