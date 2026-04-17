#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <stack>

using namespace std;

struct Rule { string lhs; int rhs_len; string full_rule; };

int main() {
    ifstream file("lr_input.txt");
    if (!file) { cout << "Missing lr_input.txt\n"; return 1; }

    int numRules; file >> numRules;
    vector<Rule> rules(numRules + 1);

    string line;
    getline(file, line);
    for (int i = 1; i <= numRules; i++) {
        getline(file, line);
        stringstream ss(line);
        string lhs, arrow, token;

        ss >> lhs >> arrow;

        int len = 0;
        while (ss >> token) len++;
        rules[i] = {lhs, len, line}; //need to give all things seperated by space
    }

    map<pair<int, string>, string> table;
    int numGoto; file >> numGoto;
    for (int i = 0; i < numGoto; i++) {
        int state; string nonTerm, nextState;
        file >> state >> nonTerm >> nextState;
        table[{state, nonTerm}] = nextState;
    }
    int numAction; file >> numAction;
    for (int i = 0; i < numAction; i++) {
        int state; string term, action;
        file >> state >> term >> action;
        table[{state, term}] = action;
    }

    vector<string> input; string token;
    while (file >> token) input.push_back(token);

    if (input.empty()) { cout << "No input string found.\n"; return 1; }

    stack<int> st; st.push(0);
    int cursor = 0;

    while (cursor < input.size()) {
        int state = st.top();
        string sym = input[cursor];
        string action = table[{state, sym}];

        cout << "State: " << state << "\t| Input: " << sym << "\t| Action: " << (action.empty() ? "Error" : action) << "\n";

        if (action == "") { cout << "\nREJECTED (Syntax Error)\n"; break; }
        if (action == "ACC") { cout << "\nACCEPTED!\n"; break; }

        if (action[0] == 'S') {
            st.push(stoi(action.substr(1)));
            cursor++;
        }
        else if (action[0] == 'R') {
            Rule r = rules[stoi(action.substr(1))];

            cout << "\t\t\t\t\t\t(Reduce by: " << r.full_rule << ")\n";
            for (int i = 0; i < r.rhs_len; i++) st.pop();
            string gotoState = table[{st.top(), r.lhs}];
            if (gotoState == "") { cout << "\nREJECTED (No GOTO)\n"; break; }

            st.push(stoi(gotoState));
            cout << "\t\t\t\t\t\t(GOTO State " << gotoState << ")\n";
        }
    }
    return 0;
}
