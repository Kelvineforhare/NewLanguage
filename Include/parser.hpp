#pragma once

#include <vector>
#include <set>
#include <utility>
#include <memory>
#include <functional>
#include "syntaxTree.hpp"
#include "../Exceptions/ParsingError.cpp"

using std::function;
using std::pair;
using std::set;
using std::shared_ptr;
using std::vector;

class Token;
bool operator==(shared_ptr<Token> a, shared_ptr<Token> b);

template <typename T>
class Parser
{
public:
    virtual set<pair<T, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) = 0;

    set<T> parse_all(vector<shared_ptr<Token>> in)
    {
        auto parsed = parse(in);
        set<T> ret;
        for (auto par : parsed)
        {
            if (par.second.size() == 0)
            {
                ret.insert(par.first);
            }
        }
        return ret;
    }
};

class TokenParser : public Parser<shared_ptr<Token>>
{
private:
    shared_ptr<Token> tok;

public:
    TokenParser(shared_ptr<Token> input)
    {
        tok = input;
    }
    set<pair<shared_ptr<Token>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        if (in.size() == 0)
        {
            return set<pair<shared_ptr<Token>, vector<shared_ptr<Token>>>>();
        }
        if (in[0] == tok)
        {
            set<pair<shared_ptr<Token>, vector<shared_ptr<Token>>>> ret;
            pair<shared_ptr<Token>, vector<shared_ptr<Token>>> pair;
            pair.first = tok;
            pair.second = vector<shared_ptr<Token>>(in.begin() + 1, in.end());
            ret.insert(pair);
            return ret;
        }
        return set<pair<shared_ptr<Token>, vector<shared_ptr<Token>>>>();
    }
};

template <typename T, typename S>
class SeqParser : public Parser<pair<T, S>>
{
private:
    Parser<T> &p;
    Parser<S> &q;

public:
    SeqParser(Parser<T> &pin, Parser<S> &qin) : p(pin), q(qin) {}

    set<pair<pair<T, S>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        auto parsed1 = p.parse(in);

        // set<pair<pair<T,S>,vector<shared_ptr<Token>>>> ret;
        // ret.insert(pair<pair<T,S>,vector<shared_ptr<Token>>>(pair<T,S>(parsed1.first,parsed2.first),parsed2.second));
        // return ret;
        auto result = set<pair<pair<T, S>, vector<shared_ptr<Token>>>>();
        for (auto it1 = parsed1.begin(); it1 != parsed1.end(); ++it1)
        {
            auto parsed2 = q.parse(it1->second);
            for (auto it2 = parsed2.begin(); it2 != parsed2.end(); ++it2)
            {
                auto value = pair<pair<T, S>, vector<shared_ptr<Token>>>(pair<T, S>(it1->first, it2->first), it2->second);
                result.insert(value);
            }
        }
        return result;
    }
};

template <typename T>
class AltParser : public Parser<T>
{

private:
    Parser<T> &p;
    Parser<T> &q;

public:
    AltParser<T> operator=(AltParser<T> &altParser)
    {
        p = altParser.p;
        q = altParser.q;
        return *this;
    }

    AltParser(Parser<T> &pin, Parser<T> &qin) : p(pin), q(qin) {}

    AltParser(const AltParser<T> &ap) : p(ap.p), q(ap.q) {}

    set<pair<T, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        auto parsed1 = p.parse(in);
        auto parsed2 = q.parse(in);
        parsed1.insert(parsed2.begin(), parsed2.end());
        return parsed1;
    }
};

template <typename T, typename S>
class MapParser : public Parser<S>
{
private:
    Parser<T> &p;
    function<S(T)> q;

public:
    MapParser(Parser<T> &pin, function<S(T)> in) : p(pin)
    {
        q = in;
    }

    set<pair<S, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        auto parsed = p.parse(in);
        auto result = set<pair<S, vector<shared_ptr<Token>>>>();
        for (auto it1 = parsed.begin(); it1 != parsed.end(); ++it1)
        {
            auto value = pair<S, vector<shared_ptr<Token>>>(q(it1->first), it1->second);
            result.insert(value);
        }
        return result;
    }
};

