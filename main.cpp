#include "Include/regex.hpp"
#include "Include/value.hpp"
#include "Include/token.hpp"
#include "Include/parser.hpp"

int main()
{
    string str = "i = 0;" "\n"
                 "if i > 5{" "\n"
                 "i = i + 1;" "\n"
                 "}" "\n"
                 "else{"
                 "i = i + 2;"  
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
