#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cctype>

using namespace std;

map<char, vector<string>> rules;

// Recursive Backtracking Function to derive the target string
bool derive(string current, string target, vector<string>& path) {
    path.push_back(current); // Record current step

    if (current == target) return true; // Success!

    // Find the first Non-Terminal (uppercase letter) in the current string
    int nt_idx = -1;
    for (int i = 0; i < current.length(); i++) {
        if (isupper(current[i])) {
            nt_idx = i;
            break;
        }
        // Optimization: If the terminals don't match the target string prefix, stop trying (prune)
        if (i >= target.length() || current[i] != target[i]) {
            path.pop_back();
            return false;
        }
    }

    // If no Non-Terminals are left, we can't derive further
    if (nt_idx == -1) {
        path.pop_back();
        return false;
    }

    // Try replacing the Non-Terminal with each of its RHS rules
    char nt = current[nt_idx];
    for (string rhs : rules[nt]) {
        string replacement = (rhs == "e") ? "" : rhs;
        string next_str = current.substr(0, nt_idx) + replacement
            + current.substr(nt_idx + 1);

        // Recursive call
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

    cout << "--- CFG Derivation Checker ---\n\n";
    cout << "Target String: " << target << "\n\n";

    vector<string> path;
    if (derive(string(1, startSymbol), target, path)) {
        cout << "Result: ACCEPTED! The string CAN be derived.\n\n";
        cout << "Derivation Steps:\n";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i] << (i == path.size() - 1 ? "" : " => ");
        }
        cout << "\n";
    } else {
        cout << "Result: REJECTED! The string CANNOT be derived.\n";
    }

    return 0;
}
