#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

string expr;
int cursor;
int tempCount;

// Forward declarations
string E();
string T();
string F();

string newTemp() {
    return "t" + to_string(tempCount++);
}

// Rule for E (Addition and Subtraction)
string E() {
    string left = T();
    while (cursor < expr.length() && (expr[cursor] == '+' || expr[cursor] == '-')) {
        char op = expr[cursor++];   // Match Operator
        string right = T();         // Parse Right side

        string result = newTemp();  // Create temp
        cout << "  " << result << " = " << left << " " << op << " " << right << "\n";

        left = result;              // The temp becomes the new left side
    }
    return left;
}


// Rule for T (Multiplication and Division)
string T() {
    string left = F();
    while (cursor < expr.length() && (expr[cursor] == '*' || expr[cursor] == '/')) {
        char op = expr[cursor++];   // Match Operator
        string right = F();         // Parse Right side

        string result = newTemp();  // Create temp
        cout << "  " << result << " = " << left << " " << op << " " << right << "\n";

        left = result;              // The temp becomes the new left side
    }
    return left;
}

// Rule for F (Parentheses and IDs)
string F() {
    if (cursor < expr.length() && isalnum(expr[cursor])) {
        string id = "";
        while (cursor < expr.length() && isalnum(expr[cursor])) {
            id += expr[cursor++];
        }
        return id; // Base case: return the variable name directly
    }
    else if (cursor < expr.length() && expr[cursor] == '(') {
        cursor++;          // Match '('
        string temp = E(); // Parse inner expression
        cursor++;          // Match ')'
        return temp;       // Return whatever temp the inner expression evaluated to
    }
    return "";
}

// Wrapper function to clean input and kick off parsing
void generateTAC(string input) {
    expr = "";
    for (char c : input) if (!isspace(c)) expr += c; // Remove spaces

    cout << "Expression: " << input << "\n";

    cursor = 0;
    tempCount = 1; // Reset temp counter to t1 for each expression

    string target = "";

    // Check if the expression contains an assignment (e.g., "c =")
    int eqPos = expr.find('=');
    if (eqPos != string::npos) {
        target = expr.substr(0, eqPos);
        cursor = eqPos + 1; // Move parser to the start of the right-hand side
    }

    // Begin Recursive Parsing
    string finalResult = E();

    // Final assignment back to the target variable
    if (target != "") {
        cout << "  " << target << " = " << finalResult << "\n";
    }
    cout << "-------------------------------------------\n";
}

int main() {
    ifstream file("tac_input.txt");
    if (!file) {
        cout << "Error: Missing tac_input.txt\n";
        return 1;
    }

    cout << "--- Three Address Code Generator ---\n\n";
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            generateTAC(line);
        }
    }

    return 0;
}
