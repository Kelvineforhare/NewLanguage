#include <string>


class Token{
    
};

class T_SEMI : public Token{

};

class T_LPAREN : public Token{

};

class T_RPAREN : public Token{

};

class T_LBRACK : public Token{

};

class T_RBRACK : public Token{

};

class T_ID :public Token{
    private:
        std::string s;
    public:
        T_ID(string in){
            s = in;
        }

        std::string getID(){
            return s;
        }
};

class T_OP :public Token{
    private:
        std::string s;
    public:
        T_OP(string in){
            s = in;
        }
        std::string getOP(){
            return s;
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
};

class T_KWD :public Token{
    private:
        std::string s;
    public:
        T_KWD(string in){
            s = in;
        }
        std::string getKWD(){
            return s;
        }
};

class T_STR :public Token{
    private:
        std::string s;
    public:
        T_STR(string in){
            s = in;
        }
        std::string getStr(){
            return s;
        }
};