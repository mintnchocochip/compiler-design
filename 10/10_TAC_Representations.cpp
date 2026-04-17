#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

struct Instruction {
    string op, arg1, arg2, result;
};

int main() {
    ifstream file("tac_input.txt");
    if (!file) { cout << "Missing tac_input.txt\n"; return 1; }

    vector<Instruction> stmts;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string num, tok1, tok2, tok3, tok4, tok5, tok6;

        ss >> num;
        ss >> tok1; // "S1" or "if"

        if (tok1 == "if") {
            // if j <= 10 goto 3
            ss >> tok2 >> tok3 >> tok4 >> tok5 >> tok6;
            // tok2=j, tok3=<=, tok4=10, tok5=goto, tok6=(3)

            stmts.push_back({tok3, tok2, tok4, tok6}); // (<=, j, 10, 3)
        }
        else {
            // It's an assignment. tok1 is the variable (e.g., "S1", "result")
            ss >> tok2 >> tok3; // tok2 is ":=", tok3 is the first argument
            // Array access check: x[S1]
            if (tok3.find('[') != string::npos) {
                int pos = tok3.find('[');
                string arrayName = tok3.substr(0, pos);
                string index = tok3.substr(pos + 1, tok3.size() - pos - 2);
                stmts.push_back({"[]", arrayName, index, tok1});
            }
            // Binary operation check: 4 * i
            else if (ss >> tok4 >> tok5) {
                // tok4 is operator "*", tok5 is second arg "i"
                stmts.push_back({tok4, tok3, tok5, tok1});
            }
            // Unary assignment: j := S7
            else {
                stmts.push_back({"=", tok3, "", tok1});
            }
        }
    }

    cout << "--- 1. QUADRUPLES ---\n";
    cout << "Loc\tOp\tArg1\tArg2\tResult\n";
    for (int i = 0; i < stmts.size(); i++) {
        cout << "(" << i+1 << ")\t" << stmts[i].op << "\t" << stmts[i].arg1
             << "\t" << stmts[i].arg2 << "\t" << stmts[i].result << "\n";
    }

    // Map temporary variables to their instruction line number
    map<string, string> tMap;
    for (int i = 0; i < stmts.size(); i++) {
        if (stmts[i].result[0] == 'S' && isdigit(stmts[i].result[1])) {
            tMap[stmts[i].result] = "(" + to_string(i + 1) + ")";
        }
    }

    cout << "\n--- TRIPLES ---\n";
    cout << "Loc\tOp\tArg1\tArg2\n";
    for (int i = 0; i < stmts.size(); i++) {
        string a1 = tMap.count(stmts[i].arg1) ? tMap[stmts[i].arg1] : stmts[i].arg1;
        string a2 = tMap.count(stmts[i].arg2) ? tMap[stmts[i].arg2] : stmts[i].arg2;

        if (stmts[i].op == "=") {
            cout << "(" << i+1 << ")\t=\t" << stmts[i].result << "\t" << a1 << "\n";
        }
        else if (stmts[i].op == "<=" || stmts[i].op == "==" || stmts[i].op == "!=") {
            cout << "(" << i+1 << ")\tif" << stmts[i].op << "\t" << a1 << "\t" << a2 << "\t(goto " << stmts[i].result << ")\n";
        }
        else {
            cout << "(" << i+1 << ")\t" << stmts[i].op << "\t" << a1 << "\t" << a2 << "\n";
        }
    }

    cout << "\n--- INDIRECT TRIPLES ---\n";
    cout << "Ptr\tStatement\n";
    int base_address = 101;
    for (int i = 0; i < stmts.size(); i++) {
        cout << base_address + i << "\t(" << i+1 << ")\n";
    }

    return 0;
}
