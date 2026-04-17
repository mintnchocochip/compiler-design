#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

int main() {

    ifstream file("dfa_input.txt");

    int numStates;
    file >> numStates;

    string startState;
    file >> startState;

    int numSymbols;
    file >> numSymbols;

    vector<char> alphabet(numSymbols);
    for (int i = 0; i < numSymbols; i++) {
        file >> alphabet[i];
    }

    vector<string> states;
    states.push_back(startState);
    for (int i = 1; i < numStates; i++) {
        string s;
        file >> s;
        states.push_back(s);
    }


    // transitions[{CurrentState, Symbol}] = NextState
    map<pair<string, char>, string> transitions;

    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < numSymbols; j++) {
            string nextState;
            file >> nextState;
            transitions[{states[i], alphabet[j]}] = nextState;
        }
    }

    int numFinal;
    file >> numFinal;
    vector<string> finalStates(numFinal);
    for (int i = 0; i < numFinal; i++) {
        file >> finalStates[i];
    }

    int numStrings;
    file >> numStrings;

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

            string nextState = transitions[{currentState, c}];

            if (nextState == "-" || nextState == "") {
                cout << "  State [" << currentState << "] --(" << c << ")--> [UNDEFINED/DEAD STATE]\n";
                rejectedEarly = true;
                break;
            }

            cout << "  State [" << currentState << "] --(" << c << ")--> State [" << nextState << "]\n";
            currentState = nextState; // Move to the next state
        }


        if (rejectedEarly) {
            cout << "Result: REJECTED (Invalid symbol or undefined transition)\n";
        } else {
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
