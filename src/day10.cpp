#include <vector>
#include <map>
// #include <array>
#include <math.h>
#include <iostream>
#include <string>
#include <bitset>
// #include <algorithm>
// #include <thread>
// #include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

// #define VERBOSE
#ifdef VERBOSE
    #define VOUT(x) do { cout << x; } while (0)
#else
    #define VOUT(x) do {} while (0)
#endif

struct Machine {
    int indicatorLight;
    vector<int> buttons;
    vector<int> joltages;
};

string repeat(const string& s, int n) {
    string out;
    out.reserve(s.size() * n);
    for (int i = 0; i < n; i++) out += s;
    return out;
}

vector<string> split(string s, char delimiter = ' ') {
    vector<string> tokens;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        if (!token.empty()) tokens.push_back(token);
        s.erase(0, pos + 1);
    }
    if (!s.empty()) tokens.push_back(s);

    return tokens;
}

auto machinify(string s) {
    Machine m;
    vector<string> tokens = split(s);
    // turn the first string into an integer representation of the binary
    m.indicatorLight = 0;
    for (int i = 0; i < tokens[0].length() - 2; i++) {
        if (tokens[0][i+1] == '#') m.indicatorLight |= (1 << i);
    }
    // get the buttons and what they do
    for (int i = 1; i < tokens.size() - 1; i++) {
        string btnstr = tokens[i];
        btnstr.erase(0, 1);
        btnstr.erase(btnstr.size() - 1);
        vector<string> btnTokens = split(btnstr, ',');
        int btnmask = 0;
        for (string w: btnTokens) {
            btnmask |= (1 << stoi(w));
        }
        m.buttons.push_back(btnmask);
    }

    string js = tokens[tokens.size() - 1];
    js.erase(0, 1);
    js.erase(js.size() - 1);
    for (string j: split(js, ',')) {
        m.joltages.push_back(stoi(j));
    }
    return m;
}

auto parseInput(char delimiter = '\n') {
    vector<Machine> input;

    string s;
    while(getline(cin, s, delimiter)) {
        if (s.empty()) break;
        input.push_back(machinify(s));
    }

    return input;
}

void testComboSize(const vector<int>& v, int target, int k, int start, vector<int>& curr, vector<vector<int>>& slns) {
    if (curr.size() == k) {
        int a = 0;
        for (int x: curr) {
            a ^= x;
        }
        if (a == target) {
            slns.push_back(curr);
        }
        return;
    }

    for (int i = start; i < v.size(); i++) {
        curr.push_back(v[i]);
        testComboSize(v, target, k, i + 1, curr, slns);
        curr.pop_back();
    }
    return;
}

int solveMachine(Machine m) {
    // find fewest number of button presses to get to indicatorLight from buttons

    // go super naive for now
    vector<int> curr;
    for (int i = 1; i <= m.buttons.size(); i++) {
        vector<vector<int>> slns;
        testComboSize(m.buttons, m.indicatorLight, i, 0, curr, slns);
        if (slns.size() > 0) return i;
    }
    return 0;
}

void solve1(vector<Machine> v) {
    int sum = 0;

    for (Machine m: v) {
        sum += solveMachine(m);
    }

    cout << "sol 1 : " << sum << endl;
    return;
}

int getStateIndicatorLight(const vector<int>& j) {
    int mask = 0;
    for (int i = 0; i < j.size(); i++) {
        if (j[i] & 1) mask |= (1 << i);     // set bit i
    }
    return mask;
}

int getBitAtIndex(int n, int i) {
    return n & (1 << i);
}

string bitify(int n) {
    return bitset<8>(n).to_string();
}

int solveMachineState(Machine m, map<int, vector<vector<int>>> memo, int depth = 0) {
    // find fewest number of button presses to get to indicatorLight from buttons
    int currState = getStateIndicatorLight(m.joltages);
    int best = 90000000;
    string prefix = repeat("┃ ", (depth == 0 ? 0 : depth - 1));
    VOUT(prefix << (depth == 0 ? "" : "┃ ") << "TO BEAT : " << bitify(currState));

    VOUT(" {");
    for (int n: m.joltages) {
        VOUT(n << ", ");
    }
    VOUT("}" << endl);
    vector<vector<int>> slns;
    if (memo.contains(currState)) {
        slns = memo[currState];
    }
    else {
        for (int i = 1; i <= m.buttons.size(); i++) {
            vector<int> curr;
            testComboSize(m.buttons, currState, i, 0, curr, slns);
        }
        memo[currState] = slns;
    }
    // if the state is 0, it could be possible that the best solution is no button presses too
    if (currState == 0) {
        vector<int> newJoltages = m.joltages;
        for (int i = 0; i < newJoltages.size(); i++) {
            newJoltages[i] /= 2;
        }
        int currBest = 2 * solveMachineState(Machine { 0, m.buttons, newJoltages }, memo, depth + 1);
        if (currBest < best) best = currBest;
    }

    if (slns.size() == 0 && best == 90000000) {
        VOUT(prefix << (depth == 0 ? "" : "┗ ") << "dead end :("<< endl);
        return best;
    }
    // otherwise return some combination of the lesser stuff and pick the lowest (best one)
    for (vector<int> sln: slns) {
        vector<int> newJoltages = m.joltages;
        bool underflowed = false;
        bool reachedZero = true;
        for (int i = 0; i < newJoltages.size(); i++) {
            int jolt = newJoltages[i];
            // apply joltage modifications
            for (int b: sln) {
                if (getBitAtIndex(b, i)) jolt--;
            }
            if (jolt < 0) {
                underflowed = true;
                break;
            }
            if (jolt != 0) {
                reachedZero = false;
            }
            newJoltages[i] = jolt / 2;
        }
        //
        // COUT SOME STUFF
        //
        VOUT(prefix << (depth == 0 ? "" : "┣ ") << "❖ ");
        for (int b: sln) {
            VOUT("{" << bitify(b) << "},");
        }
        VOUT(endl);
        // VOUT(" > {");
        // for (int n: newJoltages) {
        //     VOUT(n << ", ");
        // }
        // VOUT("}" << endl);
        //
        // COUT SOME STUFF
        //

        if (underflowed) continue;
        int currBest = reachedZero ? 0 : 2 * solveMachineState(Machine { 0, m.buttons, newJoltages }, memo, depth + 1);
        currBest += sln.size();
        if (currBest < best) best = currBest;
    }


    VOUT(prefix << (depth == 0 ? "" : "┗ ") << "best -> " << best << endl);
    return best;
}

void solve2(vector<Machine> v) {
    int sum = 0;

    for (Machine m: v) {
        VOUT("machine " << bitify(m.indicatorLight) << endl);
        map<int, vector<vector<int>>> memo;
        int sol = solveMachineState(m, memo);
        if (sol > 1000000) {
            cout << "machine " << bitify(m.indicatorLight) << " with " << m.joltages[0] << " and " << m.joltages[1] << " had an issue" << endl;
            break;
        }
        // sum += solveMachineState(m, memo);
        sum += sol;
        VOUT(endl << endl);
    }
    cout << "sol 2 : " << sum << endl;
    return;
}


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    vector<Machine> input = parseInput();

    solve1(input);
    solve2(input);
}
