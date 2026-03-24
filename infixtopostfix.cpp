#include <bits/stdc++.h>
using namespace std;

string inp;
int pos;

void E();  void E_();
void T();  void T_();
void F();

void E()  { T(); E_(); }
void T()  { F(); T_(); }

void E_() {
    if (pos < (int)inp.size() && (inp[pos] == '+' || inp[pos] == '-')) {
        char op = inp[pos++];
        T();
        cout << op << " ";
        E_();
    }
}

void T_() {
    if (pos < (int)inp.size() && (inp[pos] == '*' || inp[pos] == '/')) {
        char op = inp[pos++];
        F();
        cout << op << " ";
        T_();
    }
}

void F() {
    if (inp[pos] == '(') {
        pos++;
        E();
        pos++;
    } else {
        cout << inp[pos++] << " ";
    }
}

int main() {
    int n;
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);

        inp = "";
        for (char c : line) if (c != ' ') inp += c;
        pos = 0;

        cout << "Infix   : " << line << "\n";
        cout << "Postfix : ";
        E();
        cout << "\n\n";
    }

    return 0;
}
