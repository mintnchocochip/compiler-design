#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int main() {
    // 1. Open the input file
    ifstream file("dfa_input.txt");
    // 2. Read Number of States and Start State
    int numStates;
    file >> numStates;

    string startState;
    file >> startState;

    // 3. Read Alphabet
    int numSymbols;
    file >> numSymbols;

    vector<char> alphabet(numSymbols);
    for (int i = 0; i < numSymbols; i++) {
        file >> alphabet[i];
    }

    // 4. Read all states (Start state + the rest)
    vector<string> states;
    states.push_back(startState);
    for (int i = 1; i < numStates; i++) {
        string s;
        file >> s;
        states.push_back(s);
    }

    // 5. Read Transition Table
    // We use a Map to easily look up: transitions[{CurrentState, Symbol}] = NextState
    map<pair<string, char>, string> transitions;

    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numSymbols; j++) {
            string nextState;
            file >> nextState;
            transitions[{states[i], alphabet[j]}] = nextState;
        }
    }

    // 6. Read Final States
    int numFinal;
    file >> numFinal;
    vector<string> finalStates(numFinal);
    for (int i = 0; i < numFinal; i++) {
        file >> finalStates[i];
    }

    // 7. Read and Process Test Strings
    int numStrings;
    file >> numStrings;

    cout << "--- DFA SIMULATION RESULTS ---\n";

    for (int i = 0; i < numStrings; i++) {
        string word;
        file >> word;

        cout << "\nProcessing String: '" << word << "'\n";

        string currentState = startState;
        bool rejectedEarly = false;

        // Process the string symbol by symbol
        for (char c : word) {
            // Validate: Is the symbol in our alphabet?
            bool validSymbol = false;
            for (char a : alphabet) {
                if (a == c) validSymbol = true;
            }

            if (!validSymbol) {
                cout << "  Error: Symbol '" << c << "' does not belong to the alphabet!\n";
                rejectedEarly = true;
                break;
            }

            // Look up the next state in our map
            string nextState = transitions[{currentState, c}];

            // Validate: Is the transition undefined? (Represented by "-")
            if (nextState == "-" || nextState == "") {
                cout << "  State [" << currentState << "] --(" << c << ")--> [UNDEFINED/DEAD STATE]\n";
                rejectedEarly = true;
                break;
            }

            cout << "  State [" << currentState << "] --(" << c << ")--> State [" << nextState << "]\n";
            currentState = nextState; // Move to the next state
        }

        // Print Final Decision
        if (rejectedEarly) {
            cout << "Result: REJECTED (Invalid symbol or undefined transition)\n";
        } else {
            // Check if the state we ended up in is one of our final states
            bool isFinal = false;
            for (string f : finalStates) {
                if (f == currentState) isFinal = true;
            }

            if (isFinal) {
                cout << "Result: ACCEPTED (Ended in final state [" << currentState << "])\n";
            } else {
                cout << "Result: REJECTED (Ended in non-final state [" << currentState << "])\n";
            }
        }
    }

    return 0;
}
