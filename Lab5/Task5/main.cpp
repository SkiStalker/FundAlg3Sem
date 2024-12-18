#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <stdexcept>

using namespace std;

const regex IDENTIFIER_PATTERN(R"(^[a-zA-Z_][a-zA-Z0-9_]*$)");
const regex COMPLETE_DECL_PATTERN(R"(^\s*(int|char|float|double|void)\s+((\**\s*)?([a-zA-Z_][a-zA-Z0-9_]*))(\[.*\])*(\s*;\s*)?$)");
const regex FUNCTION_DECL_PATTERN(R"(^\s*(int|char|float|double|void)\s+\(((\**\s*)?([a-zA-Z_][a-zA-Z0-9_]*))(\[.*\])*\)(\(.*\))?(\s*;\s*)?$)");
const regex ARRAY_PATTERN(R"(\[([0-9]*)\])");

bool isIdentifierValid(const string &name)
{
    return regex_match(name, IDENTIFIER_PATTERN);
}

string translateCDeclaration(const string &decl)
{
    try
    {
        smatch matches;

        if (!regex_match(decl, matches, COMPLETE_DECL_PATTERN) && !regex_match(decl, matches, FUNCTION_DECL_PATTERN))
        {
            return "Syntax error in declaration: " + decl;
        }

        const string name = matches[4].str();

        if (!isIdentifierValid(name))
        {
            return "Invalid identifier at position " + to_string(decl.find(name)) + ";";
        }

        const string pointerSection = matches[3].str();
        string result = "Declare " + name + " as ";
        const size_t pointerCount = count(pointerSection.begin(), pointerSection.end(), '*');

        for (size_t i = 0; i < pointerCount; ++i)
        {
            result += "pointer to ";
        }

        const string arraySection = matches[5].str();
        if (!arraySection.empty())
        {
            auto arrayBegin = sregex_iterator(arraySection.begin(), arraySection.end(), ARRAY_PATTERN);
            auto arrayEnd = sregex_iterator();

            for (auto iter = arrayBegin; iter != arrayEnd; ++iter)
            {
                const string size = (*iter)[1].str();
                result += "array of " + (size.empty() ? "" : size + " elements of ");
            }
        }

        const string baseType = matches[1].str();
        result += baseType;

        const string functionSection = matches[6].str();
        if (!functionSection.empty() && functionSection != ";")
        {
            result += " with arguments: " + functionSection;
        }

        return result;
    }
    catch (const exception &e)
    {
        return "Error occurred: " + string(e.what());
    }
}

void processFile(const string &path)
{
    ifstream inputFile(path);
    if (!inputFile.is_open())
    {
        cerr << "Failed to open file: " << path << endl;
        return;
    }

    string line;
    while (getline(inputFile, line))
    {
        cout << translateCDeclaration(line) << '\n';
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " file" << endl;
        return -1;
    }

    processFile(argv[1]);
    return 0;
}