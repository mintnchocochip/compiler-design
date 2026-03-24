#include <bits/stdc++.h>
using namespace std;

struct nonterminal {
    char nt;
    vector<string> prod;
    set<char> first;
    set<char> follow;
};

struct fut {
    char primary;
    char secondary;
    int pos;
};

struct ffut {
    char target;
    char provider;
    bool from_follow;
    int pos;
};

vector<string> split(string s) {
    vector<string> result;
    string temp = "";
    for (char c : s) {
        if (c == '|') {
            if (!temp.empty()) result.push_back(temp);
            temp = "";
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) result.push_back(temp);
    return result;
}

bool isTerminal(char c) { return islower(c); }
bool hasEpsilon(const set<char>& first_set) { return first_set.count('*'); }

nonterminal* find_nt(vector<nonterminal>& cfg, char c) {
    for (auto& nt : cfg) if (nt.nt == c) return &nt;
    return nullptr;
}

bool checkLeftRecursion(const vector<nonterminal>& cfg) {
    for (const auto& x : cfg) {
        char target = x.nt;
        for (const string& s : x.prod) {
            if (!s.empty() && s[0] == target) return false;
        }
    }
    return true;
}

bool isLL1(const vector<nonterminal>& cfg) {
    for (const auto& A : cfg) {
        set<char> seen;
        for (const string& prod : A.prod) {
            set<char> this_first;
            if (prod == "*") this_first.insert('*');
            else {
                char sym = prod[0];
                if (isTerminal(sym)) this_first.insert(sym);
                else {
                    nonterminal* Y = find_nt(const_cast<vector<nonterminal>&>(cfg), sym);
                    if (Y) this_first = Y->first;
                }
            }
            for (char t : this_first) {
                if (seen.count(t)) return false;
                seen.insert(t);
            }
        }
    }
    return true;
}

void findfirst(vector<nonterminal>& cfg) {
    vector<fut> proc;
    for (auto& t : cfg) {
        for (string s : t.prod) {
            if (s == "*") {
                t.first.insert('*');
                continue;
            }
            if (s.empty()) continue;

            int i = 0;
            while (i < s.size()) {
                char sym = s[i];
                if (isTerminal(sym)) {
                    t.first.insert(sym);
                    break;
                }
                nonterminal* Y = find_nt(cfg, sym);
                if (Y && !Y->first.empty()) {
                    bool nullable = hasEpsilon(Y->first);
                    for (char term : Y->first) {
                        if (term != '*') t.first.insert(term);
                    }
                    if (!nullable) break;
                } else if (Y) {
                    proc.push_back({t.nt, sym, i});
                }
                i++;
            }
            if (i == s.size()) t.first.insert('*');
        }
    }

    bool changed = true;
    while (changed && !proc.empty()) {
        changed = false;
        for (int j = 0; j < proc.size(); j++) {
            fut dep = proc[j];
            nonterminal* prim = find_nt(cfg, dep.primary);
            nonterminal* sec = find_nt(cfg, dep.secondary);
            if (sec && !sec->first.empty()) {
                for (char term : sec->first) {
                    if (term != '*') {
                        size_t old = prim->first.size();
                        prim->first.insert(term);
                        if (prim->first.size() > old) changed = true;
                    }
                }
                proc.erase(proc.begin() + j);
                j--;
            }
        }
    }
}

void findfollow(vector<nonterminal>& cfg, char start) {
    vector<ffut> f_proc;
    for (auto& nt : cfg) nt.follow.clear();
    nonterminal* start_nt = find_nt(cfg, start);
    if (start_nt) start_nt->follow.insert('$');
    for (auto& A : cfg) {
        for (string s : A.prod) {
            if (s == "*") continue;
            int len = (int)s.size();
            for (int i = 0; i < len; ++i) {
                char B = s[i];
                if (isTerminal(B)) continue;
                if (i == len - 1) {
                    f_proc.push_back({B, A.nt, true, i});
                } else {
                    int j = i + 1;
                    bool need_followA = true;
                    while (j < len) {
                        char beta = s[j];
                        if (isTerminal(beta)) {
                            f_proc.push_back({B, beta, false, i});
                            need_followA = false;
                            break;
                        } else {
                            nonterminal* Y = find_nt(cfg, beta);
                            f_proc.push_back({B, beta, false, i});
                            if (Y && !hasEpsilon(Y->first)) {
                                need_followA = false;
                                break;
                            }
                        }
                        ++j;
                    }
                    if (need_followA) {
                        f_proc.push_back({B, A.nt, true, i});
                    }
                }
            }
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;
        for (int j = 0; j < (int)f_proc.size();) {
            ffut dep = f_proc[j];
            nonterminal* B = find_nt(cfg, dep.target);
            if (!B) { ++j; continue; }

            if (dep.from_follow) {
                nonterminal* prov = find_nt(cfg, dep.provider);
                if (!prov) { ++j; continue; }
                if (!prov->follow.empty()) {
                    for (char t : prov->follow) {
                        if (B->follow.insert(t).second) changed = true;
                    }
                    f_proc.erase(f_proc.begin() + j);
                } else {
                    ++j;
                }
            } else {
                if (isTerminal(dep.provider)) {
                    if (B->follow.insert(dep.provider).second) changed = true;
                    f_proc.erase(f_proc.begin() + j);
                } else {
                    nonterminal* prov = find_nt(cfg, dep.provider);
                    if (!prov) { ++j; continue; }
                    if (!prov->first.empty()) {
                        for (char t : prov->first) {
                            if (t != '*') {
                                if (B->follow.insert(t).second) changed = true;
                            }
                        }
                        f_proc.erase(f_proc.begin() + j);
                    } else {
                        ++j;
                    }
                }
            }
        }
        if (!changed) break;
    }
}

int main() {
    int n;
    cin >> n;

    vector<nonterminal> cfg(n);
    for (int i = 0; i < n; ++i) {
        char non;
        string prods;
        cin >> non >> prods;
        cfg[i].nt = non;
        cfg[i].prod = split(prods);
    }

    if (cfg.empty()) {
        cout << "No non-terminals provided." << endl;
        return 0;
    }

    char start_symbol = cfg.front().nt;

    if (!checkLeftRecursion(cfg)) {
        cout << "Grammar has left recursion - not LL(1)" << endl;
        return 1;
    }

    findfirst(cfg);
    findfollow(cfg, start_symbol);

    cout << "\nFIRST sets:" << endl;
    for (const auto& nt : cfg) {
        cout << "FIRST(" << nt.nt << ") = {";
        for (char t : nt.first) cout << t << " ";
        cout << "}" << endl;
    }

    cout << "\nFOLLOW sets:" << endl;
    for (const auto& nt : cfg) {
        cout << "FOLLOW(" << nt.nt << ") = {";
        for (char t : nt.follow) cout << t << " ";
        cout << "}" << endl;
    }

    cout << "\nLL(1)? " << (isLL1(cfg) ? "Yes" : "No") << endl;
    return 0;
}
