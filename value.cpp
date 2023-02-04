#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <memory>

using std::cout;
using std::set;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::move;


class Val{
    public:
        virtual string str() const = 0;
};

class Empty : public Val{
    public:
        string str()const{
            string ret = "Empty";
            return ret;
        }
};

class Chr : public Val{
    private:
        char c;
    public:
        Chr(char in){
            c = in;
        }

        string str()const{
            return string(1,c);
        }
};

class Left : public Val{
    private:
        shared_ptr<Val> v;
    public:
        Left(shared_ptr<Val> in):v(in){}

        string str()const{
            string s = "Left( ";
            s = s + v->str();
            s = s + " )";
            return s;
        }
};

class Right : public Val{
    private:
        shared_ptr<Val> v;
    public: 
        Right(shared_ptr<Val> in):v(in){}

        string str()const{
            string s = "Right( ";
            s = s + v->str();
            s = s + " )";
            return s;
        }
};

class Sequ : public Val{
    private:
        shared_ptr<Val> v1;
        shared_ptr<Val> v2;
    public:
        Sequ(shared_ptr<Val> in,shared_ptr<Val> in2):v1(in),v2(in2){}

        string str()const{
            string s = "Sequ(";
            s = s + v1->str();
            s = s + " , ";
            s = s + v2->str();
            s = s + ")";
            return s;
        }
};

class Stars : public Val{
    private:
        vector<shared_ptr<Val>> v; //possibly change to list of shared pointers to val
    public: 
        Stars(vector<shared_ptr<Val>> in):v(in){}

        string str()const{
            if(v.size() == 0){
                return "Stars(Nil)";
            }
            string s = "Stars( ";
            for(int i = 0; i < v.size();++i){
                s = s + v[i]->str();
                s = s + " ";
            }
            s = s + " )";
            return s;
        }
};

class Rec : public Val{
    private:
        shared_ptr<Val> v;
        string s;
    public: 
        Rec(string sin,shared_ptr<Val> in):v(in),s(sin){}

        string str()const{
            string ret = "Rec( ";
            ret = s + " , " + v->str();
            ret = ret + " )";
            return ret;
        }
};

class Plus : public Val{
    private:
        vector<shared_ptr<Val>> v; //possibly change to list of shared pointers to val
    public: 
        Plus(vector<shared_ptr<Val>> in):v(in){}

        string str()const{
            if(v.size() == 0){
                return "Plus(Nil)";
            }
            string s = "Plus( ";
            for(int i = 0; i < v.size();++i){
                s = s + v[i]->str();
                s = s + " ";
            }
            s = s + " )";
            return s;
        }
};

class Ntimes : public Val{
     private:
        vector<shared_ptr<Val>> v; //possibly change to list of shared pointers to val
    public: 
        Ntimes(vector<shared_ptr<Val>> in):v(in){}

        string str()const{
            if(v.size() == 0){
                return "Ntimes(Nil)";
            }
            string s = "Ntimes( ";
            for(int i = 0; i < v.size();++i){
                s = s + v[i]->str();
                s = s + " ";
            }
            s = s + " )";
            return s;
        }
};

// def mkeps(r: Rexp) : Val = r match {
//   case ONE => Empty
//   case ALT(r1, r2) => 
//     if (nullable(r1)) Left(mkeps(r1)) else Right(mkeps(r2))
//   case SEQ(r1, r2) => Sequ(mkeps(r1), mkeps(r2))
//   case STAR(r) => Stars(Nil)
//   case RECD(x, r) => Rec(x, mkeps(r))
//   case PLUS(r) => Plus(List(mkeps(r)))
//   case OPTIONAL(r) => Optional(Empty)
//   case NTIMES(r,n) => if (n == 0) Ntimes(Nil) else Ntimes(List(mkeps(r)))
// }


// int main(){
//     string digitstr = "0123456789";
//     shared_ptr<REGEX> digit = range(set<char>(begin(digitstr), end(digitstr)));
//     cout << "ok\n";
//     return 0;
// }