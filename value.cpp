#include "Include/regex.hpp"
#include "Include/value.hpp"
#include "Exceptions/LexingError.cpp"

// Val

shared_ptr<Val> Val::inj(shared_ptr<REGEX> r, char c) const
{
    shared_ptr<ID> p1 = dynamic_pointer_cast<ID>(r);
    if (p1 != nullptr)
    {
        shared_ptr<Val> ret(new Rec(p1->getStr(), this->inject(p1->getr(), c)));
        return ret;
    }
    return this->inject(r, c);
}

shared_ptr<Val> Val::inject(shared_ptr<REGEX> r, char c) const
{
    throw LexingError("Value not of right type, injection, line 17 " + this->str());
    return nullptr;
}

vector<pair<string, string>> Val::env() const
{
    return vector<pair<string, string>>{};
}

// Chr
string Chr::str() const
{
    string s(1, c);
    return s;
}

string Chr::flatten() const
{
    string s(1, c);
    return s;
}

// Empty
string Empty::str() const
{
    string ret = "Empty";
    return ret;
}

shared_ptr<Val> Empty::inject(shared_ptr<REGEX> r, char c) const
{
    shared_ptr<Val> e(new Chr(c));
    return e;
}

string Empty::flatten() const
{
    return "";
}

// Left
string Left::str() const
{
    string s = "Left( ";
    s = s + v->str();
    s = s + " )";
    return s;
}

shared_ptr<Val> Left::inject(shared_ptr<REGEX> r, char c) const
{

    shared_ptr<ALT> p1 = dynamic_pointer_cast<ALT>(r);
    if (p1 != nullptr)
    {
        shared_ptr<Val> ret(new Left(v->inj(p1->getr1(), c)));
        return ret;
    }

    shared_ptr<SEQ> p2 = dynamic_pointer_cast<SEQ>(r);
    if (p2 != nullptr)
    {
        shared_ptr<Sequ> p3 = dynamic_pointer_cast<Sequ>(v);
        if (p3 != nullptr)
        {
            shared_ptr<Val> ret(new Sequ(p3->getr1()->inj(p2->getr1(), c), p3->getr2()));
            return ret;
        }
    }
    throw LexingError("Left Injection line 80 " + r->str() + " " + typeid(*r).name() + " " + this->str() + " " + c);
    return nullptr;
}

vector<pair<string, string>> Left::env() const
{
    return v->env();
}

string Left::flatten() const
{
    return v->flatten();
}

std::shared_ptr<Val> Left::getV()
{
    return v;
}

// Right
string Right::str() const
{
    string s = "Right( ";
    s = s + v->str();
    s = s + " )";
    return s;
}

shared_ptr<Val> Right::inject(shared_ptr<REGEX> r, char c) const
{
    shared_ptr<SEQ> p1 = dynamic_pointer_cast<SEQ>(r);
    if (p1 != nullptr)
    {
        shared_ptr<Val> ret(new Sequ(p1->getr1()->mkeps(), v->inj(p1->getr2(), c)));
        return ret;
    }

    shared_ptr<ALT> p2 = dynamic_pointer_cast<ALT>(r);
    if (p2 != nullptr)
    {
        shared_ptr<Val> ret(new Right(v->inj(p2->getr2(), c)));
        return ret;
    }
    throw LexingError("Right injection line 114 " + r->str());
    return nullptr;
}

vector<pair<string, string>> Right::env() const
{
    return v->env();
}

string Right::flatten() const
{
    return v->flatten();
}

std::shared_ptr<Val> Right::getV()
{
    return v;
}

// Sequ
string Sequ::str() const
{
    string s = "Sequ(";
    s = s + v1->str();
    s = s + " , ";
    s = s + v2->str();
    s = s + ")";
    return s;
}

