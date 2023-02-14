#include "Include/r.hpp"
#include "Include/v.hpp"
#include "Exceptions/LexingError.cpp"

//Val
std::shared_ptr<Val> Val::inj(std::shared_ptr<REGEX> r, char c) const{
    //check  if ID regex
    std::shared_ptr<ID> p1 = std::dynamic_pointer_cast<ID>(r);
    if(p1 != nullptr){
        std::shared_ptr<Val> ret(new Rec(p1->getStr(),this->inj(p1->getr(),c)));
    }
    throw LexingError();
}



//Chr
string Chr::str()const{
    string s(1, c);
    return s;
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

std::shared_ptr<Val> Left::inj(std::shared_ptr<REGEX> r, char c) const{
    cout << r->str() << "\n";

    std::shared_ptr<ALT> p1 = std::dynamic_pointer_cast<ALT>(r);
    if(p1 != nullptr){
        std::shared_ptr<Val> ret(new Left(v->inj(p1->getr1(),c)));
        return ret;
    }

    std::shared_ptr<SEQ> p2 = std::dynamic_pointer_cast<SEQ>(r);
    if(p2 != nullptr){
        std::shared_ptr<Sequ> p3 = std::dynamic_pointer_cast<Sequ>(v);
        if(p3 != nullptr){
            std::shared_ptr<Val> ret(new Sequ(p3->getr1()->inj(p2->getr1(),c),p3->getr2()));
            return ret;
        }
    }
    throw LexingError();
    return nullptr;
}



//Right
string Right::str()const{
    string s = "Right( ";
    s = s + v->str();
    s = s + " )";
    return s;
}

std::shared_ptr<Val> Right::inj(std::shared_ptr<REGEX> r,char c) const {
    std::shared_ptr<SEQ> p1 = std::dynamic_pointer_cast<SEQ>(r);
    if(p1 != nullptr){
        cout << string(1,c) << "\n";
        printf("%d", c); //input char becomes empty ????
        std::shared_ptr<Val> ret(new Sequ(p1->getr1()->mkeps(),v->inj(p1->getr2(),c)));
       
        // cout << v->inj(p1->getr2(),c)->str() << "\n";
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



//Sequ
string Sequ::str()const{
    string s = "Sequ(";
    s = s + v1->str();
    s = s + " , ";
    s = s + v2->str();
    s = s + ")";
    return s;
}

//(STAR(r), Sequ(v1, Stars(vs))) => Stars(inj(r, c, v1)::vs)
//(SEQ(r1, r2), Sequ(v1, v2)) => Sequ(inj(r1, c, v1), v2)
//(PLUS(r1), Sequ(v, Stars(values))) => Plus(inj(r1, c, v)::values)
//(NTIMES(r1,_), Sequ(r, Ntimes(cs))) => Ntimes(inj(r1, c, r)::cs)
std::shared_ptr<Val> Sequ::inj(std::shared_ptr<REGEX> r, char c) const{
    std::shared_ptr<SEQ> p1 = std::dynamic_pointer_cast<SEQ>(r);
    if(p1 != nullptr){
        std::shared_ptr<Val> ret(new Sequ(v1->inj(p1->getr1(),c),v2));
        return ret;
    }
    std::shared_ptr<STAR> p2 = std::dynamic_pointer_cast<STAR>(r);
    if(p2 != nullptr){
        std::shared_ptr<Stars> p3 = std::dynamic_pointer_cast<Stars>(v2);
        (p3)?:throw LexingError();
        std::vector<std::shared_ptr<Val>> newList = p3->getList();
        newList.insert(newList.begin(),v1->inj(p2->getr(),c));
        std::shared_ptr<Val> ret(new Stars(newList));
        return ret;
    } 
    std::shared_ptr<PLUS> p4 = std::dynamic_pointer_cast<PLUS>(r);
    if(p4 != nullptr){
        std::shared_ptr<Stars> p5 = std::dynamic_pointer_cast<Stars>(v2);
        (p5)?:throw LexingError();
        std::vector<std::shared_ptr<Val>> newList = p5->getList();
        newList.insert(newList.begin(),v1->inj(p4->getr(),c));
        std::shared_ptr<Val> ret(new Plus(newList));
        return ret;
    } 
    std::shared_ptr<NTIMES> p6 = std::dynamic_pointer_cast<NTIMES>(r);
    if(p6 != nullptr){
        std::shared_ptr<Ntimes> p7 = std::dynamic_pointer_cast<Ntimes>(v2);
        (p7)?:throw LexingError();
        std::vector<std::shared_ptr<Val>> newList = p7->getList();
        newList.insert(newList.begin(),v1->inj(p6->getr(),c));
        std::shared_ptr<Val> ret(new Ntimes(newList));
        return ret;
    }
    throw LexingError();
    return nullptr;
}

std::vector<std::shared_ptr<Val>> Stars::getList(){
    return v;
}

std::shared_ptr<Val> Sequ::getr1() const{
    return v1;
}
std::shared_ptr<Val> Sequ::getr2() const{
    return v2;
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

std::vector<std::shared_ptr<Val>> Ntimes::getList(){
    return v;
}


//Regular expressions used to show what text should match


//https://www.reddit.com/r/cpp_questions/comments/9w5u6k/shared_ptr_and_implicit_conversions/






