#include <string>



class Token{
    friend bool operator==(shared_ptr<Token> a,shared_ptr<Token> b){
        if(typeid(*a) == typeid(*b) && a->toString() == b->toString()){
            return true;
        }
        return false;
    }
    public:
        virtual std::string toString() = 0;
};

class T_SEMI : public Token{
    std::string toString() override{
        return "; ";
    }
};

class T_LPAREN : public Token{
    std::string toString()override{
        return "( ";
    }
};

class T_RPAREN : public Token{
    std::string toString()override{
        return ") ";
    }
};

class T_LBRACK : public Token{
    std::string toString()override{
        return "{ ";
    }
};

class T_RBRACK : public Token{
    std::string toString()override{
        return "} ";
    }
};

class T_ID :public Token{
    private:
        std::string s;
    public:
        T_ID(std::string in){
            s = in;
        }

        std::string getID(){
            return s;
        }
        std::string toString()override{
            return s + " ";
        }
};

class T_OP :public Token{
    private:
        std::string s;
    public:
        T_OP(std::string in){
            s = in;
        }
        std::string getOP(){
            return s;
        }
        std::string toString()override{
            return s + " ";
        }
};

class T_INT :public Token{
    private:
        int i;
    public:
        T_INT(int in){
            i = in;
        }
        int getInt(){
            return i;
        }
        std::string toString()override{
            return i + " ";
        }
};

class T_KWD :public Token{
    private:
        std::string s;
    public:
        T_KWD(std::string in){
            s = in;
        }
        std::string getKWD(){
            return s;
        }
        std::string toString()override{
            return s + " ";
        }
};

class T_STR :public Token{
    private:
        std::string s;
    public:
        T_STR(std::string in){
            s = in;
        }
        std::string getStr(){
            return s;
        }
        std::string toString()override{
            return "'"+ s + "' ";
        }
};