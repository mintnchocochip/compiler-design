#include <bits/stdc++.h>
using namespace std;

set<string> dp[20][20];

vector<pair<string, string>> unitRules;
vector<tuple<string,string,string>> binRules;
string startSymbol;

string trim(string s) {
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    s.erase(remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
    return s;
}

bool cyk(vector<string>& inp) {
    int n = inp.size();
    if (n == 0) return false;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dp[i][j].clear();

    for (int i = 0; i < n; i++)
        for (auto& [lhs, rhs] : unitRules)
            if (rhs == inp[i])
                dp[i][i].insert(lhs);

    for (int len = 2; len <= n; len++)
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            for (int k = i; k < j; k++)
                for (auto& [lhs, B, C] : binRules)
                    if (dp[i][k].count(B) && dp[k+1][j].count(C))
                        dp[i][j].insert(lhs);
        }

    return dp[0][n-1].count(startSymbol);
}

int main() {
    string line;
    int n;

    getline(cin, line);
    n = stoi(trim(line));

    for (int i = 0; i < n; i++) {
        getline(cin, line);

        int pos = line.find("->");
        string lhs = trim(line.substr(0, pos));
        string rhs = line.substr(pos + 2);
        while (!rhs.empty() && rhs.front() == ' ') rhs = rhs.substr(1);
        rhs = trim(rhs);

        if (i == 0) startSymbol = lhs;

        vector<string> sym;
        string w;
        istringstream ss(rhs);
        while (ss >> w) sym.push_back(trim(w));

        if (sym.size() == 1)
            unitRules.push_back({lhs, sym[0]});
        else
            binRules.push_back({lhs, sym[0], sym[1]});
    }

    getline(cin, line);
    vector<string> inp;
    for (char c : line)
        if (c != '\r' && c != '\n')
            inp.push_back(string(1, c));

    cout << "Result: " << (cyk(inp) ? "ACCEPTED" : "REJECTED") << "\n";

    return 0;
}
