#include <iostream>
#include <string>
#include <regex>
#include <fstream>

using namespace std;

string cdecl_translate(const string& input) {
    regex declaration_regex(R"(^\s*(int|char|float|double|void)\s*(.*)\s*;?\s*$)");
    smatch match;

    if (!regex_match(input, match, declaration_regex)) {
        return "Syntax error at position " + to_string(input.find_first_not_of(" \t"));
    }

    string type = match[1].str();
    string rest = match[2].str();
    string result;

    regex function_pointer_regex(R"(\(\s*\*\s*(\w+)\s*\)\s*\(\s*\))");
    smatch func_match;
    if (regex_search(rest, func_match, function_pointer_regex)) {
        string var_name = func_match[1].str();
        result = "declare " + var_name + " as pointer to function returning " + type;
    }
    else if (rest.find('*') != string::npos) {
        regex pointer_regex(R"((\s*\*+)\s*(\w+))");
        smatch pointer_match;
        if (regex_search(rest, pointer_match, pointer_regex)) {
            string var_name = pointer_match[2].str();
            size_t pointer_count = pointer_match[1].str().size();
            result = "declare " + var_name + " as ";
            for (size_t i = 0; i < pointer_count; ++i) {
                result += "pointer to ";
            }
            result += type;
        }
    }
    else if (rest.find('[') != string::npos) {
        regex array_regex(R"((\w+)\s*(\[[^\]]*\]))");
        smatch array_match;
        if (regex_search(rest, array_match, array_regex)) {
            string var_name = array_match[1].str();
            result = "declare " + var_name + " as ";
            regex array_size_regex(R"(\[(\d*)\])");
            sregex_iterator it(array_match[2].first, array_match[2].second, array_size_regex);
            sregex_iterator end;
            while (it != end) {
                string size = (*it)[1].str();
                if (size.empty()) {
                    result += "array of unknown size of ";
                }
                else {
                    result += "array of " + size + " elements of ";
                }
                ++it;
            }
            result += type;
        }
    }
    else {
        regex simple_var_regex(R"((\w+))");
        smatch var_match;
        if (regex_match(rest, var_match, simple_var_regex)) {
            string var_name = var_match[1].str();
            result = "declare " + var_name + " as " + type;
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    ifstream input_file(argv[1]);
    if (!input_file.is_open()) {
        cerr << "Error: Could not open file " << argv[1] << endl;
        return 1;
    }

    string line;
    while (getline(input_file, line)) {
        string result = cdecl_translate(line);
        cout << result << endl;
    }

    input_file.close();
    return 0;
}