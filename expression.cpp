#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Token {
    string text;
    string type; // "id", "num", "op", "paren"
};

class ExpressionAnalyzer {
public:
    vector<Token> tokens;
    vector<string> errors;

    void analyze(const string& expr) {
        tokens.clear();
        errors.clear();
        tokenize(expr);
        if (errors.empty()) validate();
    }

    void printTokens() const {
        cout << "\nTokens:\n";
        for (int i = 0; i < (int)tokens.size(); i++)
            cout << i << ": " << tokens[i].text << " -> " << tokens[i].type << "\n";
    }

    bool ok() const { return errors.empty(); }

    void printResult() const {
        if (errors.empty()) {
            cout << "\nExpression is syntactically valid.\n";
        } else {
            cout << "\nExpression is syntactically invalid.\n";
            for (int i = 0; i < (int)errors.size(); i++)
                cout << "Error: " << errors[i] << "\n";
        }
    }

private:
    bool isSpace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }
    bool isDigit(char c) { return c >= '0' && c <= '9'; }
    bool isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
    bool isAlnum(char c) { return isAlpha(c) || isDigit(c); }

    bool isOpChar(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
    }

    void addToken(const string& t, const string& type) { tokens.push_back({t, type}); }

    void tokenize(const string& expr) {
        int n = (int)expr.size();
        int i = 0;

        while (i < n) {
            char c = expr[i];

            if (isSpace(c)) { i++; continue; }

            if (isDigit(c)) {
                int j = i;
                while (j < n && isDigit(expr[j])) j++;
                if (j < n && expr[j] == '.') {
                    j++;
                    while (j < n && isDigit(expr[j])) j++;
                }
                addToken(expr.substr(i, j - i), "num");
                i = j;
                continue;
            }

            if (isAlpha(c) || c == '_') {
                int j = i;
                while (j < n && (isAlnum(expr[j]) || expr[j] == '_')) j++;
                addToken(expr.substr(i, j - i), "id");
                i = j;
                continue;
            }

            if (c == '(' || c == ')') {
                addToken(string(1, c), "paren");
                i++;
                continue;
            }

            if (isOpChar(c)) {
                string op(1, c);
                if ((c == '+' || c == '-') && i + 1 < n && expr[i + 1] == c) {
                    op.push_back(c);
                    i++;
                }
                addToken(op, "op");
                i++;
                continue;
            }

            errors.push_back(string("Unrecognized character: ") + c);
            i++;
        }
    }

    bool isUnaryOp(const string& op) {
        return op == "+" || op == "-" || op == "++" || op == "--";
    }

    void validate() {
        vector<char> paren;
        bool needOperand = true; // at start we need operand or '(' or unary op

        for (int i = 0; i < (int)tokens.size(); i++) {
            Token t = tokens[i];

            if (t.text == "(") {
                if (!needOperand) {
                    errors.push_back("Missing operator before '(' at token " + to_string(i));
                    return;
                }
                paren.push_back('(');
                needOperand = true;
                continue;
            }

            if (t.text == ")") {
                if (needOperand) {
                    errors.push_back("Operator before ')' or empty parentheses at token " + to_string(i));
                    return;
                }
                if (paren.empty()) {
                    errors.push_back("Unmatched ')' at token " + to_string(i));
                    return;
                }
                paren.pop_back();
                needOperand = false;
                continue;
            }

            if (t.type == "op") {
                if (needOperand) {
                    if (!isUnaryOp(t.text)) {
                        errors.push_back("Binary operator '" + t.text + "' missing left operand at token " + to_string(i));
                        return;
                    }
                    needOperand = true;
                } else {
                    needOperand = true;
                }
                continue;
            }

            if (t.type == "id" || t.type == "num") {
                if (!needOperand) {
                    errors.push_back("Missing operator before '" + t.text + "' at token " + to_string(i));
                    return;
                }
                needOperand = false;
                continue;
            }

            errors.push_back("Unknown token '" + t.text + "' at token " + to_string(i));
            return;
        }

        if (!paren.empty()) {
            errors.push_back("Missing closing parenthesis");
            return;
        }

        if (needOperand && !tokens.empty()) {
            errors.push_back("Expression ends with operator");
            return;
        }
    }
};

int main() {
    cout << "Enter the expression: ";
    string expr;
    getline(cin, expr);

    ExpressionAnalyzer analyzer;
    analyzer.analyze(expr);

    if (analyzer.ok()) analyzer.printTokens();
    analyzer.printResult();

    return 0;
}
