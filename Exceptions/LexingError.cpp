#include <exception>

class LexingError : public std::exception
{
    protected:
        string msg;
    public:
        LexingError(const char *m) : msg(m){};
        LexingError(string m) {
            msg = m;
        };
        const char *what(){
            string message = string("Lexing Error: " +  msg);
            cout << message.c_str() << "\n";
            const char * ret =  message.c_str();
            return ret;
        }
};