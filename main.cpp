#include "Include/regex.hpp"
#include "Include/value.hpp"
#include "Include/token.hpp"
#include "Include/parser.hpp"
#include <fstream>

using std::fstream;
// if and else required
// semi colons after if else stantments and while statements
// semi colons after all stantment
// only int is the type

string getStringFromFile(string filename)
{
    string stringIn = "";
    string str;
    fstream file(filename);
    while (getline(file, str))
    {
        stringIn += str;
    }
    return stringIn;
}

int main(int argc, char *argv[])
{
    if(argc <= 1){
        cout << "input file by adding using command: \"./lang filename.amn\"\n";
        exit(0);
    }
    string filename(argv[1]);
    string program = getStringFromFile(filename);
    auto input = getTokensFromLang(program);


    Program parser;
    try
    {
        auto output = parser.parse_all(input);
        auto it = output.begin();
        
        if (it != output.end())
        {
            map<string, int> env;
            auto test = (*it);
            for(int i = 0; i < test.size();++i)
            {
                cout << test[i]->getString() << "\n";
            }
           // cout << test->getString() << "\n";
        }
    }
    catch (RunTimeError &e)
    {
        cout << e.what() << "\n";
        exit(0);
    }

    return 0;
}
