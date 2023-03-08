#include <exception>
#include <string>
#include <iostream>

using std::string;
using std::cout;

class ParsingError : public std::exception
{
    protected:
        string msg;
    public:
        ParsingError(const char *m) : msg(m){};
        ParsingError(string m) {
            msg = m;
        };
        const char *what(){
            string message = string("Parsing Error: " +  msg);
            cout << message.c_str() << "\n";
            const char * ret =  message.c_str();
            return ret;
        }
};