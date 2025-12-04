#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <algorithm>
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

int solve1(vector<string> v) {
    int sum = 0;
    int height = v.size();
    int width = v[0].length();

    for (int i = 0; i < v.size(); i++) {
        string s = v[i];
        for (int j = 0; j < s.length(); j++) {
            char c = s[j];
            if (c != '@') continue;

            int around = 0;
            // check if there's less than 4 rolls around (5 including the current one)
            for (int n = max(i - 1, 0); n < min(i + 2, height); n++) {
                for (int m = max(j - 1, 0); m < min(j + 2, width); m++) {
                    if (v[n][m] == '@') around++;
                    if (around >= 5) break;
                }
                if (around >= 5) break;
            }
            if (around < 5) sum++;
        }
    }

    cout << "sol 1 : " << sum << "\n";
    return 0;
}


int solve2(vector<string> v) {
    int sum = 0;
    int height = v.size();
    int width = v[0].length();

    bool removing = true;

    while (removing) {
        removing = false;
        auto prev = v;

        // for (string s: v) {
        //     cout << s << endl;
        // }
        // cout << endl << endl;
        // marks rolls for removal
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (prev[i][j] != '@') continue;

                int around = 0;
                // check if there's less than 4 rolls around (5 including the current one)
                for (int n = max(i - 1, 0); n < min(i + 2, height); n++)
                    for (int m = max(j - 1, 0); m < min(j + 2, width); m++)
                        if (prev[n][m] == '@') around++;

                if (around < 5) {
                    sum++;
                    removing = true;
                    v[i][j] = '.';
                }
            }
        }
    }

    cout << "sol 2 : " << sum << "\n";
    return 0;
}

int main() {
    vector<string> v = parseInput();

    solve1(v);
    solve2(v);
}
