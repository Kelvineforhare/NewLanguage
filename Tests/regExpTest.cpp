#include "lexer.cpp"
#include <cstdlib>

using std::to_string;

#define IS_TRUE(x) { if (!(x)) cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else cout << __FUNCTION__ << ": PASS\n"; }
#define IS_FALSE(x) { if ((x)) cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else cout << __FUNCTION__ << ": PASS\n"; }

string digitstr = "0123456789";
string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

string generateNumNtimes(int times){
    string s = "";
    for(int i = 0; i < times;++i){
        s += to_string(rand()%10); 
    }
    return s;
}

string generateCharNtimes(int times){
    string s = "";
    for(int i = 0; i < times;++i){
        if(rand()%2 == 1){
            s += string(1,65 + rand()%25);
        }
        else{
            s += string(1,97 + rand()%25);
        }
    }
    return s;
}

string generateStringStartWithNumber(){
    string s = generateNumNtimes(1) + generateCharNtimes(100);
    return s;
}

string generateAorBNtimes(int times){
    string s = "";
    for(int i = 0; i < times;++i){
        s += (rand()%2 == 1)?'a':'b';
    }
    return s;
}

void testForRegExp1(){
    shared_ptr<REGEX> t1 = seq(range(set<char>(begin(digitstr), end(digitstr))),star(range(set<char>(begin(characters), end(characters)))));
    IS_TRUE(matcher(t1,generateStringStartWithNumber()));
    IS_TRUE(matcher(t1,generateNumNtimes(1)));
    IS_FALSE(matcher(t1,generateNumNtimes(1 + rand()%100)));
    IS_FALSE(matcher(t1,generateCharNtimes(100)));
    IS_FALSE(matcher(t1,""));
    IS_FALSE(matcher(t1," "));
}


void testForRegExp2(){
    shared_ptr<REGEX> t2 = star(ntimes(range(set<char>(begin(digitstr), end(digitstr))),10));
    IS_TRUE(matcher(t2,generateNumNtimes(10 * (rand()%10))));
    IS_TRUE(matcher(t2,""));
    IS_FALSE(matcher(t2,generateStringStartWithNumber()));
     //IS_TRUE(matcher(t2,generateNumNtimes(10 * (rand()%10))));
}


void testForRegExp3(){
    shared_ptr<REGEX> t3 = plus(alt(cha('a'),cha('b')));
    IS_TRUE(matcher(t3,generateAorBNtimes(100)));
    
}



void testForValidity()
{
    testForRegExp1();
    testForRegExp2();
    testForRegExp3();
}

void testForSpeed()
{
    
}

// int main()
// {
//     testForValidity();
// }