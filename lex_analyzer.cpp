#inlclude <bits/stdc++.h>

using namespace std;

struct Token {
    string type;
    string lexeme;
    int line;
};

const unordered_set<string> KEYWORDS = {
    "alignas","alignof","and","and_eq","asm","auto","bool","break","case","catch","char","char16_t","char32_t","char8_t","class",
    "compl","concept","const","consteval","constexpr","constinit","const_cast","continue","co_await","co_return","co_yield","decltype",
    "default","delete","do","double","dynamic_cast","else","enum","explicit","export","extern","false","float","for","friend","goto",
    "if","inline","int","long","mutable","namespace","new","noexcept","not","not_eq","nullptr","operator","or","or_eq","private",
    "protected","public","register","reinterpret_cast","requires","return","short","signed","sizeof","static","static_assert",
    "static_cast","struct","switch","template","this","thread_local","throw","true","try","typedef","typeid","typename","union",
    "unsigned","using","virtual","void","volatile","wchar_t","while"
};

const unordered_set<char> PUNCTUATIONS = {';',',','(',')','{','}','[',']','?','.',':','#'};
const unordered_set<char> OPERATOR_CHARS = {'+','-','*','/','%','=','<','>','&','|','^','~','!'};
const vector<string> MULTI_OPERATORS = {
    "<<=",">>=","==","!=","<=",">=","&&","||","++","--","+=","-=","*=","/=","%=","<<",">>","->","::","&=","|=","^=","##"
};

