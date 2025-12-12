#include <vector>
// #include <map>
// #include <array>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
// #include <thread>
// #include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

struct Point {
    long long x, y;
};

struct LineSeg {
    Point p1, p2;
};

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

Point parsePoint(string s) {
    auto p = split(s, ',');

    return Point { stoll(p[0]), stoll(p[1]) };
}

auto parseInput(char delimiter = '\n') {
    vector<Point> input;

    string s;
    while(getline(cin, s, delimiter)) {
        if (s.empty()) break;
        input.push_back(parsePoint(s));
    }

    return input;
}

void solve1(vector<Point> v) {
    long long sol = 0;
    int n = v.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long c = abs(((v[i].x + 1) - v[j].x) * ((v[i].y + 1) - v[j].y)); // candidate
            if (c > sol) sol = c;
        }
    }

    cout << "sol 1 : " << sol << endl;
    return;
}

bool checkIntersection(const LineSeg& l, const Point& p, const Point& q) {
    bool isHoriz = (l.p1.y == l.p2.y);

    long long x1 = min(p.x, q.x);     // smallest x
    long long x2 = max(p.x, q.x);     // largest x
    long long y1 = min(p.y, q.y);     // smallest y
    long long y2 = max(p.y, q.y);     // largest y
    // if it's horizontal we check if l is between the two y's, else if it's between the x'es
    if (isHoriz) {
        // if it's not between the y's then it's not intersecting
        long long y = l.p1.y;
        if (y <= y1 || y >= y2) return false;
        // next check if it overlaps with a hypothetical line in the rectangle
        long long lx1 = min(l.p1.x, l.p2.x);
        long long lx2 = max(l.p1.x, l.p2.x);
        return max(lx1, x1) < min(lx2, x2);
    } else {
        long long x = l.p1.x;
        if (x <= x1 || x >= x2) return false;

        long long ly1 = min(l.p1.y, l.p2.y);
        long long ly2 = max(l.p1.y, l.p2.y);
        return max(ly1, y1) < min(ly2, y2);
    }
}

void solve2(vector<Point> v) {
    long long sol = 0;
    Point solPointA, solPointB;
    int n = v.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long c = (abs(v[i].x - v[j].x) + 1) * (abs(v[i].y - v[j].y) + 1); // candidate
            if (c < sol) continue;
            bool intersects = false;

            for (int k = 0; k < n; k++) {
                Point next = k < n - 1 ? v[k+1] : v[0];
                LineSeg l1 = { v[k] , next };

                if (checkIntersection(l1, v[i], v[j])) {
                    intersects = true;
                    break;
                }
                if (intersects) break;
            }
            if (intersects) continue;
            sol = c;
            solPointA = v[i];
            solPointB = v[j];
        }
    }
    cout << "a: " << solPointA.x << "," << solPointA.y << endl <<"b: " << solPointB.x << "," << solPointB.y << endl;

    cout << "sol 2 : " << sol << endl;
    return;
}


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    auto input = parseInput();

    solve1(input);
    solve2(input);
}
