#include "Include/regex.hpp"
#include "Include/value.hpp"
#include "Include/token.hpp"
#include "Include/parser.hpp"

int main()
{
    auto input = getTokensFromLang("true && false || true && false || true");
    BExpParser parser;
    auto output = parser.parse_all(input);
    auto it = output.begin();

    if (it != output.end())
    {
        cout << (*it)->getString() << "\n";
        map<string, int> env;
        cout << (*it)->eval_bexp(env) << "\n";
    }

    return 0;
}
