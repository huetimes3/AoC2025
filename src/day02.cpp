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

array<string, 2> getRange(string s) {
    array<string, 2> range;

    string::size_type i = s.find('-');
    if (s.npos != i) {
        range[1] = s.substr(i + 1);
        range[0] = s.substr(0, i);
    }

    return range;
}

long long halve(string s, bool roundup = true) {
    int len = s.length();
    // return first half if length is even
    if (len % 2 == 0) return stoll(s.substr(0, len / 2));

    // otherwise return either the nearest "half" that's above or below
    // an interesting property is that something like 
    // half of 11111 would round up to 100 ('1' + '0' * floor(len / 2)) and
    // half of 11111 would round down to 99 ('9' * floor(len / 2))
    return stoll((roundup) ? "1" + string(len / 2, '0') : string(len / 2, '9'));
}

int solve1(vector<string> v) {
    long long sum = 0;
    for (string s: v) {
        array<string, 2> r = getRange(s);
        // cout << r[0] << ">" << r[1] << " - ";
        long long starth = halve(r[0]);
        long long endh = halve(r[1], false);
        long long start = stoll(r[0]);
        long long end = stoll(r[1]);
        // cout << starth << ">" << endh << endl;
        if (endh < starth) continue;
        for (long long i = starth; i <= endh; i++) {
            string stringn = to_string(i);
            long long n = stoll(stringn + stringn);
            if (start <= n && n <= end) {
                sum += n;
            }
        }
    }

    cout << "sol 1 : " << sum << "\n";
    return 0;
}

// prevent regenerating the regex
regex r("^(.*)\\1+$");
bool hasRepeatingParts(long long n) {
    string s = to_string(n);
    return regex_match(s, r);
}

int solve2(vector<string> v) {
    long long sum = 0;
    for (string s: v) {
        array<string, 2> r = getRange(s);
        long long start = stoll(r[0]);
        long long end = stoll(r[1]);

        // this is where we kinda give up and just brute force (in a not silly way)
        // jokes we're doing it the silly way

        // cout << start << ">" << end << endl;
        for (long long n = start; n <= end; n++) {
            // doing the regexp solution,,,, alas,,,,
            if (hasRepeatingParts(n)) sum += n;
        }
        // abandoned endeavors
        // for (int i = 1; i <= r[1].length() / 2; i++) {
        //     cout << i << endl;
        //
        // }
    }

    cout << "sol 2 : " << sum << "\n";
    return 0;
}

int main() {
    vector<string> v = parseInput(',');

    solve1(v);
    solve2(v);
}
