#include <bits/stdc++.h>
using namespace std;

struct Production {
    char lhs;
    string rhs;
};

struct Item {
    int prod_id;
    int dot;

    bool operator<(const Item& o) const {
        if (prod_id != o.prod_id) return prod_id < o.prod_id;
        return dot < o.dot;
    }
};

struct NonTerminal {
    char nt;
    vector<string> prod;
    set<char> first;
    set<char> follow;
};

vector<Production> prods;
map<char, vector<int>> by_lhs;
set<char> terminals, nonterminals;
char start_symbol, augmented_start;
map<char, set<char>> FIRST, FOLLOW;

bool isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

set<Item> closureSet(set<Item> I) {
    queue<Item> q;
    for (auto x : I) q.push(x);

    while (!q.empty()) {
        Item it = q.front();
        q.pop();

        string rhs = prods[it.prod_id].rhs;
        if (it.dot >= (int)rhs.size()) continue;

        char X = rhs[it.dot];
        if (!isNonTerminal(X)) continue;

        for (int pid : by_lhs[X]) {
            Item ni{pid, 0};
            if (!I.count(ni)) {
                I.insert(ni);
                q.push(ni);
            }
        }
    }
    return I;
}

set<Item> goToSet(const set<Item>& I, char X) {
    set<Item> J;
    for (auto it : I) {
        string rhs = prods[it.prod_id].rhs;
        if (it.dot < (int)rhs.size() && rhs[it.dot] == X) {
            J.insert({it.prod_id, it.dot + 1});
        }
    }
    if (J.empty()) return J;
    return closureSet(J);
}

void computeFirst() {
    for (char nt : nonterminals) FIRST[nt] = {};

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto& p : prods) {
            char A = p.lhs;
            string alpha = p.rhs;

            if (alpha == "#") {
                if (!FIRST[A].count('#')) {
                    FIRST[A].insert('#');
                    changed = true;
                }
                continue;
            }

            bool allNullable = true;
            for (char X : alpha) {
                if (!isNonTerminal(X)) {
                    if (!FIRST[A].count(X)) {
                        FIRST[A].insert(X);
                        changed = true;
                    }
                    allNullable = false;
                    break;
                } else {
                    for (char c : FIRST[X]) {
                        if (c == '#') continue;
                        if (!FIRST[A].count(c)) {
                            FIRST[A].insert(c);
                            changed = true;
                        }
                    }
                    if (!FIRST[X].count('#')) {
                        allNullable = false;
                        break;
                    }
                }
            }
            if (allNullable) {
                if (!FIRST[A].count('#')) {
                    FIRST[A].insert('#');
                    changed = true;
                }
            }
        }
    }
}

set<char> firstOfString(const string& s) {
    set<char> ans;
    if (s.empty()) {
        ans.insert('#');
        return ans;
    }

    bool allNullable = true;
    for (char X : s) {
        if (!isNonTerminal(X)) {
            ans.insert(X);
            allNullable = false;
            break;
        } else {
            for (char c : FIRST[X]) {
                if (c != '#') ans.insert(c);
            }
            if (!FIRST[X].count('#')) {
                allNullable = false;
                break;
            }
        }
    }
    if (allNullable) ans.insert('#');
    return ans;
}

