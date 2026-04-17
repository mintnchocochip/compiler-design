#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

string expr;
int cursor;
int tempCount;

string E();
string T();
string F();

string newTemp() {
    return "t" + to_string(tempCount++);
}

string E() {
    string left = T();
    while (cursor < expr.length() && (expr[cursor] == '+' || expr[cursor] == '-')) {
        char op = expr[cursor++];
        string right = T();
        string result = newTemp();
        cout << "  " << result << " = " << left << " " << op << " " << right << "\n";
        left = result;
    }
    return left;
}

string T() {
    string left = F();
    while (cursor < expr.length() && (expr[cursor] == '*' || expr[cursor] == '/')) {
        char op = expr[cursor++];
        string right = F();
        string result = newTemp();
        cout << "  " << result << " = " << left << " " << op << " " << right << "\n";
        left = result;
    }
    return left;
}

string F() {
    if (cursor < expr.length() && isalnum(expr[cursor])) {
        string id = "";
        while (cursor < expr.length() && isalnum(expr[cursor])) {
            id += expr[cursor++];
        }
        return id;
    }
    else if (cursor < expr.length() && expr[cursor] == '(') {
        cursor++;
        string temp = E();
        cursor++;
        return temp;
    }
    return "";
}

void generateTAC(string input) {
    expr = "";
    for (char c : input) if (!isspace(c)) expr += c; // Remove spaces

    cout << "Expression: " << input << "\n";

    cursor = 0;
    tempCount = 1;

    string target = "";

    int eqPos = expr.find('=');
    if (eqPos != string::npos) {
        target = expr.substr(0, eqPos);
        cursor = eqPos + 1;
    }

    string finalResult = E();
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
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            generateTAC(line);
        }
    }
    return 0;
}
