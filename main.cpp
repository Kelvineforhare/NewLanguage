#include "Include/r.hpp"
#include "Include/v.hpp"



int main()
{
    shared_ptr<Val> e(new Empty());
    cout << e->inj(cha('d'),'d')->str() << "\n";
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