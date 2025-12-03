#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <cmath>
#include <regex>
using namespace std;

vector<string> parseInput(char delimiter = '\n') {
    vector<string> v;

    string s;
    while(getline(cin, s, delimiter)) {
        if (s.empty()) continue;
        size_t start = 0;
        while (start < s.size() && !isdigit(s[start])) start++;

        if (start == s.size()) continue;
        size_t end = s.size() - 1;
        while (end > start && !isdigit(s[end])) end--;

        s = s.substr(start, end - start + 1);

        if (s.empty()) continue;
        v.push_back(s);
    }

    return v;
}

int solve1(vector<string> v) {
    int sum = 0;
    for (string s: v) {
        char first = '0';
        int a;
        // get the first digit
        for (int i = 0; i < s.length() - 1; i++) {
            char c = s[i];
            if (c > first) {
                first = c;
                a = i;
            }
        }
        char second = '0';
        int b;
        for (int i = a + 1; i < s.length(); i++) {
            char c = s[i];
            if (c > second) {
                second = c;
                b = i;
            }
        }
        // combine them and add to sum
        string joltage = string() + first + second;
        sum += stoi(joltage);
    }

    cout << "sol 1 : " << sum << "\n";
    return 0;
}


int solve2(vector<string> v) {
    long long sum = 0;
    for (string s: v) {
        array<char, 12> digits;
        int startIndex = 0;
        int endCutoff = 11;
        for (int i = 0; i < 12; i++) {
            // for each of the digits
            char highest = '0';
            int n;
            for (int j = startIndex; j < s.length() - endCutoff; j++) {
                char c = s[j];
                if (c > highest) {
                    highest = c;
                    n = j;
                    if (c == '9') break;
                }
            }
            // once it finds the character, add it to digits and move on
            digits[i] = highest;
            startIndex = n + 1;
            endCutoff--;
        }
        // combine them and add to sum
        string joltage = string();
        for (char c: digits) {
            joltage += c;
        }
        sum += stoll(joltage);
    }

    cout << "sol 2 : " << sum << "\n";
    return 0;
}

int main() {
    vector<string> v = parseInput();

    solve1(v);
    solve2(v);
}
