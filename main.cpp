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
        stringIn += str + "\n";
    }
    return stringIn;
}

void execute(set<vector<shared_ptr<Decl>>>::iterator it)
{
    //catch run time exceptions
    map<string, int> env;
    auto test = (*it);
    map<string,Def> functions;
    Main mainFun;
    for (int i = 0; i < test.size(); ++i)
    {
        shared_ptr<Def> op = dynamic_pointer_cast<Def>(test[i]);
        if(op != nullptr)
        {
            functions.insert({op->getFunName(),*op.get()});
        }
        else{
            shared_ptr<Main> op = dynamic_pointer_cast<Main>(test[i]);
            mainFun = *op.get();
        }
    }
    mainFun.eval_dec(env,functions);
}

void runProgram(string program)
{
    auto input = getTokensFromLang(program);

    Program parser;
    try
    {
        auto output = parser.parse_all(input);
        auto it = output.begin();
        

        if (it != output.end())
        {
            execute(it);
        }
        else 
        {
            auto tryParse = parser.parse(input);
            auto it = tryParse.begin();
            vector<shared_ptr<Token>> notParsed;
            if (it != tryParse.end())
            {
                auto test = (*it).second;
                notParsed.insert(notParsed.begin(), test.begin(), test.end());
                ++it;
            }
            for (int i = 0; i < notParsed.size(); ++i)
            {
                cout << notParsed[i]->toString() + " , ";
            }
            cout << "\nTokens not parsed\n";
        }
    }
    catch ( RunTimeError &e)
    {
        cout << e.what() << "\n";
        exit(0);
    }
}

int main(int argc, char *argv[])
{

    if (argc <= 1)
    {
        cout << "input file by adding using command: \"./lang filename.amn\"\n";
        exit(0);
    }
    string filename(argv[1]);
    string program = getStringFromFile(filename);

    runProgram(program);

    return 0;
}