template <typename T, typename S>
class ListParser : public Parser<vector<T>>
{
private:
    Parser<T> &p;
    Parser<S> &q;

public:
    ListParser(Parser<T> &pin, Parser<S> &qin) : p(pin), q(qin) {}

    static vector<T> listParse(pair<pair<T, S>, vector<T>> input)
    {
        vector<T> vec;
        vec.push_back(input.first.first);
        vec.insert(vec.end(), input.second.begin(), input.second.end());
        return vec;
    }

    static vector<T> singleParse(T input)
    {
        vector<T> vec;
        vec.push_back(input);
        return vec;
    }

    set<pair<vector<T>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        ListParser listParser = ListParser<T, S>(p, q);
        auto seq = SeqParser<T, S>(p, q);
        auto seq2 = SeqParser<pair<T, S>, vector<T>>(seq, listParser);

        auto map = MapParser<pair<pair<T, S>, vector<T>>, vector<T>>(seq2, listParse);
        auto map2 = MapParser<T, vector<T>>(p, singleParse);

        auto alt = AltParser<vector<T>>(map, map2);

        return alt.parse(in);
    }
};

class IntParser : public Parser<int>
{
public:
    set<pair<int, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        if (in.size() == 0)
        {
            return set<pair<int, vector<shared_ptr<Token>>>>();
        }
        shared_ptr<T_INT> input = dynamic_pointer_cast<T_INT>(in[0]);
        if (input != nullptr)
        {
            vector<shared_ptr<Token>> ret(in.begin() + 1, in.end());
            return set<pair<int, vector<shared_ptr<Token>>>>{pair<int, vector<shared_ptr<Token>>>(input->getInt(), ret)};
        }
        return set<pair<int, vector<shared_ptr<Token>>>>();
    }
};

class IdParser : public Parser<string>
{
public:
    set<pair<string, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        if (in.size() == 0)
        {
            return set<pair<string, vector<shared_ptr<Token>>>>();
        }
        shared_ptr<T_ID> input = dynamic_pointer_cast<T_ID>(in[0]);
        if (input != nullptr)
        {
            vector<shared_ptr<Token>> ret(in.begin() + 1, in.end());
            return set<pair<string, vector<shared_ptr<Token>>>>{pair<string, vector<shared_ptr<Token>>>(input->getID(), ret)};
        }
        return set<pair<string, vector<shared_ptr<Token>>>>();
    }
};

shared_ptr<AExp> intToAExp(int i)
{
    shared_ptr<AExp> ret(new Int(i));
    return ret;
}

shared_ptr<AExp> stringToAExp(string s)
{
    shared_ptr<AExp> ret(new Var(s));
    return ret;
}

shared_ptr<AExp> AopFun(pair<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>> input)
{
    shared_ptr<T_OP> op = dynamic_pointer_cast<T_OP>(input.first.second);
    if (op == nullptr)
    {
        throw ParsingError("Line 152 not an operator");
    }
    shared_ptr<AExp> ret(new Aop(op->getOP(), input.first.first, input.second));
    return ret;
}

shared_ptr<AExp> brackExp(pair<pair<shared_ptr<Token>, shared_ptr<AExp>>, shared_ptr<Token>> input)
{
    return input.first.second;
}

class AExpParser : public Parser<shared_ptr<AExp>>
{
public:
    set<pair<shared_ptr<AExp>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override;
};

class FaParser : public Parser<shared_ptr<AExp>>
{
public:
    set<pair<shared_ptr<AExp>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        IntParser intParser;
        IdParser idParser;
        AExpParser aExpParser;

        TokenParser leftParTok = TokenParser(shared_ptr<Token>(new T_LPAREN()));
        TokenParser rightParTok = TokenParser(shared_ptr<Token>(new T_RPAREN()));

        auto seq = SeqParser<shared_ptr<Token>, shared_ptr<AExp>>(leftParTok, aExpParser);
        auto seq2 = SeqParser<pair<shared_ptr<Token>, shared_ptr<AExp>>, shared_ptr<Token>>(seq, rightParTok);

