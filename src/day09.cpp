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

// bool colinear(const LineSeg& a, const LineSeg& b) {
//     bool ah = (a.p1.y == a.p2.y);
//     bool bh = (b.p1.y == b.p2.y);
//     if (ah && bh) {
//         if (a.p1.y != b.p1.y) return false;
//         long long a1 = min(a.p1.x, a.p2.x), a2 = max(a.p1.x, a.p2.x);
//         long long b1 = min(b.p1.x, b.p2.x), b2 = max(b.p1.x, b.p2.x);
//         // if (a1 ==)
//         return max(a1, b1) <= min(a2, b2);
//     }
//
//     if (!ah && !bh) {
//         long long a1 = min(a.p1.y, a.p2.y), a2 = max(a.p1.y, a.p2.y);
//         long long b1 = min(b.p1.y, b.p2.y), b2 = max(b.p1.y, b.p2.y);
//         if (a1 == 66359 && a2 == 67466 && b1 == 50401 && b2 == 67466) 
//             cout << "found the segment: " << a.p1.x << " - " << b.p1.x << endl;
//         if (a.p1.x != b.p1.x) return false;
//         return max(a1, b1) <= min(a2, b2);
//     }
//
//     return false;
// }
//
// bool touchesAtEndpoint(const LineSeg& l1, const LineSeg& l2) {
//     return
//         (l1.p1.x == l2.p1.x && l1.p1.y == l2.p1.y) ||
//         (l1.p1.x == l2.p2.x && l1.p1.y == l2.p2.y) ||
//         (l1.p2.x == l2.p1.x && l1.p2.y == l2.p1.y) ||
//         (l1.p2.x == l2.p2.x && l1.p2.y == l2.p2.y);
// }
//
// bool checkIntersectionOld(const LineSeg& l1, const LineSeg& l2) {
//     // first check if the 2 lines are perp (diff bw x and y matching)
//     bool isHorizl1 = (l1.p1.y == l1.p2.y);
//     bool isHorizl2 = (l2.p1.y == l2.p2.y);
//     if (isHorizl1 == isHorizl2) return false;
//
//     LineSeg h = isHorizl1 ? l1 : l2;
//     LineSeg v = isHorizl1 ? l2 : l1;
//
//     int hy = h.p1.y;
//     int hx1 = min(h.p1.x, h.p2.x);
//     int hx2 = max(h.p1.x, h.p2.x);
//
//     int vx = v.p1.x;
//     int vy1 = min(v.p1.y, v.p2.y);
//     int vy2 = max(v.p1.y, v.p2.y);
//
//     bool intersects = (vx >= hx1 && vx <= hx2) && (hy >= vy1 && hy <= vy2);
//
//     // cout << "intersects:" << (intersects ? "y" : "n") << " : " <<  l1.p1.x << "," << l1.p1.y << " - " << l1.p2.x << "," << l1.p2.y << " and " << l2.p1.x << "," << l2.p1.y << " - " << l2.p2.x << "," << l2.p2.y << endl;
//     return intersects;
// }

bool rightCandidate(const Point& p, const Point& q) {
    return (p.x == 94699 && p.y == 50401 && q.x == 5106 && q.y == 67466);
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
        if (rightCandidate(p, q)) {
            cout << ly1 << " " << y1 << " --- " << ly2 << " " << y2 << endl;
        }
        return max(ly1, y1) < min(ly2, y2);
    }
}

// bool cool(const LineSeg& a, const LineSeg& b) {
//     long long a1 = min(a.p1.y, a.p2.y), a2 = max(a.p1.y, a.p2.y);
//     long long b1 = min(b.p1.y, b.p2.y), b2 = max(b.p1.y, b.p2.y);
//     if (a1 == 66359 && a2 == 67466 && b1 == 50401 && b2 == 67466) 
//         cout << "found the segment cool: " << a.p1.x << " - " << b.p1.x << endl;
//     return false;
//     return (a.p1.y == 67466 && a.p2.y == 66359);
// }

void printLineSeg(const LineSeg& l) {
    cout << l.p1.x << "," << l.p1.y << " - " << l.p2.x << "," << l.p2.y;
    return;
}

void solve2(vector<Point> v) {
    long long sol = 0;
    Point solPointA, solPointB;
    int n = v.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long c = abs(((v[i].x + 1) - v[j].x) * ((v[i].y + 1) - v[j].y)); // candidate
            // ignore candidate if too small
            if (c < sol) continue;
            // if (rightCandidate(v[i], v[j]))
            //     cout << "woohoo: " << c << " compared to " << sol << endl;
            // int x1 = min(v[i].x, v[j].x);
            // int x2 = max(v[i].x, v[j].x);
            // int y1 = min(v[i].y, v[j].y);
            // int y2 = max(v[i].y, v[j].y);
            // vector<LineSeg> rectEdges = {
            //     { Point{x1, y1}, Point{x2, y1} },
            //     { Point{x2, y1}, Point{x2, y2} },
            //     { Point{x2, y2}, Point{x1, y2} },
            //     { Point{x1, y2}, Point{x1, y1} }
            // };
            bool intersects = false;
            for (int k = 0; k < n; k++) {
                Point next = k < n - 1 ? v[k+1] : v[0];
                LineSeg l1 = { v[k] , next };
                // compare with the 4 sides
                // if (rightCandidate(v[i], v[j])) {
                //     printLineSeg(l1);
                //     cout << endl;
                // }
                // if (cool(l1, rectEdges[3])) cout << "at least we're trying it out maybe" << endl;


                // bool toSkip = false;
                // for (auto& e : rectEdges) {
                //     // if (cool(l1, e)) cout << "trying this combo!!" << endl;
                //     // if (colinear(l1, e)) { toSkip = true; break; }
                //     // if (touchesAtEndpoint(l1, e)) { toSkip = true; break; }
                //     if (!checkIntersection(e, v[i], v[j])) continue;
                //     // if there is an intersection, check if either 
                //     intersects = true;
                //     break;
                // }
                // if (toSkip) continue;
                if (checkIntersection(l1, v[i], v[j])) {
                    if (rightCandidate(v[i], v[j]))cout << "intersects..." << endl;
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
