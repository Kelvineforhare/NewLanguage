#include <vector>
#include <set>
#include <utility> 
#include <memory>
#include <functional>
#include "syntaxTree.hpp"
#include "../Exceptions/ParsingError.cpp"

using std::set;
using std::vector;
using std::pair;
using std::shared_ptr;
using std::function;

class Token;
bool operator==(shared_ptr<Token> a,shared_ptr<Token> b);


template <typename T>
class Parser{
    public:
        virtual set<pair<T,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) = 0;

        set<T> parse_all(vector<shared_ptr<Token>> in){
            auto parsed = parse(in);
            set<T> ret;
            for(auto par: parsed){
                if(par.second.size() == 0){
                    ret.insert(par.first);
                }
            }
            return ret;
        }
};


class TokenParser: public Parser<shared_ptr<Token>>{
    private:
        shared_ptr<Token> tok;
    public:
        TokenParser(shared_ptr<Token> input){
            tok = input;
        }
        set<pair<shared_ptr<Token>,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            if(in.size() == 0){
                return set<pair<shared_ptr<Token>,vector<shared_ptr<Token>>>>();
            }
            if(in[0] == tok){
                set<pair<shared_ptr<Token>,vector<shared_ptr<Token>>>> ret;
                pair<shared_ptr<Token>,vector<shared_ptr<Token>>> pair;
                pair.first = tok;
                pair.second = vector<shared_ptr<Token>>(in.begin()+1,in.end());
                ret.insert(pair);
                return ret;
            }
            return set<pair<shared_ptr<Token>,vector<shared_ptr<Token>>>>();
        }
};

template <typename T,typename S>
class SeqParser : public Parser<pair<T,S>>{
    private:
        Parser<T> & p;
        Parser<S> & q;
    public:
        SeqParser(Parser<T> & pin,Parser<S> & qin):p(pin),q(qin){}

        set<pair<pair<T,S>,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            auto parsed1 = p.parse(in);
            
            // set<pair<pair<T,S>,vector<shared_ptr<Token>>>> ret;
            // ret.insert(pair<pair<T,S>,vector<shared_ptr<Token>>>(pair<T,S>(parsed1.first,parsed2.first),parsed2.second));
            // return ret;
            auto result = set<pair<pair<T,S>,vector<shared_ptr<Token>>>>();
            for (auto it1 = parsed1.begin(); it1 != parsed1.end();++it1) {
                auto parsed2 = q.parse(it1->second);
                for (auto it2 = parsed2.begin();it2 != parsed2.end(); ++it2) {
                    auto value = pair<pair<T,S>,vector<shared_ptr<Token>>>(pair<T,S>(it1->first, it2->first), it2->second);
                    result.insert(value);
                }
            }
            return result;
        }
};


template <typename T>
class AltParser : public Parser<T>{
    private:
        Parser<T> & p;
        Parser<T> & q;
    public:
        AltParser(Parser<T> & pin,Parser<T> & qin):p(pin),q(qin){}

        set<pair<T,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            auto parsed1 = p.parse(in);
            auto parsed2 = q.parse(in);
            parsed1.insert(parsed2.begin(),parsed2.end());
            return parsed1;
        }
};


template <typename T,typename S>
class MapParser : public Parser<S>{
    private:
        Parser<T> & p;
        function<S (T)> q;
    public:
        MapParser(Parser<T> & pin,function<S (T)> in):p(pin){
            q = in;
        }

        set<pair<S,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            auto parsed = p.parse(in);
            auto result = set<pair<S,vector<shared_ptr<Token>>>>();
            for(auto it1 = parsed.begin(); it1 != parsed.end();++it1){
                auto value = pair<S,vector<shared_ptr<Token>>>(q(it1->first), it1->second);
                result.insert(value);
            }
            return result;
        }
};


class IntParser : public Parser<int>{
    public:
        set<pair<int,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            if(in.size() == 0){
                return set<pair<int,vector<shared_ptr<Token>>>>();
            }
            shared_ptr<T_INT> input = dynamic_pointer_cast<T_INT>(in[0]);
            if(input != nullptr){
                vector<shared_ptr<Token>> ret(in.begin()+1,in.end());
                return set<pair<int,vector<shared_ptr<Token>>>>{pair<int,vector<shared_ptr<Token>>>(input->getInt(),ret)};
            }
            return set<pair<int,vector<shared_ptr<Token>>>>();
        }
};

class IdParser : public Parser<string>{
    public:
        set<pair<string,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            if(in.size() == 0){
                return set<pair<string,vector<shared_ptr<Token>>>>();
            }
            shared_ptr<T_ID> input = dynamic_pointer_cast<T_ID>(in[0]);
            if(input != nullptr){
                vector<shared_ptr<Token>> ret(in.begin()+1,in.end());
                return set<pair<string,vector<shared_ptr<Token>>>>{pair<string,vector<shared_ptr<Token>>>(input->getID(),ret)};
            }
            return set<pair<string,vector<shared_ptr<Token>>>>();
        }
};

//Aexp parser

shared_ptr<AExp> intToAExp(int i){
    shared_ptr<AExp> ret(new Int(i));
    return ret;
}

shared_ptr<AExp> stringToAExp(string s){
    shared_ptr<AExp> ret(new Var(s));
    return ret;
}

shared_ptr<AExp> AopFun(pair<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>> input){
    shared_ptr<T_OP> op = dynamic_pointer_cast<T_OP>(input.first.second);
    if(op == nullptr){
        throw ParsingError("Line 152 not an operator");
    }
    shared_ptr<AExp> ret(new Aop(op->getOP(),input.first.first,input.second));
    return ret;
}