void computeFollow() {
    for (char nt : nonterminals) FOLLOW[nt] = {};
    FOLLOW[start_symbol].insert('$');

    bool changed = true;
    while (changed) {
        changed = false;

        for (auto& p : prods) {
            char A = p.lhs;
            string rhs = p.rhs;
            if (rhs == "#") continue;

            for (int i = 0; i < (int)rhs.size(); i++) {
                char B = rhs[i];
                if (!isNonTerminal(B)) continue;

                string beta = rhs.substr(i + 1);
                set<char> fb = firstOfString(beta);

                for (char x : fb) {
                    if (x == '#') continue;
                    if (!FOLLOW[B].count(x)) {
                        FOLLOW[B].insert(x);
                        changed = true;
                    }
                }

                if (fb.count('#')) {
                    for (char x : FOLLOW[A]) {
                        if (!FOLLOW[B].count(x)) {
                            FOLLOW[B].insert(x);
                            changed = true;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    cout << "Enter number of productions: ";
    int n;
    cin >> n;

    vector<pair<char, string>> raw;
    cout << "Use format A->aB or A-># (epsilon)\n";
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        char lhs = s[0];
        string rhs = s.substr(3);
        raw.push_back({lhs, rhs});
        nonterminals.insert(lhs);
    }

    start_symbol = raw[0].first;
    augmented_start = '\'';
    while (nonterminals.count(augmented_start)) augmented_start++;

    prods.push_back({augmented_start, string(1, start_symbol)});
    nonterminals.insert(augmented_start);

    for (auto& x : raw) prods.push_back({x.first, x.second});

    for (int i = 0; i < (int)prods.size(); i++) {
        by_lhs[prods[i].lhs].push_back(i);
        for (char c : prods[i].rhs) {
            if (c == '#') continue;
            if (isNonTerminal(c)) nonterminals.insert(c);
            else terminals.insert(c);
        }
    }
    terminals.insert('$');

    computeFirst();
    computeFollow();

    vector<set<Item>> C;
    map<set<Item>, int> id;
    map<pair<int, char>, int> transitions;

    set<Item> I0;
    I0.insert({0, 0});
    I0 = closureSet(I0);

    C.push_back(I0);
    id[I0] = 0;

    queue<int> q;
    q.push(0);

    set<char> symbols;
    for (char t : terminals) if (t != '$') symbols.insert(t);
    for (char nt : nonterminals) symbols.insert(nt);

    while (!q.empty()) {
        int i = q.front();
        q.pop();

        for (char X : symbols) {
            set<Item> J = goToSet(C[i], X);
            if (J.empty()) continue;

            if (!id.count(J)) {
                int nid = (int)C.size();
                id[J] = nid;
                C.push_back(J);
                q.push(nid);
            }
            transitions[{i, X}] = id[J];
        }
    }

    map<pair<int, char>, string> ACTION;
    map<pair<int, char>, int> GOTO;
    bool conflict = false;

    for (int i = 0; i < (int)C.size(); i++) {
        for (auto it : C[i]) {
            int pid = it.prod_id;
            string rhs = prods[pid].rhs;

            if (it.dot < (int)rhs.size()) {
                char a = rhs[it.dot];
                if (!isNonTerminal(a)) {
                    int j = transitions[{i, a}];
                    string val = "s" + to_string(j);
                    auto key = make_pair(i, a);
                    if (ACTION.count(key) && ACTION[key] != val) conflict = true;
                    ACTION[key] = val;
                }
            } else {
                char A = prods[pid].lhs;
                if (pid == 0) {
                    ACTION[{i, '$'}] = "acc";
                } else {
                    for (char a : FOLLOW[A]) {
                        string val = "r" + to_string(pid);
                        auto key = make_pair(i, a);
                        if (ACTION.count(key) && ACTION[key] != val) conflict = true;
                        ACTION[key] = val;
                    }
                }
            }
        }

        for (char A : nonterminals) {
            if (A == augmented_start) continue;
            auto k = make_pair(i, A);
            if (transitions.count(k)) GOTO[k] = transitions[k];
        }
    }

    cout << "\nCanonical Collection of LR(0) Items:\n";
    for (int i = 0; i < (int)C.size(); i++) {
        cout << "I" << i << ":\n";
        for (auto it : C[i]) {
            auto p = prods[it.prod_id];
            cout << "  " << p.lhs << "->";
            if (p.rhs == "#") {
                if (it.dot == 0) cout << ".#";
                else cout << "#.";
            } else {
                for (int k = 0; k <= (int)p.rhs.size(); k++) {
                    if (k == it.dot) cout << ".";
                    if (k < (int)p.rhs.size()) cout << p.rhs[k];
                }
            }
            cout << "\n";
        }
        cout << "\n";
    }

    vector<char> tlist, ntlist;
    for (char t : terminals) tlist.push_back(t);
    for (char nt : nonterminals) if (nt != augmented_start) ntlist.push_back(nt);

    cout << "ACTION TABLE:\n";
    cout << setw(6) << "State";
    for (char t : tlist) cout << setw(8) << t;
    cout << "\n";
    for (int i = 0; i < (int)C.size(); i++) {
        cout << setw(6) << i;
        for (char t : tlist) {
            auto k = make_pair(i, t);
            if (ACTION.count(k)) cout << setw(8) << ACTION[k];
            else cout << setw(8) << ".";
        }
        cout << "\n";
    }

    cout << "\nGOTO TABLE:\n";
    cout << setw(6) << "State";
    for (char nt : ntlist) cout << setw(8) << nt;
    cout << "\n";
    for (int i = 0; i < (int)C.size(); i++) {
        cout << setw(6) << i;
        for (char nt : ntlist) {
            auto k = make_pair(i, nt);
            if (GOTO.count(k)) cout << setw(8) << GOTO[k];
            else cout << setw(8) << ".";
        }
        cout << "\n";
    }

    if (conflict) cout << "\nGrammar has conflicts in SLR table (not SLR(1)).\n";
    else cout << "\nGrammar is conflict-free for SLR(1).\n";

    cout << "\nProductions (for reduce rX):\n";
    for (int i = 1; i < (int)prods.size(); i++) {
        cout << "r" << i << ": " << prods[i].lhs << "->" << prods[i].rhs << "\n";
    }

    cout << "\nEnter input string to test (without $): ";
    string w;
    cin >> w;
    w.push_back('$');

    vector<int> st;
    st.push_back(0);
    int ip = 0;
    bool ok = true;

    while (true) {
        int s = st.back();
        char a = w[ip];
        auto key = make_pair(s, a);

        if (!ACTION.count(key)) {
            ok = false;
            break;
        }

        string act = ACTION[key];
        if (act == "acc") break;

        if (act[0] == 's') {
            int ns = stoi(act.substr(1));
            st.push_back(ns);
            ip++;
        } else if (act[0] == 'r') {
            int rid = stoi(act.substr(1));
            auto p = prods[rid];
            int poplen = (p.rhs == "#") ? 0 : (int)p.rhs.size();

            while (poplen--) st.pop_back();

            int top = st.back();
            if (!GOTO.count({top, p.lhs})) {
                ok = false;
                break;
            }
            st.push_back(GOTO[{top, p.lhs}]);
        } else {
            ok = false;
            break;
        }
    }

    if (ok) cout << "Input string accepted.\n";
    else cout << "Input string rejected.\n";

    return 0;
}
