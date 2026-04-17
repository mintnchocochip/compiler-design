#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cctype>

using namespace std;

map<char, vector<string>> rules;

bool derive(string current, string target, vector<string>& path) {
    path.push_back(current);

    if (current == target) return true;

    int nt_idx = -1;
    for (int i = 0; i < current.length(); i++) {
        if (isupper(current[i])) {
            nt_idx = i;
            break;
        }
        if (i >= target.length() || current[i] != target[i]) {
            path.pop_back();
            return false;
        }
    }

    if (nt_idx == -1) {
        path.pop_back();
        return false;
    }

    char nt = current[nt_idx];
    for (string rhs : rules[nt]) {
        string replacement = (rhs == "e") ? "" : rhs;
        string next_str = current.substr(0, nt_idx) + replacement
            + current.substr(nt_idx + 1);

        if (derive(next_str, target, path)) {
            return true;
        }
    }

    path.pop_back();
    return false;
}

int main() {
    ifstream file("cfg_input.txt");

    int numRules;
    file >> numRules;

    char startSymbol = '\0';
    for (int i = 0; i < numRules; i++) {
        string lhs, arrow, rhs;
        file >> lhs >> arrow >> rhs;

        if (i == 0) startSymbol = lhs[0];
        rules[lhs[0]].push_back(rhs);
    }

    string target;
    file >> target;
    cout << "Target String: " << target << "\n\n";

    vector<string> path;
    if (derive(string(1, startSymbol), target, path)) {
        cout << "Result: ACCEPTED! The string CAN be derived.\n\n";
        // cout << "Derivation Steps:\n";
        // for (int i = 0; i < path.size(); i++) {
        //     cout << path[i] << (i == path.size() - 1 ? "" : " => ");
        // }
        // cout << "\n";
    } else {
        cout << "Result: REJECTED! The string CANNOT be derived.\n";
    }

    return 0;
}
