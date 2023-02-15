#include "Include/v.hpp"
#include "Include/r.hpp"
#include "Exceptions/LexingError.cpp"

using std::cout;
using std::set;
using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::move;


std::shared_ptr<ONE> one(){
    std::shared_ptr<ONE> one (new ONE());
    return one;
}

std::shared_ptr<ZERO> zero(){
    std::shared_ptr<ZERO> zero(new ZERO());
    return zero;
}

std::shared_ptr<CHAR> cha(char c){
    std::shared_ptr<CHAR> p(new CHAR(c));
    return p;
}

std::shared_ptr<ALT> alt(std::shared_ptr<REGEX> r1,std::shared_ptr<REGEX> r2){
    std::shared_ptr<ALT> a (new ALT((r1),(r2)));
    return a;
}

std::shared_ptr<SEQ> seq(std::shared_ptr<REGEX> r1,std::shared_ptr<REGEX> r2){
    std::shared_ptr<SEQ> seq(new SEQ((r1),(r2)));
    return seq;
}

std::shared_ptr<STAR> star(std::shared_ptr<REGEX> r1){
    std::shared_ptr<STAR> s(new STAR((r1)));
    return s;
}

std::shared_ptr<PLUS> plus(std::shared_ptr<REGEX> r1){
    std::shared_ptr<PLUS> p(new PLUS(r1));
    return p;
}

std::shared_ptr<NTIMES> ntimes(std::shared_ptr<REGEX> r1, int i){
    std::shared_ptr<NTIMES> n(new NTIMES(r1,i));
    return n;
}

std::shared_ptr<RANGE> range(set<char> s){
    std::shared_ptr<RANGE> r(new RANGE(s));
    return r;
}

std::shared_ptr<ID> id(string str,std::shared_ptr<REGEX> r1){
    std::shared_ptr<ID> id(new ID(r1,str));
    return id;
}


shared_ptr<REGEX> der(char c,shared_ptr<REGEX> r){
    return r->der(c);
}

shared_ptr<REGEX> ders(const vector<char> & str, shared_ptr<REGEX> r){
    int i = 0;
    if(i >= str.size()){
        return r;
    }
    shared_ptr<REGEX> s = der(str[i],r)->simp();
    ++i;
    for(;i < str.size();++i){
        s = der(str[i],s)->simp();
    }
    return s;
}

bool matcher(shared_ptr<REGEX> r,const string & s)
{
    vector<char> v(s.begin(), s.end());
    return ders(v,r)->nullable();
}

shared_ptr<REGEX> string2rexp(const string & s){
  if(s.size() == 0){
    return one();
  }
  if(s.size() == 1){
    return cha(s[0]);
  }
  shared_ptr<REGEX> ret = seq(cha(s[0]),cha(s[1]));
  for(int i = 2; i < s.size();++i){
        ret = seq(ret,cha(s[i]));
  }
  return ret;
}

shared_ptr<REGEX> stringList2rexp(const vector<string> & s){
  if(s.size() == 0){
    return one();
  }
  if(s.size() == 1){
    return string2rexp(s[0]);
  }
  shared_ptr<REGEX> ret = alt(string2rexp(s[0]),string2rexp(s[1]));
  for(int i = 2; i < s.size();++i){
        ret = alt(ret,string2rexp(s[i]));
  }
  return ret;
}

//REGEX
std::shared_ptr<Val> REGEX::mkeps() const{
    throw LexingError("Regular expression not right class line 188");
}



//ZERO
bool ZERO::nullable() const{
    return false;
}

shared_ptr<REGEX> ZERO::der(char c) {
    return {zero()};
}

string ZERO::str() const {
    return "ZERO";
}

shared_ptr<REGEX> ZERO::simp() {
    return {zero()};
}

bool ZERO::isZero() const {return true;}



//ONE
bool ONE::nullable() const{
    return true;
}

shared_ptr<REGEX> ONE::der(char c) {
    return {zero()};
}

string ONE::str() const{
    return "ONE";
}

shared_ptr<REGEX> ONE::simp(){
    return {one()};
}

bool ONE::isOne()const {return true;}

shared_ptr<Val> ONE::mkeps() const {
    shared_ptr<Empty> e(new Empty());
    return e;
}



//CHAR
bool CHAR::nullable() const{
    return false;
}

shared_ptr<REGEX> CHAR::der(char d){
    if(c == d){
        return one();
    }
    return zero();
}

string CHAR::str() const{
    string s(1, c);
    return s;
}

shared_ptr<REGEX> CHAR::simp() {
    return {cha(c)};
}



//ALT
bool ALT::nullable() const{
    return r1->nullable() || r2->nullable();
}

shared_ptr<REGEX> ALT::der(char c) {
    return {alt(r1->der(c),r2->der(c))};
}

