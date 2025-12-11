#include <vector>
// #include <map>
// #include <array>
#include <iostream>
#include <string>
// #include <algorithm>
// #include <thread>
// #include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

auto parseInput(char delimiter = '\n') {
    vector<string> input;

    string s;
    while(getline(cin, s, delimiter)) {
        if (s.empty()) break;
        input.push_back(s);
    }

    return input;
}

long long traverse(const vector<string>& input, int i, int j, vector<vector<long long>>& memo) {
    if (memo[i][j] != -1) return memo[i][j];
    long long n;
    if (i == input.size()-1) n = 1;
    else if (input[i+1][j] != '^') n = traverse(input, i+1, j, memo);
    else n = traverse(input, i+1, j-1, memo) + traverse(input, i+1, j+1, memo);

    memo[i][j] = n;
    return n;
}

void solve1(vector<string> input) {
    int sum = 0;
    int h = input.size();
    int w = input[0].length();

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            // if we're at the |, propagate down (or split among the left and right)
            if ((input[i][j] == '|' || input[i][j] == 'S') && i != h - 1) {
                if (input[i+1][j] == '^') {
                    input[i+1][j-1] = '|';
                    input[i+1][j+1] = '|';
                    sum++;
                    continue;
                }
                input[i+1][j] = '|';
            }
        }
    }

    cout << "sol 1 : " << sum << endl;
    return;
}


void solve2(vector<string> input) {
    long long sum = 0;
    int h = input.size();
    int w = input[0].length();

    vector<vector<long long>> memo(h, vector<long long>(w, -1));
    // find the starting point
    for (int j = 0; j < input[0].length(); j++) {
        if (input[0][j] == 'S') {
            sum = traverse(input, 1, j, memo);
            break;
        }
    }

    cout << "sol 2 : " << sum << endl;
    return;
}


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    vector<string> input = parseInput();

    solve1(input);
    solve2(input);
}