        auto intMp = MapParser<int, shared_ptr<AExp>>(intParser, intToAExp);
        auto idMp = MapParser<string, shared_ptr<AExp>>(idParser, stringToAExp);
        auto expMp = MapParser<pair<pair<shared_ptr<Token>, shared_ptr<AExp>>, shared_ptr<Token>>, shared_ptr<AExp>>(seq2, brackExp);

        auto alt = AltParser<shared_ptr<AExp>>(intMp, idMp);
        auto alt2 = AltParser<shared_ptr<AExp>>(alt, expMp);

        return alt2.parse(in);
    }
};

class TeParser : public Parser<shared_ptr<AExp>>
{
public:
    set<pair<shared_ptr<AExp>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        FaParser faParser;
        TeParser teParser;

        TokenParser timesTok = TokenParser(shared_ptr<Token>(new T_OP("*")));
        TokenParser divTok = TokenParser(shared_ptr<Token>(new T_OP("/")));

        auto numTimes = SeqParser<shared_ptr<AExp>, shared_ptr<Token>>(faParser, timesTok);
        auto numDiv = SeqParser<shared_ptr<AExp>, shared_ptr<Token>>(faParser, divTok);

        auto timesParse = SeqParser<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>(numTimes, teParser);
        auto divParse = SeqParser<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>(numDiv, teParser);

        auto mpTimes = MapParser<pair<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>, shared_ptr<AExp>>(timesParse, AopFun);
        auto mpDiv = MapParser<pair<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>, shared_ptr<AExp>>(divParse, AopFun);

        auto alt1 = AltParser<shared_ptr<AExp>>(mpTimes, mpDiv);
        auto alt2 = AltParser<shared_ptr<AExp>>(alt1, faParser);

        return alt2.parse(in);
    }
};

set<pair<shared_ptr<AExp>, vector<shared_ptr<Token>>>> AExpParser::parse(vector<shared_ptr<Token>> in)
{
    TeParser teParser;

    TokenParser plusTok = TokenParser(shared_ptr<Token>(new T_OP("+")));
    TokenParser subTok = TokenParser(shared_ptr<Token>(new T_OP("-")));

    auto numPlus = SeqParser<shared_ptr<AExp>, shared_ptr<Token>>(teParser, plusTok);
    auto numSub = SeqParser<shared_ptr<AExp>, shared_ptr<Token>>(teParser, subTok);

    AExpParser parse3;

    auto plusParse = SeqParser<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>(numPlus, parse3);
    auto subParse = SeqParser<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>(numSub, parse3);

    auto mpPlus = MapParser<pair<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>, shared_ptr<AExp>>(plusParse, AopFun);
    auto mpSub = MapParser<pair<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>, shared_ptr<AExp>>(subParse, AopFun);

    auto alt1 = AltParser<shared_ptr<AExp>>(mpPlus, mpSub);
    auto alt2 = AltParser<shared_ptr<AExp>>(alt1, teParser);
    return alt2.parse(in);
    // auto se1 = SeqParser<pair<int,shared_ptr<Token>>,AExpParser>(se,parse3)
}

shared_ptr<BExp> makeToBexp(pair<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>> input)
{
    shared_ptr<T_OP> op = dynamic_pointer_cast<T_OP>(input.first.second);
    if (op == nullptr)
    {
        throw RunTimeError("Line 330 not an operator");
    }
    shared_ptr<BExp> ret(new Bop(op->getOP(), input.first.first, input.second));
    return ret;
}

shared_ptr<BExp> trueBexp(shared_ptr<Token> input)
{
    shared_ptr<BExp> ret(new True());
    return ret;
}

shared_ptr<BExp> falseBexp(shared_ptr<Token> input)
{
    shared_ptr<BExp> ret(new False());
    return ret;
}

shared_ptr<BExp> getBoolFromBrack(pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<Token>> input)
{
    return input.first.second;
}

class BExpParser : public Parser<shared_ptr<BExp>>
{
    set<pair<shared_ptr<BExp>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override;
};

class Bl : public Parser<shared_ptr<BExp>>
{
    set<pair<shared_ptr<BExp>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        AExpParser aExpParser;
        vector<TokenParser> toks;
        BExpParser bexp;

