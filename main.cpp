#include "Include/r.hpp"
#include "Include/v.hpp"
#include "Exceptions/LexingError.cpp"
#include <cstdlib>

std::ostream& operator<<(std::ostream& os,  std::pair<string,string> const& p)
{
    os << "(" << p.first << ","  ;
    for (int i = 0 ; i < p.second.length() ; i++ ){
        if(p.second[i] == '\n'){
            cout<<"\\n";
        }
        else{
            cout << p.second[i];
        }
    }
    os << ")";
    return os;
}

//https://www.geeksforgeeks.org/passing-a-function-as-a-parameter-in-cpp/
//for lexing keep a list of regular expressions to go back on?
std::shared_ptr<Val> lex(std::shared_ptr<REGEX> r,string s, int i){
    if(i >= s.size()){
        (r->nullable())?:throw LexingError("Input doesnt match regular expression");
        std::shared_ptr<Val> ret = r->mkeps();
        return ret;
    }
    //lex(der(s[i],r)->simp(),s,++i)->inj(r,s[i]);
    shared_ptr<REGEX> reg = der(s[i],r);
    ++i;
    std::shared_ptr<Val> val = nullptr;
    try {
        val = lex(reg,s,i)->inj(r,s[i-1]);
        //cout << val->str() << "\n";
    }
    catch(LexingError &e){
        cout << e.what() <<"\n";
        exit(0);
    }
    return val;
}

vector<pair<string,string>> tokenise(std::shared_ptr<REGEX> r,string s){
    return lex(r,s,0)->env();
}

void printVector(vector<pair<string,string>> vec){
    for(int i =0; i < vec.size();++i){
        cout << vec[i] << ",";
    }
    cout << "\n";
}

// int main()
// {
//     string digitstr = "0123456789";
//     shared_ptr<REGEX> digit = id("numbers",star(range(set<char>(begin(digitstr), end(digitstr)))));
//     auto ret = tokenise(digit,"079");
//     printVector(ret);
   
   
// }

int main(){
    string digitstr = "0123456789";
    string digitstr1 = "123456789";
    string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string sym = "._><=;,\':";
    vector<string>{"while","if","then","else","do","for","to","true","false","read","write","skip"};

    shared_ptr<RANGE> digit = range(set<char>(begin(digitstr), end(digitstr)));
    shared_ptr<RANGE> digit1 = range(set<char>(begin(digitstr1), end(digitstr1)));
    shared_ptr<ALT> num = alt(digit,seq(digit1,star(digit)));

    shared_ptr<ALT> whitespaces = alt(alt(alt(cha('\n'),cha('\t')),cha('\r')),cha(' '));
    shared_ptr<ALT> spaces = alt(alt(cha('\n'),cha('\t')),cha('\r'));

    shared_ptr<RANGE> letters = range(set<char> (begin(characters), end(characters)));
    shared_ptr<ALT> symbols =  alt(letters,range(set<char> (begin(sym), end(sym))));

    shared_ptr<ID> keyword = id("key",stringList2rexp(vector<string>{"while","if","then","else","do","for","to","true","false","read","write","skip"}));
    shared_ptr<ID> comment = id("com",seq(seq(seq(cha('/'),cha('/')),star(alt(alt(symbols,digit),cha(' ')))),spaces)); 
    shared_ptr<ID> string = id("str",seq(seq(cha('\"'),star(alt(alt(symbols,whitespaces),digit))),cha('\"')));

    shared_ptr<STAR> lang_regs = star(alt(comment,alt(string,keyword)));

    auto ret = tokenise(lang_regs,"//this is a comment\nthenwhilefor");
    printVector(ret);

    return 0;
}