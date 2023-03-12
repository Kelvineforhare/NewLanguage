#include "Include/value.hpp"
#include "Include/regex.hpp"
#include "Exceptions/LexingError.cpp"

using std::cout;
using std::move;
using std::set;
using std::shared_ptr;
using std::string;
using std::unique_ptr;
using std::vector;

std::shared_ptr<ONE> one()
{
    std::shared_ptr<ONE> one(new ONE());
    return one;
}

std::shared_ptr<ZERO> zero()
{
    std::shared_ptr<ZERO> zero(new ZERO());
    return zero;
}

std::shared_ptr<CHAR> cha(char c)
{
    std::shared_ptr<CHAR> p(new CHAR(c));
    return p;
}

std::shared_ptr<ALT> alt(std::shared_ptr<REGEX> r1, std::shared_ptr<REGEX> r2)
{
    std::shared_ptr<ALT> a(new ALT((r1), (r2)));
    return a;
}

std::shared_ptr<SEQ> seq(std::shared_ptr<REGEX> r1, std::shared_ptr<REGEX> r2)
{
    std::shared_ptr<SEQ> seq(new SEQ((r1), (r2)));
    return seq;
}

std::shared_ptr<STAR> star(std::shared_ptr<REGEX> r1)
{
    std::shared_ptr<STAR> s(new STAR((r1)));
    return s;
}

std::shared_ptr<PLUS> plus(std::shared_ptr<REGEX> r1)
{
    std::shared_ptr<PLUS> p(new PLUS(r1));
    return p;
}

std::shared_ptr<NTIMES> ntimes(std::shared_ptr<REGEX> r1, int i)
{
    std::shared_ptr<NTIMES> n(new NTIMES(r1, i));
    return n;
}

std::shared_ptr<RANGE> range(set<char> s)
{
    std::shared_ptr<RANGE> r(new RANGE(s));
    return r;
}

std::shared_ptr<ID> id(string str, std::shared_ptr<REGEX> r1)
{
    std::shared_ptr<ID> id(new ID(r1, str));
    return id;
}

shared_ptr<REGEX> der(char c, shared_ptr<REGEX> r)
{
    return r->der(c);
}

// shared_ptr<REGEX> ders(const vector<char> & str, shared_ptr<REGEX> r){
//     int i = 0;
//     if(i >= str.size()){
//         return r;
//     }
//     shared_ptr<REGEX> s = der(str[i],r)->simp();
//     ++i;
//     for(;i < str.size();++i){
//         s = der(str[i],s)->simp();
//     }
//     return s;
// }

// bool matcher(shared_ptr<REGEX> r,const string & s)
// {
//     vector<char> v(s.begin(), s.end());
//     return ders(v,r)->nullable();
// }

shared_ptr<REGEX> string2rexp(const string &s)
{
    if (s.size() == 0)
    {
        return one();
    }
    if (s.size() == 1)
    {
        return cha(s[0]);
    }
    shared_ptr<REGEX> ret = seq(cha(s[0]), cha(s[1]));
    for (int i = 2; i < s.size(); ++i)
    {
        ret = seq(ret, cha(s[i]));
    }
    return ret;
}

shared_ptr<REGEX> stringList2rexp(const vector<string> &s)
{
    if (s.size() == 0)
    {
        return one();
    }
    if (s.size() == 1)
    {
        return string2rexp(s[0]);
    }
    shared_ptr<REGEX> ret = alt(string2rexp(s[0]), string2rexp(s[1]));
    for (int i = 2; i < s.size(); ++i)
    {
        ret = alt(ret, string2rexp(s[i]));
    }
    return ret;
}

shared_ptr<Val> F_ID(shared_ptr<Val> v)
{
    return v;
}

Function F_RIGHT(Function f)
{

    auto ret = [f](shared_ptr<Val> v) -> shared_ptr<Val>
    {
        return shared_ptr<Val>(new Right(f(v)));
    };
    return ret;
}

Function F_LEFT(Function f)
{
    auto ret = [f](shared_ptr<Val> v) -> shared_ptr<Val>
    {
        return shared_ptr<Val>(new Left(f(v)));
    };
    return ret;
}

Function F_ALT(Function f1, Function f2)
{
    auto ret = [f1, f2](shared_ptr<Val> v) -> shared_ptr<Val>
    {
        shared_ptr<Right> p1 = dynamic_pointer_cast<Right>(v);
        if (p1 != nullptr)
        {
            return shared_ptr<Val>(new Right(f2(p1->getV())));
        }
        shared_ptr<Left> p2 = dynamic_pointer_cast<Left>(v);
        if (p2 != nullptr)
        {
            shared_ptr<Val> ans(new Left(f1(p2->getV())));
            return ans;
        }
        return v;
    };
    return ret;
}

shared_ptr<Val> F_ERROR(shared_ptr<Val> v = shared_ptr<Val>(new Empty()))
{
    throw LexingError("Line 143 regular expression doesnt match");
}

