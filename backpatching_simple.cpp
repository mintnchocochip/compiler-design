#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Instruction {
    string op, arg1, arg2, res;
    int target;
    bool isJump;
};

// We use 1-based indexing so line numbers match standard TAC format
vector<Instruction> tac(1);

// Helper to get the next instruction line number
int nextInstr() {
    return tac.size();
}

// 1. Emit a normal expression
int emit(string op, string arg1, string arg2, string res) {
    tac.push_back({op, arg1, arg2, res, -1, false});
    return tac.size() - 1;
}

// 2. Emit a conditional jump (target is incomplete / -1)
int emit_if(string op, string x, string y) {
    tac.push_back({op, x, y, "goto", -1, true});
    return tac.size() - 1;
}

// 3. Emit an unconditional jump (target is incomplete / -1)
int emit_goto() {
    tac.push_back({"goto", "", "", "", -1, true});
    return tac.size() - 1;
}

// BACKPATCHING: Fill in the missing target line numbers
void backpatch(const vector<int>& list, int targetLine) {
    for (int line : list) {
        tac[line].target = targetLine;
    }
}

// Merge two lists of line numbers
vector<int> mergeLists(vector<int> l1, vector<int> l2) {
    vector<int> res = l1;
    res.insert(res.end(), l2.begin(), l2.end());
    return res;
}

// --- PRINTING FUNCTIONS ---
void printTAC() {
    cout << "\n--- Three Address Code ---\n";
    for (int i = 1; i < tac.size(); i++) {
        if (tac[i].isJump) {
            if (tac[i].op == "goto") 
                cout << i << ": goto " << tac[i].target << "\n";
            else 
                cout << i << ": if " << tac[i].arg1 << " " << tac[i].op << " " << tac[i].arg2 << " goto " << tac[i].target << "\n";
        } else if (tac[i].op == ":=") {
            cout << i << ": " << tac[i].res << " := " << tac[i].arg1 << "\n";
        } else if (tac[i].op == "=[]") {
            cout << i << ": " << tac[i].res << " := " << tac[i].arg1 << "[" << tac[i].arg2 << "]\n";
        } else {
            cout << i << ": " << tac[i].res << " := " << tac[i].arg1 << " " << tac[i].op << " " << tac[i].arg2 << "\n";
        }
    }
}

void printQuadruples() {
    cout << "\n--- Quadruple Table ---\n";
    cout << "Idx\tOp\tArg1\tArg2\tRes\n";
    for (int i = 1; i < tac.size(); i++) {
        cout << i << "\t" << tac[i].op << "\t" << tac[i].arg1 << "\t" << tac[i].arg2 << "\t";
        if (tac[i].isJump) cout << tac[i].target << " (Target)\n";
        else cout << tac[i].res << "\n";
    }
}

void printTriples() {
    cout << "\n--- Triple Table ---\n";
    cout << "Idx\tOp\tArg1\tArg2\n";
    for (int i = 1; i < tac.size(); i++) {
        cout << i << "\t" << tac[i].op << "\t" << tac[i].arg1 << "\t";
        if (tac[i].isJump) cout << tac[i].target << " (Target)\n";
        else cout << tac[i].arg2 << "\n";
    }
}

void printIndirectTriples() {
    cout << "\n--- Indirect Triples ---\n";
    cout << "Ptr\t->\tIdx\n";
    for (int i = 1; i < tac.size(); i++) {
        cout << i << "\t->\t" << i << "\n";
    }
}

// --- DEMOS ---
void buildGiven13Statements() {
    emit("*", "4", "i", "S1");
    emit("=[]", "x", "S1", "S2");
    emit("*", "4", "j", "S3");
    emit("=[]", "y", "S3", "S4");
    emit("*", "S2", "S4", "S5");
    emit("+", "result", "S5", "S6");
    emit(":=", "S6", "", "result");
    emit("+", "j", "1", "S7");
    emit(":=", "S7", "", "j");
    
    int i10 = emit_if("<=", "j", "10"); 
    tac[i10].target = 3; // Hardcoded loop jump
    
    emit("+", "i", "1", "S8");
    emit(":=", "S8", "", "i");
    
    int i13 = emit_if("<=", "i", "20"); 
    tac[i13].target = 1; // Hardcoded loop jump
}

void backpatchDemo() {
    // Clear previous TAC for the demo
    tac.clear();
    tac.resize(1); // Keep 1-based index

    cout << "\n\n=== BACKPATCHING DEMO ===";
    cout << "\nCode: if (a < b) then x = 1 else x = 2\n";

    // 1. Condition
    int if_instr = emit_if("<", "a", "b");
    vector<int> trueList = {if_instr};

    // 2. False jump (skip "then" block)
    int skip_then = emit_goto();
    vector<int> falseList = {skip_then};

    // 3. "Then" block
    int M1 = nextInstr();
    emit(":=", "1", "", "x");

    // 4. Skip "else" block
    int skip_else = emit_goto();
    vector<int> nextList = {skip_else};

    // 5. "Else" block
    int M2 = nextInstr();
    emit(":=", "2", "", "x");

    // 6. Backpatch targets!
    backpatch(trueList, M1);
    backpatch(falseList, M2);
    backpatch(nextList, nextInstr()); // jump to end

    // Print results
    printTAC();
    printQuadruples();
}

int main() {
    buildGiven13Statements();
    printTAC();
    printQuadruples();
    printTriples();
    printIndirectTriples();

    backpatchDemo();

    return 0;
}