class FaParser : public Parser<shared_ptr<AExp>>{
    public:
        set<pair<shared_ptr<AExp>,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            IntParser intParser;
            IdParser idParser;

            auto intMp = MapParser<int,shared_ptr<AExp>>(intParser,intToAExp);
            auto idMp =  MapParser<string,shared_ptr<AExp>>(idParser,stringToAExp);

            auto alt = AltParser<shared_ptr<AExp>>(intMp,idMp);

            return alt.parse(in);
        }
};

class TeParser : public Parser<shared_ptr<AExp>>{
    public:
        set<pair<shared_ptr<AExp>,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            FaParser faParser;
            TeParser teParser;

            TokenParser timesTok= TokenParser(shared_ptr<Token>(new T_OP("*"))); 
            TokenParser divTok= TokenParser(shared_ptr<Token>(new T_OP("/"))); 

            auto numTimes = SeqParser<shared_ptr<AExp>,shared_ptr<Token>>(faParser,timesTok);
            auto numDiv = SeqParser<shared_ptr<AExp>,shared_ptr<Token>>(faParser,divTok);

            auto timesParse = SeqParser<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>>(numTimes,teParser);
            auto divParse = SeqParser<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>>(numDiv,teParser);


            auto mpTimes = MapParser<pair<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>>,shared_ptr<AExp>>(timesParse,AopFun);
            auto mpDiv = MapParser<pair<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>>,shared_ptr<AExp>>(divParse,AopFun);

            auto alt1 = AltParser<shared_ptr<AExp>>(mpTimes,mpDiv);
            auto alt2 = AltParser<shared_ptr<AExp>>(alt1,faParser);


            return alt2.parse(in);
        }
};

class AExpParser : public  Parser<shared_ptr<AExp>>{
    public:

        set<pair<shared_ptr<AExp>,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            TeParser teParser;

            TokenParser plusTok= TokenParser(shared_ptr<Token>(new T_OP("+"))); 
            TokenParser subTok= TokenParser(shared_ptr<Token>(new T_OP("-"))); 

            auto numPlus = SeqParser<shared_ptr<AExp>,shared_ptr<Token>>(teParser,plusTok);
            auto numSub = SeqParser<shared_ptr<AExp>,shared_ptr<Token>>(teParser,subTok);

            AExpParser parse3;

            auto plusParse = SeqParser<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>>(numPlus,parse3);
            auto subParse =  SeqParser<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>>(numSub,parse3);

            auto mpPlus = MapParser<pair<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>>,shared_ptr<AExp>>(plusParse,AopFun);
            auto mpSub = MapParser<pair<pair<shared_ptr<AExp>,shared_ptr<Token>>,shared_ptr<AExp>>,shared_ptr<AExp>>(subParse,AopFun);

            auto alt1 = AltParser<shared_ptr<AExp>>(mpPlus,mpSub);
            auto alt2 = AltParser<shared_ptr<AExp>>(alt1,teParser);
            return alt2.parse(in);
           //auto se1 = SeqParser<pair<int,shared_ptr<Token>>,AExpParser>(se,parse3)
        }
};


//Stmt

shared_ptr<Stmt> assign(pair<pair<string,shared_ptr<Token>>,shared_ptr<AExp>> input){
    shared_ptr<Stmt> ret(new Assign(input.first.first,input.second));
    return ret;
}

shared_ptr<Stmt> writeVar(pair<pair<shared_ptr<Token>,shared_ptr<Token>>,pair<string,shared_ptr<Token>>> input){
    shared_ptr<Stmt> ret(new WriteVar(input.second.first));
    return ret;
}

class StmtParser : public  Parser<shared_ptr<Stmt>>{
    public:

        set<pair<shared_ptr<Stmt>,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            IdParser idParser;
            AExpParser aExpParser;

            
            TokenParser equalTok = TokenParser(shared_ptr<Token>(new T_OP("="))); 
            TokenParser writeTok = TokenParser(shared_ptr<Token>(new T_KWD("write"))); 
            TokenParser leftParTok = TokenParser(shared_ptr<Token>(new T_LPAREN())); 
            TokenParser rightParTok = TokenParser(shared_ptr<Token>(new T_RPAREN())); 
            

            auto seq1 = SeqParser<string,shared_ptr<Token>>(idParser,equalTok);
            auto seq2 = SeqParser<pair<string,shared_ptr<Token>>,shared_ptr<AExp>>(seq1,aExpParser);

            auto writeSeq = SeqParser<shared_ptr<Token>,shared_ptr<Token>>(writeTok,leftParTok);
            auto writeSeq2 = SeqParser<string,shared_ptr<Token>>(idParser,rightParTok);
            auto fullwriteSeq = SeqParser<pair<shared_ptr<Token>,shared_ptr<Token>>,pair<string,shared_ptr<Token>>>(writeSeq,writeSeq2);

            auto mpWrite = MapParser<pair<pair<shared_ptr<Token>,shared_ptr<Token>>,pair<string,shared_ptr<Token>>>,shared_ptr<Stmt>>(fullwriteSeq,writeVar);
            auto mpAssign = MapParser<pair<pair<string,shared_ptr<Token>>,shared_ptr<AExp>>,shared_ptr<Stmt>>(seq2,assign);

            auto alt = AltParser<shared_ptr<Stmt>>(mpWrite,mpAssign);

            return alt.parse(in);
           //auto se1 = SeqParser<pair<int,shared_ptr<Token>>,AExpParser>(se,parse3)
        }
};






