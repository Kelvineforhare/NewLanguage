#include <exception>

class LexingError : public std::exception {
    public:
        const char *what(){
            return "Lexing Error";
        }
};