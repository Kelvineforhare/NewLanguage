#include <vector>
#include <set>
#include <utility> 
#include <memory>
#include <functional>
#include "syntaxTree.hpp"

using std::set;
using std::vector;
using std::pair;
using std::shared_ptr;
using std::function;

class Token;
class T_INT;
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

class TeParser : public Parser<int>{
    public:
        set<pair<int,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
            IntParser parser;
            return parser.parse(in);
        }
};

shared_ptr<AExp> test(pair<int,shared_ptr<Token>> input){
    shared_ptr<AExp> ret1(new Int(input.first));
    shared_ptr<AExp> ret(new Aop("+",ret1,ret1));
    return ret;
}

class AExpParser : public  MapParser<pair<int,shared_ptr<Token>>,shared_ptr<AExp>>{
    private:
        TeParser parse1;
        TokenParser parse2= TokenParser(shared_ptr<Token>(new T_OP("+"))); 
        SeqParser<int,shared_ptr<Token>> se = SeqParser<int,shared_ptr<Token>>(parse1,parse2);
        // shared_ptr<AExpParser> parse3;
    public:

        AExpParser(): MapParser(se,test){
        }


        set<pair<shared_ptr<AExp>,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override{
           //AExpParser parse3;
           
           auto mp = MapParser<pair<int,shared_ptr<Token>>,shared_ptr<AExp>>(se,test);
           return mp.parse(in);
           //auto se1 = SeqParser<pair<int,shared_ptr<Token>>,AExpParser>(se,parse3)
        }
};





