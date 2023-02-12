#include "Include/r.hpp"
#include "Include/v.hpp"
#include "Exceptions/LexingError.cpp"

//Val
std::shared_ptr<Val> Val::inj(std::shared_ptr<REGEX> r, char c) const{
     throw LexingError();
}



//Chr
string Chr::str()const{
    return string(1,c);
}



//Empty
string Empty::str()const{
    string ret = "Empty";
    return ret;
}

shared_ptr<Val> Empty::inj(shared_ptr<REGEX> r, char c) const{
    shared_ptr<Val> e(new Chr(c));
    return e;
}


//Left
string Left::str()const{
    string s = "Left( ";
    s = s + v->str();
    s = s + " )";
    return s;
}



//Right
string Right::str()const{
    string s = "Right( ";
    s = s + v->str();
    s = s + " )";
    return s;
}

std::shared_ptr<Val> Right::inj(std::shared_ptr<REGEX> r,char c) const {
    cout << r->str() << "\n";

    std::shared_ptr<SEQ> p1 = std::dynamic_pointer_cast<SEQ>(r);
    if(p1 != nullptr){
        std::shared_ptr<Val> ret(new Sequ(p1->getr1()->mkeps(),v->inj(p1->getr2(),c)));
        return ret;
    }

    std::shared_ptr<ALT> p2 = std::dynamic_pointer_cast<ALT>(r);
    if(p2 != nullptr){
        std::shared_ptr<Val> ret(new Right(v->inj(p2->getr2(),c)));
        return ret;
    }
    throw LexingError();
    return nullptr;
}




string Sequ::str()const{
    string s = "Sequ(";
    s = s + v1->str();
    s = s + " , ";
    s = s + v2->str();
    s = s + ")";
    return s;
}

string Stars::str()const{
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

string Rec::str()const{
    string ret = "Rec( ";
    ret = s + " , " + v->str();
    ret = ret + " )";
    return ret;
}

string Plus::str()const{
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

string Ntimes::str()const{
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


//Regular expressions used to show what text should match


//https://www.reddit.com/r/cpp_questions/comments/9w5u6k/shared_ptr_and_implicit_conversions/






