#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

map<char, vector<string>> rules;
map<char, set<char>> FIRST;
map<char, set<char>> FOLLOW;
vector<char> nonTerminals;

// Split string by '|'
vector<string> split(const string& s) {
    vector<string> res;
    string temp = "";
    for (char c : s) {
        if (c == '|') {
            res.push_back(temp);
            temp = "";
        } else {
            temp += c;
        }
    }
    res.push_back(temp);
    return res;
}

bool isTerminal(char c) {
    return !isupper(c); // Lowercase letters and symbols are terminals
}

// Compute FIRST set of a string (like the right hand side of a rule)
set<char> firstOfString(string s) {
    set<char> res;
    if (s.empty() || s == "*") {
        res.insert('*'); // '*' represents epsilon
        return res;
    }

    for (char c : s) {
        if (isTerminal(c)) {
            res.insert(c);
            return res;
        }

        // It's a non-terminal
        bool hasEpsilon = false;
        for (char f : FIRST[c]) {
            if (f == '*') hasEpsilon = true;
            else res.insert(f);
        }

        // If this non-terminal can't be epsilon, we stop checking the rest of the string
        if (!hasEpsilon) return res;
    }

    // If all characters can be epsilon, the whole string can be epsilon
    res.insert('*');
    return res;
}

// Fixed-point iteration to compute FIRST sets
void computeFirst() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (char A : nonTerminals) {
            for (string rhs : rules[A]) {
                set<char> f = firstOfString(rhs);
                for (char c : f) {
                    if (FIRST[A].insert(c).second) {
                        changed = true; // A new symbol was added
                    }
                }
            }
        }
    }
}

// Fixed-point iteration to compute FOLLOW sets
void computeFollow(char startSymbol) {
    FOLLOW[startSymbol].insert('$');

    bool changed = true;
    while (changed) {
        changed = false;
        for (char A : nonTerminals) {
            for (string rhs : rules[A]) {
                if (rhs == "*") continue;

                for (size_t i = 0; i < rhs.size(); i++) {
                    char B = rhs[i];
                    if (isTerminal(B)) continue;

                    // Get FIRST of what comes after B
                    string rest = rhs.substr(i + 1);
                    set<char> f = firstOfString(rest);

                    // Add all non-epsilon symbols to FOLLOW(B)
                    for (char c : f) {
                        if (c != '*' && FOLLOW[B].insert(c).second) {
                            changed = true;
                        }
                    }

                    // If 'rest' can be epsilon (or B is the last character)
                    // Then FOLLOW(A) should be added to FOLLOW(B)
                    if (f.count('*') || i == rhs.size() - 1) {
                        for (char c : FOLLOW[A]) {
                            if (FOLLOW[B].insert(c).second) {
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool checkLeftRecursion() {
    for (char A : nonTerminals) {
        for (string rhs : rules[A]) {
            if (!rhs.empty() && rhs[0] == A) return true;
        }
    }
    return false;
}

bool checkLL1() {
    for (char A : nonTerminals) {
        set<char> seen;
        for (string rhs : rules[A]) {
            set<char> f = firstOfString(rhs);
            for (char c : f) {
                if (seen.count(c)) return false; // Collision found!
                seen.insert(c);
            }
        }
    }
    return true;
}

int main() {
    int n;
    cout << "Enter number of rules: ";
    if (!(cin >> n)) return 0;

    cout << "Enter rules (e.g. S aSb|*):\n";
    for (int i = 0; i < n; i++) {
        char nt;
        string rhs;
        cin >> nt >> rhs;
        
        nonTerminals.push_back(nt);
        rules[nt] = split(rhs);
    }

    if (nonTerminals.empty()) {
        cout << "No rules provided.\n";
        return 0;
    }

    char startSymbol = nonTerminals[0];

    if (checkLeftRecursion()) {
        cout << "Grammar has left recursion - not LL(1)\n";
        return 1;
    }

    computeFirst();
    computeFollow(startSymbol);

    cout << "\nFIRST sets:\n";
    for (char nt : nonTerminals) {
        cout << "FIRST(" << nt << ") = { ";
        for (char c : FIRST[nt]) cout << c << " ";
        cout << "}\n";
    }

    cout << "\nFOLLOW sets:\n";
    for (char nt : nonTerminals) {
        cout << "FOLLOW(" << nt << ") = { ";
        for (char c : FOLLOW[nt]) cout << c << " ";
        cout << "}\n";
    }

    cout << "\nLL(1)? " << (checkLL1() ? "Yes" : "No") << "\n";

    return 0;
}