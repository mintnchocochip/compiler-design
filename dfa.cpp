#include <bits/stdc++.h>
using namespace std;

struct tt {
    string current;
    vector<string> nextstates;
};

class dfa {
public:
    int nostates;
    vector<string> states;
    vector<char> alphabet;
    string initial;
    vector<string> finalstates;
    string currentstate;
    vector<tt> trans;
    bool flag = false;
    string rejectionReason;

    dfa(int totalStates, const string& init) {
        nostates = max(totalStates, 0);
        initial = init;
        currentstate = initial;
        states.push_back(initial);
    }

    bool isFinal() const {
        return find(finalstates.begin(), finalstates.end(), currentstate) != finalstates.end();
    }

    void printalpha() const {
        for (char c : alphabet) {
            cout << c << " ";
        }
        cout << endl;
    }

    void getalpha() {
        alphabet.clear();
        int n;
        cout << "Enter the number of alphabets" << endl;
        cin >> n;
        if (n <= 0) {
            cout << "Alphabet must contain at least one symbol." << endl;
            return;
        }
        unordered_set<char> seen;
        cout << "Enter the alphabet symbols" << endl;
        for (int i = 0; i < n; ++i) {
            char c;
            cin >> c;
            if (seen.insert(c).second) {
                alphabet.push_back(c);
            } else {
                cout << "Duplicate symbol '" << c << "' ignored." << endl;
                --i;
            }
        }
    }

    void getstates() {
        states.clear();
        states.push_back(initial);
        cout << "Enter the number of states (excluding the initial)" << endl;
        int n;
        cin >> n;
        cout << "Enter the remaining state names" << endl;
        while (n-- > 0) {
            string name;
            cin >> name;
            if (!stateExists(name)) {
                states.push_back(name);
            } else {
                cout << "State '" << name << "' already present. Enter a different name." << endl;
                ++n;
            }
        }
        nostates = static_cast<int>(states.size());
    }

    void getTransitiontable() {
        trans.clear();
        unordered_set<string> captured;
        cout << "Enter the transition table rows using the states defined" << endl;
        while (static_cast<int>(trans.size()) < nostates) {
            tt temp;
            cout << "Enter current state" << endl;
            cin >> temp.current;
            if (!stateExists(temp.current)) {
                cout << "State '" << temp.current << "' is not defined. Re-enter the row." << endl;
                continue;
            }
            if (!captured.insert(temp.current).second) {
                cout << "Transition row for state '" << temp.current << "' already provided. Re-enter a different state." << endl;
                continue;
            }
            cout << "Enter the next states (newline separator, in order of entered alphabet)" << endl;
            vector<string> row;
            for (size_t i = 0; i < alphabet.size(); ++i) {
                string next;
                cin >> next;
                row.push_back(next);
            }
            temp.nextstates = row;
            trans.push_back(temp);
        }
    }

    int find(char a) const {
        for (size_t idx = 0; idx < alphabet.size(); ++idx) {
            if (alphabet[idx] == a) {
                return static_cast<int>(idx);
            }
        }
        return -1;
    }

    void reset() {
        currentstate = initial;
        flag = false;
        rejectionReason.clear();
    }

    bool stateExists(const string& name) const {
        return find(states.begin(), states.end(), name) != states.end();
    }

    void iterate(char alp) {
        vector<string> possible;
        bool rowFound = false;
        for (const tt& tra : trans) {
            if (currentstate == tra.current) {
                possible = tra.nextstates;
                rowFound = true;
                break;
            }
        }
        if (!rowFound) {
            flag = true;
            rejectionReason = "No transition row defined for state " + currentstate;
            return;
        }
        int idx = find(alp);
        if (idx == -1) {
            flag = true;
            rejectionReason = string("Encountered symbol '") + alp + "' not in alphabet";
            return;
        }
        if (idx >= static_cast<int>(possible.size())) {
            flag = true;
            rejectionReason = "Transition row for state " + currentstate + " is incomplete";
            return;
        }
        const string& next = possible[idx];
        if (next == "-" || next.empty()) {
            flag = true;
            rejectionReason = string("Undefined transition from state ") + currentstate + " on symbol '" + alp + "'";
            return;
        }
        if (!stateExists(next)) {
            flag = true;
            rejectionReason = "Next state '" + next + "' is not defined";
            return;
        }
        currentstate = next;
    }
};

int main() {
    cout << "Enter total number of states (including the initial state)" << endl;
    int totalStates;
    cin >> totalStates;

    cout << "Enter the initial state" << endl;
    string initState;
    cin >> initState;

    dfa machine(totalStates, initState);

    machine.getalpha();
    if (machine.alphabet.empty()) {
        cout << "Exiting: alphabet not provided correctly." << endl;
        return 1;
    }

    machine.getstates();

    machine.getTransitiontable();

    cout << "Enter the number of final states" << endl;
    int finalCount;
    cin >> finalCount;
    machine.finalstates.clear();
    cout << "Enter the final states" << endl;
    while (finalCount-- > 0) {
        string fs;
        cin >> fs;
        if (!machine.stateExists(fs)) {
            cout << "State '" << fs << "' is not defined. Enter a valid final state." << endl;
            ++finalCount;
            continue;
        }
        if (find(machine.finalstates.begin(), machine.finalstates.end(), fs) != machine.finalstates.end()) {
            cout << "State '" << fs << "' already marked final. Enter a different state." << endl;
            ++finalCount;
            continue;
        }
        machine.finalstates.push_back(fs);
    }

    cout << "Enter the number of strings to validate" << endl;
    int testCount;
    cin >> testCount;
    vector<string> testStrings;
    cout << "Enter each test string on a new line" << endl;
    for (int i = 0; i < testCount; ++i) {
        string sample;
        cin >> sample;
        testStrings.push_back(sample);
    }

    for (const string& input : testStrings) {
        cout << "\nTesting string: " << input << endl;
        machine.reset();

        for (char c : input) {
            machine.iterate(c);
            if (machine.flag) {
                break;
            }
        }

        if (machine.flag) {
            cout << "The string is REJECTED (Stopped at state: " << machine.currentstate << ")" << endl;
            if (!machine.rejectionReason.empty()) {
                cout << "Reason: " << machine.rejectionReason << endl;
            }
        } else if (machine.isFinal()) {
            cout << "The string is ACCEPTED (Final state: " << machine.currentstate << ")" << endl;
        } else {
            cout << "The string is REJECTED (Final state: " << machine.currentstate << ")" << endl;
            cout << "Reason: The automaton halted in a non-final state" << endl;
        }
    }

    return 0;
}