#include <exception>
#include <string>
#include <iostream>

using std::string;
using std::cout;

class RunTimeError : public std::exception
{
    protected:
        string msg;
    public:
        RunTimeError(const char *m) : msg(m){};
        RunTimeError(string m) {
            msg = m;
        };
        const char *what(){
            string message = string("Runtime Error: " +  msg);
            cout << message.c_str() << "\n";
            const char * ret =  message.c_str();
            return ret;
        }
};