string ALT::str() const{
    return "(" + r1->str() + " || " + r2->str() + ")";
}

shared_ptr<REGEX> ALT::simp() {
    shared_ptr<REGEX> n1 = r1->simp();
    shared_ptr<REGEX> n2 = r2->simp();
    
    if(n1->isZero()){
        return n2;
    }
    if(n2->isZero()){
        return n1;                                          
    }
    if(n1 == n2){
        return n1;
    }
    return alt(n1,n2);
}

shared_ptr<Val> ALT::mkeps() const {
    if(r1->nullable()){
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



//SEQ
bool SEQ::nullable() const {
    return r1->nullable() && r2->nullable();
}

shared_ptr<REGEX> SEQ::der(char c){
    
    if(r1->nullable()){
        return alt(seq(r1->der(c),r2),r2->der(c));
    }
    return seq(r1->der(c),(r2));
}

string SEQ::str() const{
    return "(" +  r1->str() + " && " + r2->str() + ")";
}

shared_ptr<REGEX> SEQ::simp() {
    shared_ptr<REGEX> n1 = r1->simp();
    shared_ptr<REGEX> n2 = r2->simp();

    if(n1->isZero() || n2->isZero()){
        
        return zero();
    }
    if(n1->isOne()){
        return n2;
    }
    if(n2->isOne()){
        return n1;
    }
    return seq((n1),(n2)); 
}

shared_ptr<Val> SEQ::mkeps() const {
    shared_ptr<Sequ> e(new Sequ(r1->mkeps(),r2->mkeps()));
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



//STAR
bool STAR::nullable() const{
    return true;
}

shared_ptr<REGEX> STAR::der(char c) {
    return seq(r1->der(c),star((r1)));
}

string STAR::str() const{
    return r1->str() + "*";
}

shared_ptr<REGEX> STAR::simp(){
    return star((r1));
}

shared_ptr<Val> STAR::mkeps() const {
    shared_ptr<Stars> e(new Stars(vector<shared_ptr<Val>>{}));
    return e;
}

std::shared_ptr<REGEX> STAR::getr() const{
    return r1;
}



//PLUS
bool PLUS::nullable() const{
    return r1->nullable();
}

shared_ptr<REGEX> PLUS::der(char c){
    return seq(r1->der(c),star(r1));
}

string PLUS::str() const {
    return r1->str() + "+";
}

shared_ptr<REGEX> PLUS::simp(){
    return {plus(r1)};
}

shared_ptr<Val> PLUS::mkeps() const {
    shared_ptr<Plus> e(new Plus(vector<shared_ptr<Val>>{r1->mkeps()}));
    return e;
}

std::shared_ptr<REGEX> PLUS::getr() const{
    return r1;
}



//NTIMES
bool NTIMES::nullable() const{
    return (i == 0)?true:r1->nullable();
}

shared_ptr<REGEX> NTIMES::der(char c){
    if(i == 0){
        return zero();
    }
    return seq(r1->der(c),ntimes(r1,i-1));
}   

string NTIMES::str() const {
    return r1->str() + "{" + std::to_string(i) + "}";
}  

shared_ptr<REGEX> NTIMES::simp(){
    return ntimes(r1,i);
}   

shared_ptr<Val> NTIMES::mkeps() const {
    if(i == 0){
        shared_ptr<Ntimes> e(new Ntimes(vector<shared_ptr<Val>>{}));
        return e;
    }
    shared_ptr<Ntimes> e(new Ntimes(vector<shared_ptr<Val>>{r1->mkeps()}));
    return e;
}

std::shared_ptr<REGEX> NTIMES::getr() const{
    return r1;
}



//RANGE
bool RANGE::nullable() const{
    return false;
}

shared_ptr<REGEX> RANGE::der(char c){
    if(s.find(c) != s.end()){
        return one();
    }
    return zero();
}

string RANGE::str() const {
    string r = "[ ";
    for (char const& c : s){
        r = r + c + ", ";
    }
    r = r + "]";
    return r;
}

shared_ptr<REGEX> RANGE::simp() {
    return range(s);
}



//ID
bool ID::nullable() const{
    return r1->nullable();
}

shared_ptr<REGEX> ID::der(char c) {
    return r1->der(c);
} 

string ID::str() const {
    return s + ": (" + r1->str() + ")";
}     

shared_ptr<REGEX> ID::simp()  {
    return {id(s,r1)};
}

shared_ptr<Val> ID::mkeps() const {
    shared_ptr<Rec> e(new Rec(s,r1->mkeps()));
    return e;
}

 std::string ID::getStr() const{
    return s;
 }

 std::shared_ptr<REGEX> ID::getr() const{
    return r1;
 }





