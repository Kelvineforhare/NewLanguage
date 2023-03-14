#include "Include/regex.hpp"
#include "Include/value.hpp"
#include "Include/token.hpp"
#include "Include/parser.hpp"

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
    string fib = "n = 1;"
                 "minus1 = 1;"
                 "minus2 = 0;"
                 "while (n > 0) {"
                 "temp = minus2;"
                 "minus2 = minus1 + minus2;"
                 "minus1 = temp;"
                 "n = n - 1;"
                 "};"
                 "print(minus2);";
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
