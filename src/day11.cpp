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

void solve1(vector<string> input) {
    int sum = 0;

    cout << "sol 1 : " << sum << endl;
    return;
}


void solve2(vector<string> input) {
    int sum = 0;


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
