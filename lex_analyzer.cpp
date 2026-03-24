#include <bits/stdc++.h>

using namespace std;

static const unordered_set<string> KEYW = {
    "alignas","alignof","and","and_eq","asm","auto","bitand","bitor","bool","break",
    "case","catch","char","class","const","constexpr","continue","default","delete",
    "do","double","else","enum","explicit","extern","false","float","for","friend",
    "goto","if","inline","int","long","mutable","namespace","new","noexcept",
    "nullptr","operator","private","protected","public","register","return",
    "short","signed","sizeof","static","struct","switch","template","this",
    "throw","true","try","typedef","typename","union","unsigned","using",
    "virtual","void","volatile","while"
};

set<string> identifiers;
set<string> literalSet;
set<string> operators_;
set<string> keywords;
set<string> punctuations;
set<string> preprocessors;

vector<pair<string,string>> table;

void addToken(const string& lex, const string& kind) {
    table.emplace_back(lex, kind);

    if (kind == "identifier") identifiers.insert(lex);
    else if (kind == "keyword") keywords.insert(lex);
    else if (kind == "operator") operators_.insert(lex);
    else if (kind == "literal") literalSet.insert(lex);
    else if (kind == "punctuation") punctuations.insert(lex);
    else if (kind == "preprocessor") preprocessors.insert(lex);
}

bool isValidIdentifier(const string& s) {
    if (s.empty() || KEYW.count(s)) return false;
    if (!isalpha(static_cast<unsigned char>(s[0])) && s[0] != '_') return false;
    return all_of(s.begin()+1, s.end(), [](char c) {
        return isalnum(static_cast<unsigned char>(c)) || c == '_';
    });
}


static const regex PREPROC_RE(R"(^\s*#.*)");
static const regex TOKEN_RE(
    R"((\s+)|
    ("([^"\\]|\\.)*")|
    ('([^'\\]|\\.)*')|
    (\d+(\.\d+)?([eE][+-]?\d+)?)|
    ([A-Za-z_]\w*)|
    (==|!=|<=|>=|\+\+|--|&&|\|\||<<|>>|[-+*/%=<>&|^!~?:])|
    ([()\{\}\[\];,]))"
);


vector<string> getTokens(const string& line) {
    vector<string> tokens;

    if (regex_match(line, PREPROC_RE)) {
        addToken(line, "preprocessor");
        return tokens;
    }

    size_t pos = 0;
    while (pos < line.size()) {
        smatch m;
        if (!regex_search(line.begin()+pos, line.end(), m, TOKEN_RE,
                          regex_constants::match_continuous)) {
            pos++;
            continue;
        }

        if (m[1].matched) {
            pos += m.length();
            continue;
        }

        if (m[2].matched || m[4].matched || m[6].matched) {
            addToken(m.str(), "literal");
        }
        else if (m[8].matched) {
            string tok = m.str();
            if (KEYW.count(tok)) addToken(tok, "keyword");
            else addToken(tok, "identifier");
        }
        else if (m[9].matched) addToken(m.str(), "operator");
        else if (m[10].matched) addToken(m.str(), "punctuation");

        tokens.push_back(m.str());
        pos += m.length();
    }

    return tokens;
}


template<typename T>
void printSet(const T& s, const string& name) {
    cout << name << ": ";
    for (const auto& x : s) cout << x << " ";
    cout << "\n";
}

void printLexTable() {
    cout << "\nS.No\tLexeme\t\tType\n";
    int i = 1;
    for (const auto& p : table)
        cout << i++ << "\t" << p.first << "\t\t" << p.second << "\n";
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    ifstream in(argv[1]);
    if (!in) {
        cout << "File not found\n";
        return 1;
    }

    string line;
    while (getline(in, line)) {
        getTokens(line);
    }

    cout << "\n----- Retrieved Objects -----\n";
    printSet(identifiers, "Identifiers");
    printSet(literalSet, "Literals");
    printSet(keywords, "Keywords");
    printSet(operators_, "Operators");
    printSet(punctuations, "Punctuations");
    printSet(preprocessors, "Preprocessors");

    cout << "\n----- Lexeme Table -----\n";
    printLexTable();

    return 0;
}
