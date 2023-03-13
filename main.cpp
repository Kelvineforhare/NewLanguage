#include "Include/regex.hpp"
#include "Include/value.hpp"
#include "Include/token.hpp"
#include "Include/parser.hpp"

int main()
{
    string str = "n = 1;" "\n"
                 "f = 1;" "\n"
                 "while f + n < 10{" "\n"
                 "f = f + n;"
                 "print(f);"
                 "print(n);"
                 "};" "\n";
    auto input = getTokensFromLang(str);
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
