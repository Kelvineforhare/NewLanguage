#include "Include/regex.hpp"
#include "Include/value.hpp"
#include "Include/token.hpp"
#include "Include/parser.hpp"
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

std::ostream& operator<<(std::ostream& os,  std::pair<shared_ptr<Token>,vector<shared_ptr<Token>>> const& p)
{
    os << "(" << p.first->toString() << " : "  ;
    for (int i = 0 ; i < p.second.size() ; i++ ){
        cout << p.second[i]->toString() << ", ";
    }
    os << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os,  std::pair<pair<shared_ptr<Token>,shared_ptr<Token>>,vector<shared_ptr<Token>>> const& p)
{
    os << "(" << p.first.first->toString() << " , "  ;
    os << p.first.second->toString() << " : "  ;
    for (int i = 0 ; i < p.second.size() ; i++ ){
        cout << p.second[i]->toString() << ", ";
    }
    os << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os,  std::pair<int,vector<shared_ptr<Token>>> const& p)
{
    os << "(" << (p.first) << " : "  ;
    for (int i = 0 ; i < p.second.size() ; i++ ){
        cout << p.second[i]->toString() << ", ";
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
    auto reg = der(s[i],r)->simp();
    ++i;
    std::shared_ptr<Val> val = nullptr;
    try {
        val = lex(reg.first,s,i);
        val = reg.second(val);
        val = val->inj(r,s[i-1]);
    }
    catch(LexingError &e){
        cout << e.what() <<"\n";
        exit(0);
    }
    return val;
}

vector<pair<string,string>> lexing(std::shared_ptr<REGEX> r,string s){
    return lex(r,s,0)->env();
}

shared_ptr<Token> getTokFromStr(pair<string,string> s)
{
    if (s.first == "semi") return shared_ptr<Token>(new T_SEMI());
    if (s.first == "(") return shared_ptr<Token>(new T_LPAREN());   
    if (s.first == ")") return shared_ptr<Token>(new T_RPAREN());   
    if (s.first == "{") return shared_ptr<Token>(new T_LBRACK());
    if (s.first == "}") return shared_ptr<Token>(new T_RBRACK());
    if (s.first == "id") return shared_ptr<Token>(new T_ID(s.second));
    if (s.first == "op") return shared_ptr<Token>(new T_OP(s.second));
    if (s.first == "int") return shared_ptr<Token>(new T_INT(stoi(s.second)));
    if (s.first == "key") return shared_ptr<Token>(new T_KWD(s.second));
    if (s.first == "str") return shared_ptr<Token>(new T_STR(string(s.second.begin()+1,s.second.end()-1)));
}

vector<shared_ptr<Token>> tokenise(vector<pair<string,string>> input)
{
    vector<shared_ptr<Token>> tokens;
    for(int i = 0; i < input.size();++i){
        tokens.push_back(getTokFromStr(input[i]));
    }
    return tokens;
}

void printVector(vector<pair<string,string>> vec){
    for(int i =0; i < vec.size();++i){
        cout << vec[i] << ",";
    }
    cout << "\n";
}

void printTokVector(vector<shared_ptr<Token>> vec){
    for(int i =0; i < vec.size();++i){
        cout << vec[i]->toString() << ",";
    }
    cout << "\n";
}



// int main()
// {
//     // string digitstr = "0123456789";
//     shared_ptr<REGEX> r = id("test",alt(alt(star(cha('a')),cha('a')),cha('a')));

//     // auto ret2 = tokenise2(r,"aa");
//     // printVector(ret2);
//     auto ret = tokenise(r,"aa");
//     printVector(ret);
   
// }

// int main(){
//     string digitstr = "0123456789";
//     string digitstr1 = "123456789";
//     string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//     string sym = "._><=;,\':";
//     vector<string>{"while","if","then","else","do","for","to","true","false","read","write","skip"};

//     shared_ptr<RANGE> digit = range(set<char>(begin(digitstr), end(digitstr)));
//     shared_ptr<RANGE> digit1 = range(set<char>(begin(digitstr1), end(digitstr1)));
//     shared_ptr<ALT> num = alt(digit,seq(digit1,star(digit)));

//     shared_ptr<ALT> whitespaces = alt(alt(alt(cha('\n'),cha('\t')),cha('\r')),cha(' '));
//     shared_ptr<ALT> spaces = alt(alt(cha('\n'),cha('\t')),cha('\r'));

//     shared_ptr<RANGE> letters = range(set<char> (begin(characters), end(characters)));
//     shared_ptr<ALT> symbols =  alt(letters,range(set<char> (begin(sym), end(sym))));

//     shared_ptr<ID> keyword = id("key",stringList2rexp(vector<string>{"while","if","then","else","for","true","false","input","print"}));
//     shared_ptr<ID> comment = id("com",seq(seq(seq(cha('/'),cha('/')),star(alt(alt(symbols,digit),cha(' ')))),spaces)); 
//     shared_ptr<ID> string = id("str",seq(seq(cha('\"'),star(alt(alt(symbols,whitespaces),digit))),cha('\"')));

//     shared_ptr<STAR> lang_regs = star(alt(alt(comment,keyword),string));

//     auto ret = lexing(lang_regs,"\"kelvin\"");
//     printVector(ret);
//     printTokVector(tokenise(ret));
//     return 0;
// }

// int main(){
//     shared_ptr<Token> token(new T_KWD("for"));
//     shared_ptr<Token> token2(new T_KWD("while"));

//     shared_ptr<Token> token3(new T_KWD("for"));
//     shared_ptr<Token> token4(new T_KWD("while"));

//     TokenParser tok =  TokenParser(token);
//     TokenParser tok2 =  TokenParser(token2);
//     auto tokseq =  MapParser<shared_ptr<Token>,int>(tok,test);
    
//     vector<shared_ptr<Token>> list;
//     list.push_back(token3);

//     auto output = tokseq.parse(list);
//     auto it = output.begin();
//     if(it != output.end()){
//         cout << *it << "\n"; 
//     }
//     return 0;
// }

int test(shared_ptr<Token> test){
    return 10;
}

struct Te{
    shared_ptr<Token> token = shared_ptr<Token>(new T_OP("*"));
};

struct AExp{
    Te parser;
    shared_ptr<Token> token = shared_ptr<Token>(new T_OP("+"));
    shared_ptr<AExp>  e;
};