Function F_SEQ_Empty1(Function f1, Function f2)
{
    auto ret = [f1, f2](shared_ptr<Val> v) -> shared_ptr<Val>
    {
        shared_ptr<Val> empty(new Empty());
        return shared_ptr<Val>(new Sequ(f1(empty), f2(v)));
    };
    return ret;
}

Function F_SEQ_Empty2(Function f1, Function f2)
{
    auto ret = [f1, f2](shared_ptr<Val> v) -> shared_ptr<Val>
    {
        shared_ptr<Val> empty(new Empty());
        return shared_ptr<Val>(new Sequ(f1(v), f2(empty)));
    };
    return ret;
}

Function F_SEQ(Function f1, Function f2)
{
    auto ret = [f1, f2](shared_ptr<Val> v) -> shared_ptr<Val>
    {
        shared_ptr<Sequ> p = dynamic_pointer_cast<Sequ>(v);
        if (p != nullptr)
        {
            return shared_ptr<Val>(new Sequ(f1(p->getr1()), f2(p->getr2())));
        }
        return v;
    };
    return ret;
}

// REGEX
shared_ptr<Val> REGEX::mkeps() const
{
    throw LexingError("Regular expression not right class line 188");
}

// ZERO
bool ZERO::nullable() const
{
    return false;
}

shared_ptr<REGEX> ZERO::der(char c)
{
    return {zero()};
}

string ZERO::str() const
{
    return "ZERO";
}

pair<shared_ptr<REGEX>, Function> ZERO::simp()
{
    pair<shared_ptr<REGEX>, Function> ret{zero(), F_ID};
    return ret;
}

bool ZERO::isZero() const { return true; }

// ONE
bool ONE::nullable() const
{
    return true;
}

shared_ptr<REGEX> ONE::der(char c)
{
    return {zero()};
}

string ONE::str() const
{
    return "ONE";
}

pair<shared_ptr<REGEX>, Function> ONE::simp()
{
    pair<shared_ptr<REGEX>, Function> ret{one(), F_ID};
    return ret;
}

bool ONE::isOne() const { return true; }

shared_ptr<Val> ONE::mkeps() const
{
    shared_ptr<Empty> e(new Empty());
    return e;
}

// CHAR
bool CHAR::nullable() const
{
    return false;
}

shared_ptr<REGEX> CHAR::der(char d)
{
    if (c == d)
    {
        return one();
    }
    return zero();
}

string CHAR::str() const
{
    string s(1, c);
    return s;
}

pair<shared_ptr<REGEX>, Function> CHAR::simp()
{
    pair<shared_ptr<REGEX>, Function> ret{cha(c), F_ID};
    return ret;
}

// ALT
bool ALT::nullable() const
{
    return r1->nullable() || r2->nullable();
}

shared_ptr<REGEX> ALT::der(char c)
{
    return {alt(r1->der(c), r2->der(c))};
}

string ALT::str() const
{
    return "(" + r1->str() + " || " + r2->str() + ")";
}

pair<shared_ptr<REGEX>, Function> ALT::simp()
{
    pair<shared_ptr<REGEX>, Function> n1 = r1->simp();
    pair<shared_ptr<REGEX>, Function> n2 = r2->simp();

    if (n1.first->isZero())
    {
        return pair<shared_ptr<REGEX>, Function>{n2.first, F_RIGHT(n2.second)};
    }
    if (n2.first->isZero())
    {
        return pair<shared_ptr<REGEX>, Function>{n1.first, F_LEFT(n1.second)};
    }
    if (n1.first == n2.first)
    {
        return pair<shared_ptr<REGEX>, Function>{n1.first, F_LEFT(n1.second)};
    }
    return pair<shared_ptr<REGEX>, Function>{alt(n1.first, n2.first), F_ALT(n1.second, n2.second)};
}

shared_ptr<Val> ALT::mkeps() const
{
    if (r1->nullable())
    {
        shared_ptr<Val> e(new Left(r1->mkeps()));
        return e;
    }
    shared_ptr<Right> e(new Right(r2->mkeps()));
    return e;
}

shared_ptr<REGEX> ALT::getr1()
{
    return r1;
}

shared_ptr<REGEX> ALT::getr2()
{
    return r2;
}

// SEQ
bool SEQ::nullable() const
{
    return r1->nullable() && r2->nullable();
}

shared_ptr<REGEX> SEQ::der(char c)
{

    if (r1->nullable())
    {
        return alt(seq(r1->der(c), r2), r2->der(c));
    }
    return seq(r1->der(c), (r2));
}

string SEQ::str() const
{
    return "(" + r1->str() + " && " + r2->str() + ")";
}