        TokenParser leftParTok = TokenParser(shared_ptr<Token>(new T_LPAREN()));
        TokenParser rightParTok = TokenParser(shared_ptr<Token>(new T_RPAREN()));
        TokenParser trueTok = shared_ptr<Token>(new T_KWD("true"));
        TokenParser falseTok = shared_ptr<Token>(new T_KWD("false"));

        auto seq = SeqParser<shared_ptr<Token>, shared_ptr<BExp>>(leftParTok, bexp);
        auto seq2 = SeqParser<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<Token>>(seq, rightParTok);

        auto trueMp = MapParser<shared_ptr<Token>, shared_ptr<BExp>>(trueTok, trueBexp);
        auto falseMp = MapParser<shared_ptr<Token>, shared_ptr<BExp>>(falseTok, falseBexp);
        auto brackBool = MapParser<pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<Token>>, shared_ptr<BExp>>(seq2, getBoolFromBrack);

        toks.push_back(TokenParser(shared_ptr<Token>(new T_OP("=="))));
        toks.push_back(TokenParser(shared_ptr<Token>(new T_OP("!="))));
        toks.push_back(TokenParser(shared_ptr<Token>(new T_OP("<"))));
        toks.push_back(TokenParser(shared_ptr<Token>(new T_OP(">"))));
        toks.push_back(TokenParser(shared_ptr<Token>(new T_OP("<="))));
        toks.push_back(TokenParser(shared_ptr<Token>(new T_OP(">="))));

        vector<SeqParser<shared_ptr<AExp>, shared_ptr<Token>>> seqP;

        for (int i = 0; i < toks.size(); ++i)
        {
            seqP.push_back(SeqParser<shared_ptr<AExp>, shared_ptr<Token>>(aExpParser, toks[i]));
        }

        vector<SeqParser<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>> fullSeq;
        for (int i = 0; i < seqP.size(); ++i)
        {
            fullSeq.push_back(SeqParser<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>(seqP[i], aExpParser));
        }

        vector<MapParser<pair<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>, shared_ptr<BExp>>> seqMp;
        for (int i = 0; i < fullSeq.size(); ++i)
        {
            seqMp.push_back(MapParser<pair<pair<shared_ptr<AExp>, shared_ptr<Token>>, shared_ptr<AExp>>, shared_ptr<BExp>>(fullSeq[i], makeToBexp));
        }

        AltParser<shared_ptr<BExp>> alt = AltParser<shared_ptr<BExp>>(seqMp[0], seqMp[1]);
        auto newAlt = AltParser<shared_ptr<BExp>>(alt, seqMp[2]);
        auto newAlt2 = AltParser<shared_ptr<BExp>>(newAlt, seqMp[3]);
        auto newAlt3 = AltParser<shared_ptr<BExp>>(newAlt2, seqMp[4]);
        auto newAlt4 = AltParser<shared_ptr<BExp>>(newAlt3, seqMp[5]);
        auto newAlt5 = AltParser<shared_ptr<BExp>>(newAlt4, trueMp);
        auto newAlt6 = AltParser<shared_ptr<BExp>>(newAlt5, falseMp);
        auto newAlt7 = AltParser<shared_ptr<BExp>>(newAlt6, brackBool);

        return newAlt7.parse(in);
    }
};

// shared_ptr<BExp> andBexp(pair<pair<shared_ptr<BExp>,shared_ptr<Token>>,shared_ptr<BExp>> input){
//     shared_ptr<BExp> ret(new And(input.first.first,input.second));
//     return ret;
// }

// shared_ptr<BExp> orBexp(pair<pair<shared_ptr<BExp>,shared_ptr<Token>>,shared_ptr<BExp>> input){
//     shared_ptr<BExp> ret(new Or(input.first.first,input.second));
//     return ret;
// }

shared_ptr<BExp> getAndBexp(pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<BExp>> input)
{
    shared_ptr<BExp> ret(new And(input.first.second, input.second));
    return ret;
}

shared_ptr<BExp> getSingleBexp(pair<shared_ptr<Token>, shared_ptr<BExp>> input)
{
    return input.second;
}

// TermTail
class TermTail : public Parser<shared_ptr<BExp>>
{
    set<pair<shared_ptr<BExp>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        TokenParser andTok = shared_ptr<Token>(new T_OP("&&"));
        Bl bl;
        TermTail termTail;