bool isIdentifierStart(char c) {
    return isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool isIdentifierPart(char c) {
    return isalnum(static_cast<unsigned char>(c)) || c == '_';
}

bool isKeyword(const string& word) {
    return KEYWORDS.find(word) != KEYWORDS.end();
}

bool matchMultiOperator(const string& line, size_t index, string& matched) {
    for (const auto& op : MULTI_OPERATORS) {
        if (index + op.size() <= line.size() && line.compare(index, op.size(), op) == 0) {
            matched = op;
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Provide input file path\n";
        return 1;
    }
    ifstream source(argv[1]);
    if (!source) {
        cout << "Cannot open file\n";
        return 1;
    }
    vector<vector<Token>> tokenLines;
    unordered_map<string, size_t> symbolLocations;
    vector<pair<string, int>> symbolTable;
    vector<pair<int, string>> diagnostics;
    size_t identifierCount = 0;
    size_t literalCount = 0;
    size_t operatorCount = 0;
    size_t punctuationCount = 0;
    size_t keywordCount = 0;
    bool inBlockComment = false;
    string line;
    int currentLine = 0;
    while (getline(source, line)) {
        ++currentLine;
        vector<Token> tokens;
        size_t i = 0;
        while (i < line.size()) {
            if (inBlockComment) {
                if (line[i] == '*' && i + 1 < line.size() && line[i + 1] == '/') {
                    inBlockComment = false;
                    i += 2;
                } else {
                    ++i;
                }
                continue;
            }
            if (isspace(static_cast<unsigned char>(line[i]))) {
                ++i;
                continue;
            }
            if (line[i] == '/' && i + 1 < line.size()) {
                if (line[i + 1] == '/') {
                    break;
                }
                if (line[i + 1] == '*') {
                    inBlockComment = true;
                    i += 2;
                    continue;
                }
            }
            if (line[i] == '"') {
                size_t start = i;
                ++i;
                bool closed = false;
                while (i < line.size()) {
                    if (line[i] == '\\' && i + 1 < line.size()) {
                        i += 2;
                        continue;
                    }
                    if (line[i] == '"') {
                        closed = true;
                        ++i;
                        break;
                    }
                    ++i;
                }
                string lexeme = line.substr(start, i - start);
                tokens.push_back({"STRING_LITERAL", lexeme, currentLine});
                ++literalCount;
                if (!closed) {
                    diagnostics.push_back({currentLine, "Unterminated string literal: " + lexeme});
                }
                continue;
            }
            if (line[i] == '\'') {
                size_t start = i;
                ++i;
                bool closed = false;
                while (i < line.size()) {
                    if (line[i] == '\\' && i + 1 < line.size()) {
                        i += 2;
                        continue;
                    }
                    if (line[i] == '\'') {
                        closed = true;
                        ++i;
                        break;
                    }
                    ++i;
                }
                string lexeme = line.substr(start, i - start);
                tokens.push_back({"CHAR_LITERAL", lexeme, currentLine});
                ++literalCount;
                if (!closed) {
                    diagnostics.push_back({currentLine, "Unterminated character literal: " + lexeme});
                }
                continue;
            }
            if (isdigit(static_cast<unsigned char>(line[i])) || (line[i] == '.' && i + 1 < line.size() && isdigit(static_cast<unsigned char>(line[i + 1])))) {
                size_t start = i;
                bool hasDot = false;
                if (line[i] == '.') {
                    hasDot = true;
                    ++i;
                }
                while (i < line.size() && isdigit(static_cast<unsigned char>(line[i]))) {
                    ++i;
                }
                if (i < line.size() && line[i] == '.' && !hasDot) {
                    hasDot = true;
                    ++i;
                    while (i < line.size() && isdigit(static_cast<unsigned char>(line[i]))) {
                        ++i;
                    }
                }
                if (i < line.size() && (line[i] == 'e' || line[i] == 'E')) {
                    size_t expStart = i;
                    ++i;
                    if (i < line.size() && (line[i] == '+' || line[i] == '-')) {
                        ++i;
                    }
                    bool expDigits = false;
                    while (i < line.size() && isdigit(static_cast<unsigned char>(line[i]))) {
                        expDigits = true;
                        ++i;
                    }
                    if (!expDigits) {
                        string fragment = line.substr(start, i - start);
                        diagnostics.push_back({currentLine, "Invalid numeric literal exponent: " + fragment});
                    }
                }
                if (i < line.size() && (isalpha(static_cast<unsigned char>(line[i])) || line[i] == '_')) {
                    size_t invalidStart = start;
                    while (i < line.size() && (isalnum(static_cast<unsigned char>(line[i])) || line[i] == '_')) {
                        ++i;
                    }
                    string lexeme = line.substr(invalidStart, i - invalidStart);
                    tokens.push_back({"INVALID", lexeme, currentLine});
                    diagnostics.push_back({currentLine, "Invalid identifier: " + lexeme});
                } else {
                    string lexeme = line.substr(start, i - start);
                    tokens.push_back({"NUMERIC_LITERAL", lexeme, currentLine});
                    ++literalCount;
                }
                continue;
            }
            if (isIdentifierStart(line[i])) {
                size_t start = i;
                ++i;
                while (i < line.size() && isIdentifierPart(line[i])) {
                    ++i;
                }
                string lexeme = line.substr(start, i - start);
                if (isKeyword(lexeme)) {
                    tokens.push_back({"KEYWORD", lexeme, currentLine});
                    ++keywordCount;
                } else {
                    tokens.push_back({"IDENTIFIER", lexeme, currentLine});
                    ++identifierCount;
                    if (symbolLocations.find(lexeme) == symbolLocations.end()) {
                        symbolLocations[lexeme] = symbolTable.size();
                        symbolTable.push_back({lexeme, currentLine});
                    }
                }
                continue;
            }
            string matchedOperator;
            if (matchMultiOperator(line, i, matchedOperator)) {
                tokens.push_back({"OPERATOR", matchedOperator, currentLine});
                ++operatorCount;
                i += matchedOperator.size();
                continue;
            }
            if (OPERATOR_CHARS.find(line[i]) != OPERATOR_CHARS.end()) {
                string lexeme(1, line[i]);
                tokens.push_back({"OPERATOR", lexeme, currentLine});
                ++operatorCount;
                ++i;
                continue;
            }
            if (PUNCTUATIONS.find(line[i]) != PUNCTUATIONS.end()) {
                string lexeme(1, line[i]);
                tokens.push_back({"PUNCTUATION", lexeme, currentLine});
                ++punctuationCount;
                ++i;
                continue;
            }
            string lexeme(1, line[i]);
            tokens.push_back({"UNKNOWN", lexeme, currentLine});
            diagnostics.push_back({currentLine, "Unknown token: " + lexeme});
            ++i;
        }
        tokenLines.push_back(tokens);
    }
    if (inBlockComment) {
        diagnostics.push_back({currentLine, "Unterminated block comment"});
    }
    cout <<endl<< "Token Stream\n";
    for (size_t lineIndex = 0; lineIndex < tokenLines.size(); ++lineIndex) {
        cout << "Line " << lineIndex + 1 << ":";
        if (tokenLines[lineIndex].empty()) {
            cout << " (no tokens)";
        }
        cout << "\n";
        for (const auto& token : tokenLines[lineIndex]) {
            cout << "  <" << token.type << ", " << token.lexeme << ">\n";
        }
    }
    cout <<endl<< "Symbol Table\n";
    if (symbolTable.empty()) {
        cout << "  (empty)\n";
    } else {
        cout << left << setw(10) << "Index" << setw(25) << "Identifier" << "FirstLine\n";
        for (size_t index = 0; index < symbolTable.size(); ++index) {
            cout << left << setw(10) << index + 1 << setw(25) << symbolTable[index].first << symbolTable[index].second << "\n";
        }
    }
    cout <<endl<< "Category Counts\n";
    cout << "  Identifiers: " << identifierCount << "\n";
    cout << "  Literals: " << literalCount << "\n";
    cout << "  Operators: " << operatorCount << "\n";
    cout << "  Punctuations: " << punctuationCount << "\n";
    cout << "  Keywords: " << keywordCount << "\n";
    if (!diagnostics.empty()) {
        cout << "Errors\n";
        for (const auto& entry : diagnostics) {
            cout << "  Line " << entry.first << ": " << entry.second << "\n";
        }
    }
    return 0;
}
