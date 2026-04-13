#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct TransitionRow {
    string state;
    vector<string> nextState;   // nextState[symbolIndex]
};

class DFA {
public:
    vector<string> states;
    vector<char> alphabet;
    vector<TransitionRow> transitions;

    string startState;
    string currentState;
    vector<string> finalStates;

    void reset() { currentState = startState; }

    int alphabetIndex(char symbol) const {
        for (int i = 0; i < (int)alphabet.size(); i++)
            if (alphabet[i] == symbol) return i;
        return -1;
    }

    bool isFinal(const string& state) const {
        for (int i = 0; i < (int)finalStates.size(); i++)
            if (finalStates[i] == state) return true;
        return false;
    }

    TransitionRow* findRow(const string& state) {
        for (int i = 0; i < (int)transitions.size(); i++)
            if (transitions[i].state == state) return &transitions[i];
        return nullptr;
    }

    bool move(char symbol, string& error) {
        int symIndex = alphabetIndex(symbol);
        if (symIndex == -1) {
            error = string("Symbol '") + symbol + "' not in alphabet.";
            return false;
        }

        TransitionRow* row = findRow(currentState);
        if (!row) {
            error = "No transition row for state '" + currentState + "'.";
            return false;
        }

        string to = row->nextState[symIndex];
        if (to == "-" || to == "") {
            error = "Transition undefined (dead state).";
            return false;
        }

        currentState = to;
        return true;
    }
};

int main() {
    DFA dfa;

    int numStates;
    cout << "Enter total number of states:\n";
    cin >> numStates;

    cout << "Enter initial state name:\n";
    cin >> dfa.startState;
    dfa.states.push_back(dfa.startState);

    int numSymbols;
    cout << "Enter number of alphabet symbols:\n";
    cin >> numSymbols;

    cout << "Enter the " << numSymbols << " alphabet symbols (space separated):\n";
    for (int i = 0; i < numSymbols; i++) {
        char c;
        cin >> c;
        dfa.alphabet.push_back(c);
    }

    cout << "Enter the names of the other " << (numStates - 1) << " states:\n";
    for (int i = 1; i < numStates; i++) {
        string s;
        cin >> s;
        dfa.states.push_back(s);
    }

    cout << "--- Transition Table Definition ---\n";
    cout << "For acceptable transitions, enter the Target State Name.\n";
    cout << "For undefined/dead transitions, enter '-'\n";

    for (int i = 0; i < numStates; i++) {
        TransitionRow row;
        row.state = dfa.states[i];
        row.nextState.resize(numSymbols);

        for (int j = 0; j < numSymbols; j++) {
            cout << "From State '" << row.state << "' on symbol '" << dfa.alphabet[j] << "' -> ";
            cin >> row.nextState[j];
        }
        dfa.transitions.push_back(row);
    }

    int numFinal;
    cout << "Enter number of final states:\n";
    cin >> numFinal;

    cout << "Enter the " << numFinal << " final state names:\n";
    for (int i = 0; i < numFinal; i++) {
        string s;
        cin >> s;
        dfa.finalStates.push_back(s);
    }

    int numStrings;
    cout << "Enter number of strings to test:\n";
    cin >> numStrings;

    cout << "Enter the strings:\n";
    for (int t = 0; t < numStrings; t++) {
        string input;
        cin >> input;

        dfa.reset();
        cout << "\nProcessing string: " << input << " ...\n";
        cout << "  Start: " << dfa.currentState << "\n";

        bool ok = true;
        for (char ch : input) {
            string err;
            if (!dfa.move(ch, err)) {
                cout << "  Error: " << err << "\n";
                cout << "Result: REJECTED\n";
                ok = false;
                break;
            }
            cout << "  --(" << ch << ")--> " << dfa.currentState << "\n";
        }

        if (ok) {
            if (dfa.isFinal(dfa.currentState))
                cout << "Result: ACCEPTED (Ended in final state '" << dfa.currentState << "')\n";
            else
                cout << "Result: REJECTED (Ended in non-final state '" << dfa.currentState << "')\n";
        }
    }

    return 0;
}