        auto seq = SeqParser<shared_ptr<Token>, shared_ptr<BExp>>(andTok, bl);
        auto seq2 = SeqParser<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<BExp>>(seq, termTail);

        auto map = MapParser<pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<BExp>>, shared_ptr<BExp>>(seq2, getAndBexp);
        auto map2 = MapParser<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<BExp>>(seq, getSingleBexp);

        auto alt = AltParser<shared_ptr<BExp>>(map, map2);
        return alt.parse(in);
    }
};

shared_ptr<BExp> andToBexp(pair<shared_ptr<BExp>, shared_ptr<BExp>> input)
{
    shared_ptr<BExp> ret(new And(input.first, input.second));
    return ret;
}

class Term : public Parser<shared_ptr<BExp>>
{
    set<pair<shared_ptr<BExp>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        Bl bl;
        TermTail bExpParser;
        auto andSeq = SeqParser<shared_ptr<BExp>, shared_ptr<BExp>>(bl, bExpParser);
        auto andMp = MapParser<pair<shared_ptr<BExp>, shared_ptr<BExp>>, shared_ptr<BExp>>(andSeq, andToBexp);

        auto alt = AltParser<shared_ptr<BExp>>(bl, andMp);
        return alt.parse(in);
    }
};

shared_ptr<BExp> getOrBexp(pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<BExp>> input)
{
    shared_ptr<BExp> ret(new Or(input.first.second, input.second));
    return ret;
}

class BoolOr : public Parser<shared_ptr<BExp>>
{
    set<pair<shared_ptr<BExp>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        TokenParser orTok = shared_ptr<Token>(new T_OP("||"));
        Term term;
        BoolOr boolOr;

        auto seq = SeqParser<shared_ptr<Token>, shared_ptr<BExp>>(orTok, term);
        auto seq2 = SeqParser<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<BExp>>(seq, boolOr);

        auto orSeq = SeqParser<shared_ptr<Token>, shared_ptr<BExp>>(orTok, term);

        auto map = MapParser<pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<BExp>>, shared_ptr<BExp>>(seq2, getOrBexp);
        auto map2 = MapParser<pair<shared_ptr<Token>, shared_ptr<BExp>>, shared_ptr<BExp>>(seq, getSingleBexp);

        auto alt = AltParser<shared_ptr<BExp>>(map, map2);
        return alt.parse(in);
    }
};

shared_ptr<BExp> orToBexp(pair<shared_ptr<BExp>, shared_ptr<BExp>> input)
{
    shared_ptr<BExp> ret(new Or(input.first, input.second));
    return ret;
}

set<pair<shared_ptr<BExp>, vector<shared_ptr<Token>>>> BExpParser::parse(vector<shared_ptr<Token>> in)
{
    Term term;
    BoolOr boolOr;

    auto seq = SeqParser<shared_ptr<BExp>, shared_ptr<BExp>>(term, boolOr);
    auto map = MapParser<pair<shared_ptr<BExp>, shared_ptr<BExp>>, shared_ptr<BExp>>(seq, orToBexp);

    auto alt = AltParser<shared_ptr<BExp>>(map, term);

    return alt.parse(in);
}

shared_ptr<Stmt> assign(pair<pair<string, shared_ptr<Token>>, shared_ptr<AExp>> input)
{
    shared_ptr<Stmt> ret(new Assign(input.first.first, input.second));
    return ret;
}

shared_ptr<Stmt> writeVar(pair<pair<shared_ptr<Token>, shared_ptr<Token>>, pair<string, shared_ptr<Token>>> input)
{
    shared_ptr<Stmt> ret(new WriteVar(input.second.first));
    return ret;
}

shared_ptr<Stmt> ifStmt(pair<pair<pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, vector<shared_ptr<Stmt>>>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>> input)
{
    shared_ptr<Stmt> ret(new If(input.first.first.first.second, input.first.first.second, input.second));
    return ret;
}

shared_ptr<Stmt> whileStmt(pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, vector<shared_ptr<Stmt>>> input)
{
    shared_ptr<Stmt> ret(new While(input.first.second, input.second));
    return ret;
}

