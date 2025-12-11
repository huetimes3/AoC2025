#include <vector>
#include <map>
#include <array>
#include <iostream>
#include <string>
#include <algorithm>
// #include <thread>
// #include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

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

auto tokenize(vector<string> v) {
    vector<vector<string>> tokenMap;
    for (string s: v) tokenMap.push_back(split(s));
    return tokenMap;
}

auto parseInput(char delimiter = '\n') {
    vector<string> input;

    string s;
    while(getline(cin, s, delimiter)) {
        if (s.empty()) break;
        input.push_back(s);
    }

    return input;
}

long long solveCephalopodProblem(char op, vector<string> operands) {
    long long result;
    if (op == '+') {
        result = 0;
        for (string operand: operands) {
            result += stoi(operand);
        }
    } else {
        result = 1;
        for (string operand: operands) {
            result *= stoi(operand);
        }
    }
    return result;
}


void solve1(vector<vector<string>> input) {
    long long sum = 0;

    int length = input[0].size();
    int height = input.size();

    for (int i = 0; i < length; i++) {
        vector<string> operands;
        long long result = 0;
        for (int j = 0; j < height - 1; j++) {
            operands.push_back(input[j][i]);
        }
        sum += solveCephalopodProblem(input[height - 1][i][0], operands);
    }

    cout << "sol 1 : " << sum << "\n";
    return;
}


void solve2(vector<string> input) {
    // god freaking damn it
    long long sum = 0;

    int h = input.size();
    int w = input[0].size();

    char op = input[h - 1][0];
    vector<string> operands = {};
    for (int i = 0; i < w; i++) {
        string operand = "";
        // if the next one has the operator  in its col then calculate the current and clear stuff
        // TODO: handle edge case here!!!!
        if (i != w - 1 && input[h - 1][i + 1] != ' ') {
            sum += solveCephalopodProblem(op, operands);
            op = input[h - 1][i + 1];
            operands = {};
            continue;
        }
        for (int j = 0; j < h - 1; j++) {
            // if the char isn't a space add it to the operand
            if (input[j][i] != ' ') {
                operand += input[j][i];
            }
        }
        operands.push_back(operand);
    }
    sum += solveCephalopodProblem(op, operands);

    cout << "sol 2 : " << sum << "\n";
    return;
}


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    vector<string> input = parseInput();

    solve1(tokenize(input));
    solve2(input);
}