shared_ptr<Val> Sequ::inject(shared_ptr<REGEX> r, char c) const
{
    shared_ptr<SEQ> p1 = dynamic_pointer_cast<SEQ>(r);
    if (p1 != nullptr)
    {
        shared_ptr<Val> ret(new Sequ(v1->inj(p1->getr1(), c), v2));
        return ret;
    }
    shared_ptr<STAR> p2 = dynamic_pointer_cast<STAR>(r);
    if (p2 != nullptr)
    {
        shared_ptr<Stars> p3 = dynamic_pointer_cast<Stars>(v2);
        (p3) ?: throw LexingError("Sequ injection, not of type Stars (STAR) line 147 " + v2->str());
        vector<shared_ptr<Val>> newList = p3->getList();
        newList.insert(newList.begin(), v1->inj(p2->getr(), c));
        shared_ptr<Val> ret(new Stars(newList));
        return ret;
    }
    shared_ptr<PLUS> p4 = dynamic_pointer_cast<PLUS>(r);
    if (p4 != nullptr)
    {
        shared_ptr<Stars> p5 = dynamic_pointer_cast<Stars>(v2);
        (p5) ?: throw LexingError("Sequ injection, not of type Stars (PLUS) line 156 " + v2->str());
        vector<shared_ptr<Val>> newList = p5->getList();
        newList.insert(newList.begin(), v1->inj(p4->getr(), c));
        shared_ptr<Val> ret(new Plus(newList));
        return ret;
    }
    shared_ptr<NTIMES> p6 = dynamic_pointer_cast<NTIMES>(r);
    if (p6 != nullptr)
    {
        shared_ptr<Ntimes> p7 = dynamic_pointer_cast<Ntimes>(v2);
        (p7) ?: throw LexingError("Sequ injection, not of type Ntimes line 165 " + v2->str());
        vector<shared_ptr<Val>> newList = p7->getList();
        newList.insert(newList.begin(), v1->inj(p6->getr(), c));
        shared_ptr<Val> ret(new Ntimes(newList));
        return ret;
    }
    throw LexingError("Sequ injection, not right type " + r->str());
    return nullptr;
}

shared_ptr<Val> Sequ::getr1() const
{
    return v1;
}
shared_ptr<Val> Sequ::getr2() const
{
    return v2;
}

vector<pair<string, string>> Sequ::env() const
{
    vector<pair<string, string>> e1 = v1->env();
    vector<pair<string, string>> e2 = v2->env();
    vector<pair<string, string>> ret;
    ret.reserve(e1.size() + e1.size());
    ret.insert(ret.end(), e1.begin(), e2.end());
    ret.insert(ret.end(), e2.begin(), e2.end());
    return ret;
}

string Sequ::flatten() const
{
    return v1->flatten() + v2->flatten();
}

// Stars
string Stars::str() const
{
    if (v.size() == 0)
    {
        return "Stars(Nil)";
    }
    string s = "Stars( ";
    for (int i = 0; i < v.size(); ++i)
    {
        s = s + v[i]->str();
        s = s + " ";
    }
    s = s + " )";
    return s;
}

vector<shared_ptr<Val>> Stars::getList()
{
    return v;
}

vector<pair<string, string>> Stars::env() const
{
    vector<pair<string, string>> ret;
    for (int i = 0; i < v.size(); ++i)
    {
        vector<pair<string, string>> add = v[i]->env();
        ret.insert(ret.end(), add.begin(), add.end());
    }
    return ret;
}

string Stars::flatten() const
{
    string ret = "";
    for (int i = 0; i < v.size(); ++i)
    {
        ret = ret + v[i]->flatten();
    }
    return ret;
}

// Rec
string Rec::str() const
{
    string ret = "Rec( ";
    ret = ret + s + " , " + v->str();
    ret = ret + " )";
    return ret;
}

vector<pair<string, string>> Rec::env() const
{
    vector<pair<string, string>> ret{pair<string, string>{s, v->flatten()}};
    return ret;
}

string Rec::flatten() const
{
    return v->flatten();
}

// Plus
string Plus::str() const
{
    if (v.size() == 0)
    {
        return "Plus(Nil)";
    }
    string s = "Plus( ";
    for (int i = 0; i < v.size(); ++i)
    {
        s = s + v[i]->str();
        s = s + " ";
    }
    s = s + " )";
    return s;
}

vector<pair<string, string>> Plus::env() const
{
    vector<pair<string, string>> ret;
    for (int i = 0; i < v.size(); ++i)
    {
        vector<pair<string, string>> add = v[i]->env();
        ret.insert(ret.end(), add.begin(), add.end());
    }
    return ret;
}

string Plus::flatten() const
{
    string ret = "";
    for (int i = 0; i < v.size(); ++i)
    {
        ret = ret + v[i]->flatten();
    }
    return ret;
}

// Ntimes
string Ntimes::str() const
{
    if (v.size() == 0)
    {
        return "Ntimes(Nil)";
    }
    string s = "Ntimes( ";
    for (int i = 0; i < v.size(); ++i)
    {
        s = s + v[i]->str();
        s = s + " ";
    }
    s = s + " )";
    return s;
}

vector<shared_ptr<Val>> Ntimes::getList()
{
    return v;
}

vector<pair<string, string>> Ntimes::env() const
{
    vector<pair<string, string>> ret;
    for (int i = 0; i < v.size(); ++i)
    {
        vector<pair<string, string>> add = v[i]->env();
        ret.insert(ret.end(), add.begin(), add.end());
    }
    return ret;
}

string Ntimes::flatten() const
{
    string ret = "";
    for (int i = 0; i < v.size(); ++i)
    {
        ret = ret + v[i]->flatten();
    }
    return ret;
}