shared_ptr<Stmt> callStmt(pair<pair<pair<string,shared_ptr<Token>>,vector<string>>,shared_ptr<Token>> input)
{
    shared_ptr<Stmt> ret(new Call(input.first.first.first,input.first.second));
    return ret;
}


class BlockParser : public Parser<vector<shared_ptr<Stmt>>>
{
    set<pair<vector<shared_ptr<Stmt>>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override;
};

class StmtParser : public Parser<shared_ptr<Stmt>>
{
public:
    set<pair<shared_ptr<Stmt>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        IdParser idParser;
        AExpParser aExpParser;
        BExpParser bExpParser;
        BlockParser blockParser;

        TokenParser equalTok = TokenParser(shared_ptr<Token>(new T_OP("=")));
        TokenParser writeTok = TokenParser(shared_ptr<Token>(new T_KWD("print")));
        TokenParser ifTok = TokenParser(shared_ptr<Token>(new T_KWD("if")));
        TokenParser elseTok = TokenParser(shared_ptr<Token>(new T_KWD("else")));
        TokenParser whileTok = TokenParser(shared_ptr<Token>(new T_KWD("while")));
        TokenParser leftParTok = TokenParser(shared_ptr<Token>(new T_LPAREN()));
        TokenParser rightParTok = TokenParser(shared_ptr<Token>(new T_RPAREN()));
        TokenParser comTok = TokenParser(shared_ptr<Token>(new T_COMMA()));

        auto seq1 = SeqParser<string, shared_ptr<Token>>(idParser, equalTok);
        auto seq2 = SeqParser<pair<string, shared_ptr<Token>>, shared_ptr<AExp>>(seq1, aExpParser);

        auto writeSeq = SeqParser<shared_ptr<Token>, shared_ptr<Token>>(writeTok, leftParTok);
        auto writeSeq2 = SeqParser<string, shared_ptr<Token>>(idParser, rightParTok);
        auto fullwriteSeq = SeqParser<pair<shared_ptr<Token>, shared_ptr<Token>>, pair<string, shared_ptr<Token>>>(writeSeq, writeSeq2);

        auto ifSeq = SeqParser<shared_ptr<Token>, shared_ptr<BExp>>(ifTok, bExpParser);
        auto ifSeq3 = SeqParser<pair<shared_ptr<Token>, shared_ptr<BExp>>, vector<shared_ptr<Stmt>>>(ifSeq, blockParser);
        auto ifSeq4 = SeqParser<pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, vector<shared_ptr<Stmt>>>, shared_ptr<Token>>(ifSeq3, elseTok);
        auto ifSeq5 = SeqParser<pair<pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, vector<shared_ptr<Stmt>>>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>>(ifSeq4, blockParser);

        auto whileSeq = SeqParser<shared_ptr<Token>, shared_ptr<BExp>>(whileTok, bExpParser);
        auto whileSeq2 = SeqParser<pair<shared_ptr<Token>, shared_ptr<BExp>>, vector<shared_ptr<Stmt>>>(whileSeq, blockParser);

        auto list = ListParser<string, shared_ptr<Token>>(idParser, comTok);
        auto funcCallSeq = SeqParser<string,shared_ptr<Token>>(idParser,leftParTok);
        auto funcCallSeq2 = SeqParser<pair<string,shared_ptr<Token>>,vector<string>>(funcCallSeq,list);
        auto funcCallSeq3 = SeqParser<pair<pair<string,shared_ptr<Token>>,vector<string>>,shared_ptr<Token>>(funcCallSeq2,rightParTok);

        auto mpWrite = MapParser<pair<pair<shared_ptr<Token>, shared_ptr<Token>>, pair<string, shared_ptr<Token>>>, shared_ptr<Stmt>>(fullwriteSeq, writeVar);
        auto mpAssign = MapParser<pair<pair<string, shared_ptr<Token>>, shared_ptr<AExp>>, shared_ptr<Stmt>>(seq2, assign);
        auto mpIf = MapParser<pair<pair<pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, vector<shared_ptr<Stmt>>>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>>, shared_ptr<Stmt>>(ifSeq5, ifStmt);
        auto mpWhile = MapParser<pair<pair<shared_ptr<Token>, shared_ptr<BExp>>, vector<shared_ptr<Stmt>>>, shared_ptr<Stmt>>(whileSeq2, whileStmt);
        auto mpCall = MapParser<pair<pair<pair<string,shared_ptr<Token>>,vector<string>>,shared_ptr<Token>>, shared_ptr<Stmt>>(funcCallSeq3,callStmt);