pair<shared_ptr<REGEX>, Function> SEQ::simp()
{
    // cout << r1->str() << " test\n";
    // cout << r2->str() << " test\n";
    pair<shared_ptr<REGEX>, Function> n1 = r1->simp();
    pair<shared_ptr<REGEX>, Function> n2 = r2->simp();

    if (n1.first->isZero() || n2.first->isZero())
    {
        return pair<shared_ptr<REGEX>, Function>(zero(), F_ERROR);
    }
    if (n1.first->isOne())
    {
        return pair<shared_ptr<REGEX>, Function>(n2.first, F_SEQ_Empty1(n1.second, n2.second));
    }
    if (n2.first->isOne())
    {
        return pair<shared_ptr<REGEX>, Function>(n1.first, F_SEQ_Empty2(n1.second, n2.second));
    }
    return pair<shared_ptr<REGEX>, Function>(seq(n1.first, n2.first), F_SEQ(n1.second, n2.second));
}

shared_ptr<Val> SEQ::mkeps() const
{
    shared_ptr<Sequ> e(new Sequ(r1->mkeps(), r2->mkeps()));
    return e;
}

shared_ptr<REGEX> SEQ::getr1()
{
    return r1;
}

shared_ptr<REGEX> SEQ::getr2()
{
    return r2;
}

// STAR
bool STAR::nullable() const
{
    return true;
}

shared_ptr<REGEX> STAR::der(char c)
{
    return seq(r1->der(c), star((r1)));
}

string STAR::str() const
{
    return r1->str() + "*";
}

pair<shared_ptr<REGEX>, Function> STAR::simp()
{
    pair<shared_ptr<REGEX>, Function> ret{star((r1)), F_ID};
    return ret;
}

shared_ptr<Val> STAR::mkeps() const
{
    shared_ptr<Stars> e(new Stars(vector<shared_ptr<Val>>{}));
    return e;
}

std::shared_ptr<REGEX> STAR::getr() const
{
    return r1;
}

// PLUS
bool PLUS::nullable() const
{
    return r1->nullable();
}

shared_ptr<REGEX> PLUS::der(char c)
{
    return seq(r1->der(c), star(r1));
}

string PLUS::str() const
{
    return r1->str() + "+";
}

pair<shared_ptr<REGEX>, Function> PLUS::simp()
{
    pair<shared_ptr<REGEX>, Function> ret{plus(r1), F_ID};
    return ret;
}

shared_ptr<Val> PLUS::mkeps() const
{
    shared_ptr<Plus> e(new Plus(vector<shared_ptr<Val>>{r1->mkeps()}));
    return e;
}

std::shared_ptr<REGEX> PLUS::getr() const
{
    return r1;
}

// NTIMES
bool NTIMES::nullable() const
{
    return (i == 0) ? true : r1->nullable();
}

shared_ptr<REGEX> NTIMES::der(char c)
{
    if (i == 0)
    {
        return zero();
    }
    return seq(r1->der(c), ntimes(r1, i - 1));
}

string NTIMES::str() const
{
    return r1->str() + "{" + std::to_string(i) + "}";
}

pair<shared_ptr<REGEX>, Function> NTIMES::simp()
{
    pair<shared_ptr<REGEX>, Function> ret{ntimes(r1, i), F_ID};
    return ret;
}

shared_ptr<Val> NTIMES::mkeps() const
{
    if (i == 0)
    {
        shared_ptr<Ntimes> e(new Ntimes(vector<shared_ptr<Val>>{}));
        return e;
    }
    shared_ptr<Ntimes> e(new Ntimes(vector<shared_ptr<Val>>{r1->mkeps()}));
    return e;
}

std::shared_ptr<REGEX> NTIMES::getr() const
{
    return r1;
}

// RANGE
bool RANGE::nullable() const
{
    return false;
}

shared_ptr<REGEX> RANGE::der(char c)
{
    if (s.find(c) != s.end())
    {
        return one();
    }
    return zero();
}

string RANGE::str() const
{
    string r = "[ ";
    for (char const &c : s)
    {
        r = r + c + ", ";
    }
    r = r + "]";
    return r;
}

pair<shared_ptr<REGEX>, Function> RANGE::simp()
{
    pair<shared_ptr<REGEX>, Function> ret{range(s), F_ID};
    return ret;
}

// ID
bool ID::nullable() const
{
    return r1->nullable();
}

shared_ptr<REGEX> ID::der(char c)
{
    return r1->der(c);
}

string ID::str() const
{
    return s + ": (" + r1->str() + ")";
}

pair<shared_ptr<REGEX>, Function> ID::simp()
{
    pair<shared_ptr<REGEX>, Function> ret{id(s, r1), F_ID};
    return ret;
}

shared_ptr<Val> ID::mkeps() const
{
    shared_ptr<Rec> e(new Rec(s, r1->mkeps()));
    return e;
}

std::string ID::getStr() const
{
    return s;
}

std::shared_ptr<REGEX> ID::getr() const
{
    return r1;
}
