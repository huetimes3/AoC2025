#include <vector>
#include <iostream>
#include <string>
using namespace std;

vector<string> parseInput(char delimiter = '\n') {
    vector<string> v;

    string s;
    while(getline(cin, s, delimiter)) {
        if (s.size() == 0) break;
        v.push_back(s);
    }

    return v;
}

int mod(int a, int b) {
    return (a % b + b) % b;
}

int solve1(vector<string> v) {
    int dialPos = 50;
    int count = 0;

    for (string s: v) {
        // get shift
        int shift = stoi(s.substr(1));
        if (s[0] == 'L') shift *= -1;

        dialPos += shift;
        dialPos = mod(dialPos, 100);

        if (dialPos == 0) count++;
    }
    return count;
}

int solve2(vector<string> v) {
    int dialPos = 50;
    int count = 0;

    for (string s: v) {
        // get shift
        int shift = stoi(s.substr(1));
        int from = dialPos;
        if (s[0] == 'L') {
            shift *= -1;
            // weird move here, but basically avoid shifting when it's at 0
            if (dialPos != 0) from -= 100;
        }

        int to = from + shift;

        count += abs(to / 100);
        // cout << s << " : " << from << " > " << to << " : " << abs(to / 100) << endl;
        dialPos = mod(to, 100);
    }
    return count;
}

int main() {
    vector<string> v = parseInput();

    int sol1 = solve1(v);
    int sol2 = solve2(v);
    cout << "sol 1 : " << sol1 << "\n";
    cout << "sol 2 : " << sol2 << "\n";
}
