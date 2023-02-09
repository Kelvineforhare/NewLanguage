#include "Exceptions/LexingError.cpp"
#include "Include/r.h"

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
        // virtual shared_ptr<Val> inj(shared_ptr<REGEX> r, char c) const {
        //     throw LexingError();
        // }
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


class Empty : public Val{
    public:
        string str()const{
            string ret = "Empty";
            return ret;
        }
        shared_ptr<Val> inj(shared_ptr<CHAR> r,char c) const {
            shared_ptr<Chr> ch(new Chr(c));
            return ch;
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

        shared_ptr<Val> inj(shared_ptr<ALT> r,char c) const {
            //shared_ptr<Right> right(new Right(v->inj(r->getr2(),c)));
            return nullptr;
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

//Regular expressions used to show what text should match


//https://www.reddit.com/r/cpp_questions/comments/9w5u6k/shared_ptr_and_implicit_conversions/




int main()
{
    shared_ptr<Empty> e(new Empty());
    //cout << e->inj(cha('c'),'c')->str() << "\n";
}

// int main(){
//     string digitstr = "0123456789";
//     string digitstr1 = "123456789";
//     string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//     string sym = "._><=;,\':";
//     vector<string>{"while","if","then","else","do","for","to","true","false","read","write","skip"};

//     shared_ptr<REGEX> digit = range(set<char>(begin(digitstr), end(digitstr)));
//     shared_ptr<REGEX> digit1 = range(set<char>(begin(digitstr1), end(digitstr1)));
//     shared_ptr<REGEX> num = alt(digit,seq(digit1,star(digit)));

//     shared_ptr<REGEX> whitespaces = alt(alt(alt(cha('\n'),cha('\t')),cha('\r')),cha(' '));
//     shared_ptr<REGEX> spaces = alt(alt(cha('\n'),cha('\t')),cha('\r'));

//     shared_ptr<REGEX> letters = range(set<char> (begin(characters), end(characters)));
//     shared_ptr<REGEX> symbols =  alt(letters,range(set<char> (begin(sym), end(sym))));

//     shared_ptr<REGEX> keyword = id("key",stringList2rexp(vector<string>{"while","if","then","else","do","for","to","true","false","read","write","skip"}));
//     shared_ptr<REGEX> comment = id("com",seq(seq(seq(cha('/'),cha('/')),star(alt(alt(symbols,digit),cha(' ')))),spaces)); 
//     shared_ptr<REGEX> string = id("str",seq(seq(cha('\"'),star(alt(alt(symbols,whitespaces),digit))),cha('\"')));

//     shared_ptr<REGEX> lang_regs = star(alt(comment,alt(string,keyword)));


//     cout << "else//comment\n\"This is string\"" << "\n";
//     cout << matcher(lang_regs, "else//comment\n\"This is string\"while") << "\n";

//     cout << seq(one(),star(one()))->mkeps()->str() << "\n";

//     return 0;
// }