        auto alt = AltParser<shared_ptr<Stmt>>(mpWrite, mpAssign);
        auto alt2 = AltParser<shared_ptr<Stmt>>(alt, mpIf);
        auto alt3 = AltParser<shared_ptr<Stmt>>(alt2, mpWhile);
        auto alt4 = AltParser<shared_ptr<Stmt>>(alt3,mpCall);

        return alt4.parse(in);
    }
};

vector<shared_ptr<Stmt>> makeToList(pair<pair<shared_ptr<Stmt>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>> input)
{
    vector<shared_ptr<Stmt>> ret(input.second);
    ret.insert(ret.begin(), input.first.first);
    return ret;
}

vector<shared_ptr<Stmt>> makeToSingleToList(pair<shared_ptr<Stmt>, shared_ptr<Token>> input)
{
    vector<shared_ptr<Stmt>> ret{input.first};
    return ret;
}

class Stmts : public Parser<vector<shared_ptr<Stmt>>>
{
public:
    set<pair<vector<shared_ptr<Stmt>>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        StmtParser stmtParser;
        TokenParser semiTok = TokenParser(shared_ptr<Token>(new T_SEMI()));
        Stmts stmts;

        auto seq1 = SeqParser<shared_ptr<Stmt>, shared_ptr<Token>>(stmtParser, semiTok);
        auto seq2 = SeqParser<pair<shared_ptr<Stmt>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>>(seq1, stmts);

        auto map = MapParser<pair<pair<shared_ptr<Stmt>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>>, vector<shared_ptr<Stmt>>>(seq2, makeToList);
        auto map2 = MapParser<pair<shared_ptr<Stmt>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>>(seq1, makeToSingleToList);

        auto alt = AltParser<vector<shared_ptr<Stmt>>>(map, map2);

        return alt.parse(in);
    }
};

vector<shared_ptr<Stmt>> getStmts(pair<pair<shared_ptr<Token>, vector<shared_ptr<Stmt>>>, shared_ptr<Token>> input)
{
    return input.first.second;
}

set<pair<vector<shared_ptr<Stmt>>, vector<shared_ptr<Token>>>> BlockParser::parse(vector<shared_ptr<Token>> in)
{
    TokenParser leftBrTok = TokenParser(shared_ptr<Token>(new T_LBRACK()));
    TokenParser rightBrTok = TokenParser(shared_ptr<Token>(new T_RBRACK()));
    Stmts stmts;

    auto seq = SeqParser<shared_ptr<Token>, vector<shared_ptr<Stmt>>>(leftBrTok, stmts);
    auto seq2 = SeqParser<pair<shared_ptr<Token>, vector<shared_ptr<Stmt>>>, shared_ptr<Token>>(seq, rightBrTok);

    auto map = MapParser<pair<pair<shared_ptr<Token>, vector<shared_ptr<Stmt>>>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>>(seq2, getStmts);

    return map.parse(in);
}

shared_ptr<Decl> createFunc(pair<pair<pair<pair<pair<shared_ptr<Token>, string>, shared_ptr<Token>>, vector<string>>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>> input)
{
    auto test = input.first.first.second;
    shared_ptr<Decl> ret(new Def(input.first.first.first.first.second, input.first.first.second, input.second));
    return ret;
}

class Definition : public Parser<shared_ptr<Decl>>
{
public:
    set<pair<shared_ptr<Decl>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
        IdParser idParser;
        BlockParser blockParser;
        TokenParser defTok = TokenParser(shared_ptr<Token>(new T_KWD("def")));
        TokenParser comTok = TokenParser(shared_ptr<Token>(new T_COMMA()));
        TokenParser leftParTok = TokenParser(shared_ptr<Token>(new T_LPAREN()));
        TokenParser rightParTok = TokenParser(shared_ptr<Token>(new T_RPAREN()));

