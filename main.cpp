#include "Include/regex.hpp"
#include "Include/value.hpp"
#include "Include/token.hpp"
#include "Include/parser.hpp"
#include <sstream>
//#include <string>

// string getString(string & str)
// {
//     std::ifstream ifs(str.c_str());
//     std::ostringstream sstr;
//     sstr << ifs.rdbuf();
//     string line;
//     return "test";
// }

int main(int argc, char *argv[])
{
    if (argc > 1)
    {

        string t1;

        string t2(argv[1]);

        cout << "t1 :\n"
             << t1 << std::endl;

        cout << "t2 :\n"
             << t2 << std::endl;
    }
    // if and else required
    // semi colons after if else stantments and while statements
    // semi colons after all stantment
    // only int is the type
    string fib = "i = 0;"
                 "print(i);";
    auto input = getTokensFromLang(fib);
    Stmts parser;
    try
    {
        auto output = parser.parse_all(input);
        auto it = output.begin();

        if (it != output.end())
        {
            map<string, int> env;
            auto test = (*it);
            cout << test << "\n";
        }
    }
    catch (RunTimeError &e)
    {
        cout << e.what() << "\n";
        exit(0);
    }

    return 0;
}
