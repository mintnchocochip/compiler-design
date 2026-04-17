#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <cctype>

using namespace std;


set<string> identifiers;
set<string> literals;
set<string> operators;
set<string> keywords;
set<string> punctuations;
set<string> preprocessors;

vector<pair<string, string>> lexTable;


set<string> keywordSet = {
    "int", "float", "char", "double", "void", "if", "else", "for", "while",
    "do", "return", "class", "struct", "public", "private", "protected", "namespace", "using"
};

void addToken(string lexeme, string type) {
    lexTable.push_back({lexeme, type});

    if (type == "identifier") identifiers.insert(lexeme);
    else if (type == "keyword") keywords.insert(lexeme);
    else if (type == "operator") operators.insert(lexeme);
    else if (type == "literal") literals.insert(lexeme);
    else if (type == "punctuation") punctuations.insert(lexeme);
    else if (type == "preprocessor") preprocessors.insert(lexeme);
}

void analyzeLine(string line) {
    int n = line.length();
    int i = 0;

    while (i < n) {
        char c = line[i];

        // 1. Skip Whitespaces
        if (isspace(c)) {
            i++;
            continue;
        }

        // 2. Preprocessor Directives (e.g., #include <iostream>)
        if (c == '#') {
            addToken(line, "preprocessor");
            break;
        }

        // 3. String Literals (e.g., "hello")
        if (c == '"') {
            string temp = "";
            temp += c;
            i++;
            while (i < n && line[i] != '"') {
                temp += line[i];
                i++;
            }
            if (i < n) {
                temp += line[i];
                i++;
            }
            addToken(temp, "literal");
            continue;
        }

        // 4. Identifiers and Keywords
        if (isalpha(c) || c == '_') {
            string temp = "";
            while (i < n && (isalnum(line[i]) || line[i] == '_')) {
                temp += line[i];
                i++;
            }
            if (keywordSet.count(temp)) {
                addToken(temp, "keyword");
            } else {
                addToken(temp, "identifier");
            }
            continue;
        }

        // 5. Numeric Literals
        if (isdigit(c)) {
            string temp = "";
            while (i < n && (isdigit(line[i]) || line[i] == '.')) {
                temp += line[i];
                i++;
            }
            addToken(temp, "literal");
            continue;
        }

        // 6. Operators
        string opChars = "+-*/%=<>!&|";
        if (opChars.find(c) != string::npos) {
            string temp = "";
            temp += c;
            i++;
            // Check for 2-character operators like ==, <=, ++
            if (i < n && opChars.find(line[i]) != string::npos) {
                temp += line[i];
                i++;
            }
            addToken(temp, "operator");
            continue;
        }

        // 7. Punctuations
        string punctChars = "(){}[];,";
        if (punctChars.find(c) != string::npos) {
            string temp = "";
            temp += c;
            addToken(temp, "punctuation");
            i++;
            continue;
        }
        i++;
    }
}


void printSet(const set<string>& s, string name) {
    cout << name << ": ";
    for (const string& x : s) {
        cout << x << " ";
    }
    cout << "\n";
}

int main() {

    string filename = "lex_input.txt";
    ifstream file(filename);

    if (!file) {
        cout << "Error: File '" << filename << "' not found.\n";
        return 1;
    }

    string line;
    while (getline(file, line)) {
        analyzeLine(line);
    }


    cout << "\n----- Retrieved Objects -----\n";
    printSet(identifiers, "Identifiers");
    printSet(literals, "Literals");
    printSet(keywords, "Keywords");
    printSet(operators, "Operators");
    printSet(punctuations, "Punctuations");
    printSet(preprocessors, "Preprocessors");

    cout << "\n----- Lexeme Table -----\n";
    cout << "S.No\tLexeme\t\tType\n";
    cout << "------------------------------------\n";
    int index = 1;
    for (auto& row : lexTable) {
        cout << index++ << "\t" << row.first << "\t\t" << row.second << "\n";
    }

    return 0;
}
