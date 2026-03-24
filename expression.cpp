#include <bits/stdc++.h>
using namespace std;

struct Token {
    string lexeme;
    string type;
};

bool isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

vector<Token> tokenize(const string& expr, vector<string>& errors) {
    vector<Token> tokens;
    const int n = static_cast<int>(expr.size());

    for (int i = 0; i < n; ) {
        char c = expr[i];

        if (isspace(static_cast<unsigned char>(c))) {
            ++i;
            continue;
        }

        if (isdigit(static_cast<unsigned char>(c))) {
            int j = i;
            bool dotSeen = false;
            while (j < n) {
                char cj = expr[j];
                if (isdigit(static_cast<unsigned char>(cj))) {
                    ++j;
                } else if (cj == '.') {
                    if (dotSeen) break;
                    dotSeen = true;
                    ++j;
                } else {
                    break;
                }
            }
            tokens.push_back({expr.substr(i, j - i), "number"});
            i = j;
            continue;
        }

        if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
            int j = i;
            while (j < n) {
                char cj = expr[j];
                if (isalnum(static_cast<unsigned char>(cj)) || cj == '_') ++j;
                else break;
            }
            tokens.push_back({expr.substr(i, j - i), "identifier"});
            i = j;
            continue;
        }

        if (c == '(' || c == ')') {
            tokens.push_back({string(1, c), "parenthesis"});
            ++i;
            continue;
        }

        if (isOperatorChar(c)) {
            string op(1, c);
            if ((c == '+' || c == '-') && i + 1 < n && expr[i + 1] == c) {
                op.push_back(c);
                ++i;
            }
            tokens.push_back({op, "operator"});
            ++i;
            continue;
        }

        errors.push_back("Unrecognized character: " + string(1, c));
        ++i;
    }

    return tokens;
}

void printTokens(const vector<Token>& tokens) {
    cout << "\nTokens after lexical analysis:\n";
    for (size_t i = 0; i < tokens.size(); ++i) {
        cout << i << ": " << tokens[i].lexeme << " -> " << tokens[i].type << '\n';
    }
}

bool validateParentheses(const vector<Token>& tokens, vector<string>& errors) {
    vector<char> st;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].lexeme == "(") st.push_back('(');
        else if (tokens[i].lexeme == ")") {
            if (st.empty()) {
                errors.push_back("Extra closing parenthesis at token " + to_string(i));
                return false;
            }
            st.pop_back();
        }
    }
    if (!st.empty()) {
        errors.push_back("Missing closing parenthesis");
        return false;
    }
    return true;
}

bool validateSyntax(const vector<Token>& tokens, vector<string>& errors) {
    bool expectOperand = true;
    vector<char> parenStack;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& t = tokens[i];

        if (t.lexeme == "(") {
            if (!expectOperand) {
                errors.push_back("Misplaced '(' before token " + to_string(i));
                return false;
            }
            parenStack.push_back('(');
            expectOperand = true;
        }
        else if (t.lexeme == ")") {
            if (expectOperand) {
                errors.push_back("Empty parentheses or operator before ')' at token " + to_string(i));
                return false;
            }
            if (parenStack.empty()) {
                errors.push_back("Unmatched ')' at token " + to_string(i));
                return false;
            }
            parenStack.pop_back();
            expectOperand = false;
        }
        else if (t.type == "operator") {
            bool isUnary = expectOperand && (t.lexeme == "+" || t.lexeme == "-" || t.lexeme == "++" || t.lexeme == "--");
            if (expectOperand && !isUnary) {
                errors.push_back("Binary operator '" + t.lexeme + "' missing left operand at token " + to_string(i));
                return false;
            }
            expectOperand = true;
        }
        else {
            if (!expectOperand) {
                errors.push_back("Missing operator before '" + t.lexeme + "' at token " + to_string(i));
                return false;
            }
            expectOperand = false;
        }
    }

    if (expectOperand && !tokens.empty()) {
        errors.push_back("Expression ends with operator");
        return false;
    }
    if (!parenStack.empty()) {
        errors.push_back("Unbalanced parentheses");
        return false;
    }
    return errors.empty();
}

int main() {
    cout << "Enter the expression: ";
    string expression;
    getline(cin, expression);

    vector<string> lexicalErrors;
    vector<Token> tokens = tokenize(expression, lexicalErrors);

    if (!lexicalErrors.empty()) {
        for (const auto& err : lexicalErrors) {
            cout << "Lexical error: " << err << '\n';
        }
        cout << "Expression is syntactically invalid due to lexical errors.\n";
        return 0;
    }

    printTokens(tokens);

    vector<string> validationErrors;
    bool parenOk = validateParentheses(tokens, validationErrors);
    bool syntaxOk = validateSyntax(tokens, validationErrors);

    if (parenOk && syntaxOk && validationErrors.empty()) {
        cout << "\nExpression is syntactically valid.\n";
    } else {
        for (const auto& err : validationErrors) {
            cout << "Error: " << err << '\n';
        }
        cout << "\nExpression is syntactically invalid.\n";
    }

    return 0;
}
