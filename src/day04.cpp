#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif
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

void printGrid(const vector<string>& v) {
    cout << "\033[H";
    for (const auto& row : v) {
        for (char c : row) {
            switch (c) {
                case '@': cout << "\033[93m" << "█" << "\033[0m"; break; // yellow
                case '1': cout << "\033[91m" << "█" << "\033[0m"; break; // red!
                case '2': cout << "\033[31m" << "▓" << "\033[0m"; break; // less red
                case '3': cout << "\033[35m" << "▒" << "\033[0m"; break; // magenta
                case '4': cout << "\033[34m" << "░" << "\033[0m"; break; // blue
                case '5': cout << "\033[37m" << "·" << "\033[0m"; break; // white
                default: cout << " "; break;
            }
        }
        cout << endl;
    }
    cout.flush();
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

int solve2visual(vector<string> v) {
    int sum = 0;
    int height = v.size();
    int width = v[0].length();

    cout << "\033[2J";   // clear screen
    cout << "\033[?25l"; // hide cursor

    bool removing = true;

    while (removing) {
        removing = false;
        auto prev = v;

        // show animation frame
        printGrid(v);
        this_thread::sleep_for(chrono::milliseconds(100));

        // fade older removed cells
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                if (v[i][j] >= '1' && v[i][j] <= '5')
                    v[i][j] = (v[i][j] == '5') ? '.' : char(v[i][j] + 1);

        // removal check
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (prev[i][j] != '@') continue;

                int around = 0;
                for (int n = max(i - 1, 0); n < min(i + 2, height); n++)
                    for (int m = max(j - 1, 0); m < min(j + 2, width); m++)
                        if (prev[n][m] == '@')
                            around++;

                if (around < 5) {
                    sum++;
                    removing = true;
                    v[i][j] = '1'; // NEW: newly removed becomes stage 1
                }
            }
        }
    }

    // final frame
    printGrid(v);
    cout << "\033[?25h"; // show cursor again
 
    cout << "sol 2 : " << sum << "\n";
    return sum;
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    vector<string> v = parseInput();

    solve1(v);
    solve2(v);
    solve2visual(v);
}