        auto list = ListParser<string, shared_ptr<Token>>(idParser, comTok);

        auto seq = SeqParser<shared_ptr<Token>, string>(defTok, idParser);
        auto seq2 = SeqParser<pair<shared_ptr<Token>, string>, shared_ptr<Token>>(seq, leftParTok);
        auto seq3 = SeqParser<pair<pair<shared_ptr<Token>, string>, shared_ptr<Token>>, vector<string>>(seq2, list);
        auto seq4 = SeqParser<pair<pair<pair<shared_ptr<Token>, string>, shared_ptr<Token>>, vector<string>>, shared_ptr<Token>>(seq3, rightParTok);
        auto seq5 = SeqParser<pair<pair<pair<pair<shared_ptr<Token>, string>, shared_ptr<Token>>, vector<string>>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>>(seq4, blockParser);

        auto map = MapParser<pair<pair<pair<pair<pair<shared_ptr<Token>, string>, shared_ptr<Token>>, vector<string>>, shared_ptr<Token>>, vector<shared_ptr<Stmt>>>, shared_ptr<Decl>>(seq5, createFunc);

        return map.parse(in);
    }
};

vector<shared_ptr<Decl>> listDecl(pair<shared_ptr<Decl>,vector<shared_ptr<Decl>>> input)
{
    input.second.insert(input.second.begin(),input.first);
    return input.second;
}

vector<shared_ptr<Decl>> makeMain(vector<shared_ptr<Stmt>> input)
{
    shared_ptr<Decl> dec(new Main(input));
    vector<shared_ptr<Decl>> ret{dec};
    return ret;
}


class Program : public Parser<vector<shared_ptr<Decl>>>
{
public:
    set<pair<vector<shared_ptr<Decl>>, vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in) override
    {
       Definition def;
       Program prog;
       Stmts stmts;
       
       auto seq = SeqParser<shared_ptr<Decl>,vector<shared_ptr<Decl>>>(def,prog);

       auto map = MapParser<pair<shared_ptr<Decl>,vector<shared_ptr<Decl>>>,vector<shared_ptr<Decl>>>(seq,listDecl);
       auto map2 = MapParser<vector<shared_ptr<Stmt>>,vector<shared_ptr<Decl>>>(stmts,makeMain);

       auto alt = AltParser<vector<shared_ptr<Decl>>>(map,map2);
    
       return alt.parse(in);
    }
};

// class SimpleBExpParser: public Parser<shared_ptr<BExp>>{
//     set<pair<shared_ptr<BExp>,vector<shared_ptr<Token>>>> parse(vector<shared_ptr<Token>> in)override{
//         Bl bl;
//         TokenParser andTok =shared_ptr<Token>(new T_OP("&&"));
//         TokenParser orTok =shared_ptr<Token>(new T_OP("||"));

//         TokenParser trueTok =shared_ptr<Token>(new T_KWD("true"));
//         TokenParser falseTok = shared_ptr<Token>(new T_KWD("false"));

//         auto andSeq = SeqParser<shared_ptr<BExp>,shared_ptr<Token>>(bl,andTok);
//         auto orSeq = SeqParser<shared_ptr<BExp>,shared_ptr<Token>>(bl,orTok);

//         auto fullAndSeq = SeqParser<pair<shared_ptr<BExp>,shared_ptr<Token>>,shared_ptr<BExp>>(andSeq,bl);
//         auto fullOrSeq = SeqParser<pair<shared_ptr<BExp>,shared_ptr<Token>>,shared_ptr<BExp>>(orSeq,bl);

//         auto andMp = MapParser<pair<pair<shared_ptr<BExp>,shared_ptr<Token>>,shared_ptr<BExp>>,shared_ptr<BExp>>(fullAndSeq,andBexp);
//         auto orMp = MapParser<pair<pair<shared_ptr<BExp>,shared_ptr<Token>>,shared_ptr<BExp>>,shared_ptr<BExp>>(fullOrSeq,orBexp);

//         auto alt = AltParser<shared_ptr<BExp>>(andMp,orMp);
//         auto fullAlt = AltParser<shared_ptr<BExp>>(alt,bl);

//         return fullAlt.parse(in);
//     }
// };
