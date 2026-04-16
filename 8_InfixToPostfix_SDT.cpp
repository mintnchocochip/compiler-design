#include <iostream>
#include <string>
#include <cctype>

using namespace std;

string expr;
int cursor;
string postfix;

void E();
void T();
void F();

void E() {
    T();
    while (cursor < expr.length() && (expr[cursor] == '+' || expr[cursor] == '-')) {
        char op = expr[cursor];
        cursor++;
        T();
        postfix += op;
        postfix += " ";
    }
}

void T() {
    F();
    while (cursor < expr.length() && (expr[cursor] == '*' || expr[cursor] == '/')) {
        char op = expr[cursor];
        cursor++;
        F();
        postfix += op;
        postfix += " ";
    }
}

void F() {
    if (cursor < expr.length() && isalnum(expr[cursor])) {

        string id = "";
        while (cursor < expr.length() && isalnum(expr[cursor])) {
            id += expr[cursor++];
        }
        postfix += id + " ";
    }
    else if (cursor < expr.length() && expr[cursor] == '(') {
        cursor++;
        E();
        cursor++;
    }
}

void translate(string input) {
    expr = input;
    cursor = 0;
    postfix = "";
    E();
    cout << "Infix:   " << input << "\n";
    cout << "Postfix: " << postfix << "\n\n";
}

int main() {
    int n;
    cin>>n;

    for(int i = 0; i < n; i++){
        string exp;
        cin >> exp;
        translate(exp);
    }

    return 0;
}
