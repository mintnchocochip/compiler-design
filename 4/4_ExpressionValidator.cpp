#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

class ExpressionValidator {
    // Token structure: type is 'V' (Value/ID), 'O' (Operator), '(', or ')'
    struct Token { string val; char type; };
    vector<Token> tokens;
    string err = "";

public:
    void analyze(string expr) {
        tokenize(expr);
        if (err.empty()) validate();

        // Final Output
        if (!err.empty()) cout << "\nINVALID -> " << err << "\n";
        else cout << "\nVALID\n";
    }

private:
    void tokenize(string s) {
        for (size_t i = 0; i < s.length(); ) {
            char c = s[i];

            if (isspace(c)) {
                i++;
            }
            // 1. Group Identifiers and Numbers together as 'Values' ('V')
            else if (isalnum(c) || c == '_' || c == '.') {
                string v = "";
                while (i < s.length() && (isalnum(s[i]) || s[i] == '_' || s[i] == '.'))
                    v += s[i++];
                tokens.push_back({v, 'V'});
            }
            // 2. Parentheses
            else if (c == '(' || c == ')') {
                tokens.push_back({string(1, c), c});
                i++;
            }
            // 3. Operators ('O')
            else if (string("+-*/%^=").find(c) != string::npos) {
                string o(1, c);
                i++;
                if (i < s.length() && c == s[i] && (c == '+' || c == '-' || c == '='))
                    o += s[i++]; // Handle ++, --, ==
                tokens.push_back({o, 'O'});
            }
            else {
                err = "Unknown char: " + string(1, c);
                return;
            }
        }

        // Print tokens
        for (auto t : tokens) cout << "   [" << t.type << "] " << t.val << "\n";
    }

    void validate() {
        int parens = 0;
        bool expectVal = true; // true = expect Value or '(', false = expect Operator or ')'

        for (auto t : tokens) {
            if (t.type == '(') {
                if (!expectVal) { err = "Missing operator before '('"; return; }
                parens++;
            }
            else if (t.type == ')') {
                if (expectVal) { err = "Unexpected ')'"; return; }
                parens--;
                if (parens < 0) { err = "Unmatched ')'"; return; }
            }
            else if (t.type == 'O') {
                if (expectVal && t.val != "+" && t.val != "-" && t.val != "++" && t.val != "--") {
                    err = "Invalid unary operator: " + t.val; return;
                }
                expectVal = true;
            }
            else if (t.type == 'V') {
                if (!expectVal) { err = "Missing operator before " + t.val; return; }
                expectVal = false;
            }
        }

        if (parens > 0) err = "Missing ')'";
        else if (expectVal && !tokens.empty()) err = "Expression ends with operator";
    }
};

int main() {
    ifstream file("expr_input.txt");
    string expr = "a + b * (c - ++d)"; // Default if file missing
    if (file) getline(file, expr);

    ExpressionValidator().analyze(expr);
    return 0;
}
