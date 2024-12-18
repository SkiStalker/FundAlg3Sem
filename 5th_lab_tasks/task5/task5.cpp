#include <iostream>
#include <string>
#include <regex>
#include <stack>
#include <fstream>

bool isValidIdentifier(const std::string& identifier) {
    std::regex identifierRegex("^[a-zA-Z_][a-zA-Z0-9_]*$");
    return std::regex_match(identifier, identifierRegex);
}

std::string cdecl_translate(const std::string& declaration) {
    try {
        std::smatch match;
        std::regex baseTypeRegex("^(int|char|float|double|void)");
        std::regex fullDeclarationRegex(
                R"(^\s*(int|char|float|double|void)\s+((\**\s*)?([a-zA-Z_][a-zA-Z0-9_]*))(\[.*\])*(\s*;\s*)?$)"
        );
        std::regex funcDeclarationRegex(
                R"(^\s*(int|char|float|double|void)\s+\(((\**\s*)?([a-zA-Z_][a-zA-Z0-9_]*))(\[.*\])*\)(\(.*\))?(\s*;\s*)?$)"
        );

        if (!std::regex_match(declaration, match, fullDeclarationRegex) && !std::regex_match(declaration, match, funcDeclarationRegex)) {
            return "Syntax error in declaration: " + declaration;
        }

        std::string baseType = match[1].str();
        std::string identifier = match[4].str();
        std::string pointerPart = match[3].str();
        std::string arrayPart = match[5].str();
        std::string funcPart = match[6].str();

        if (!isValidIdentifier(identifier)) {
            return "Syntax error at position " + std::to_string(declaration.find(identifier));
        }

        std::string description = "declare " + identifier + " as ";

        size_t pointerCount = std::count(pointerPart.begin(), pointerPart.end(), '*');
        for (size_t i = 0; i < pointerCount; ++i) {
            description += "pointer to ";
        }

        if (!arrayPart.empty()) {
            std::regex arrayRegex(R"(\[([0-9]*)\])");
            std::sregex_iterator begin(arrayPart.begin(), arrayPart.end(), arrayRegex);
            std::sregex_iterator end;

            for (auto it = begin; it != end; ++it) {
                std::string size = (*it)[1].str();
                if (size.empty()) {
                    description += "array of ";
                } else {
                    description += "array of " + size + " elements of ";
                }
            }
        }

        description += baseType;

        if (!funcPart.empty() && (funcPart != ";")) {
            description += " getting as args: " + funcPart;
        }

        return description;

    } catch (const std::exception& e) {
        return std::string("Error: ") + e.what();
    }
}
// Интерпретатор для работы с файлами
void interpretFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << cdecl_translate(line) << std::endl;
    }

    file.close();
}

// Главная функция
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    interpretFile(filePath);

    return 0;
}