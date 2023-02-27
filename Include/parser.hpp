#include <vector>
#include <set>
#include <utility> 
#include <memory>
#include <functional>

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





