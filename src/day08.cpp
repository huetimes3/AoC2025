#include <vector>
#include <queue>
#include <functional>
#include <numeric>
#include <map>
// #include <array>
#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
// #include <thread>
// #include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

struct Point {
    long long x;
    long long y;
    long long z;
};

struct Pair {
    int i1, i2;         // indeces of the points
    long long dist;
};

struct DSU {
    vector<int> rank, parent;
    int components;
    // constructor
    DSU(int n) : parent(n), rank(n, 1), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (rank[a] < rank[b]) swap(a, b);
        parent[b] = a;
        rank[a] += rank[b];
        components--;
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }

    bool allConnected() const {
        return components == 1;
    }
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

    return Point { stoll(p[0]), stoll(p[1]), stoll(p[2]) };
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

long long calculateDist(Point p1, Point p2) {
    return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2);
}

auto cmp = [](Pair left, Pair right) { return left.dist < right.dist; };
bool revcmp (int i, int j) { return i > j; }

const int MAX_CONNECTIONS = 1000;
void solve1(vector<Point> input) {
    int sum = 0;
    // storage of close stuff is only 1000 max, we can be suboptimal with it
    vector<vector<Point>> circuits;

    // store 1000 closest connections
    priority_queue<Pair, vector<Pair>, decltype(cmp)> pq(cmp);
    for (int i = 0; i < input.size(); i++) {
        for (int j = i + 1; j < input.size(); j++) {
            Pair p = { i, j, calculateDist(input[i], input[j]) };
            pq.push(p);

            if (pq.size() > MAX_CONNECTIONS) {
                pq.pop();
            }
        }
    }
    vector<Pair> sortedPairs;
    while(!pq.empty()) {
        sortedPairs.push_back(pq.top());
        pq.pop();
    }
    reverse(sortedPairs.begin(), sortedPairs.end());

    // now try to create a DSU
    DSU dsu(input.size());
    for (auto &p : sortedPairs) {
        dsu.unite(p.i1, p.i2);
    }

    // next try to create map of top connections
    map<int, int> rootSizes;
    for (int i = 0; i < input.size(); i++) {
        int root = dsu.find(i);
        rootSizes[root]++;
    }

    vector<int> sizes;
    for (auto r: rootSizes) {
        sizes.push_back(r.second);
    }
    sort(sizes.begin(), sizes.end(), revcmp);

    cout << "sol 1 : " << sizes[0] * sizes[1] * sizes[2] << endl;
    return;
}


void solve2(vector<Point> input) {
    int sum = 0;
    // storage of close stuff is only 1000 max, we can be suboptimal with it
    vector<vector<Point>> circuits;

    // store 1000 closest connections
    priority_queue<Pair, vector<Pair>, decltype(cmp)> pq(cmp);
    for (int i = 0; i < input.size(); i++) {
        for (int j = i + 1; j < input.size(); j++) {
            Pair p = { i, j, calculateDist(input[i], input[j]) };
            pq.push(p);
        }
    }
    vector<Pair> sortedPairs;
    while(!pq.empty()) {
        sortedPairs.push_back(pq.top());
        pq.pop();
    }
    reverse(sortedPairs.begin(), sortedPairs.end());

    // now try to create a DSU
    DSU dsu(input.size());

    for (auto &p : sortedPairs) {
        dsu.unite(p.i1, p.i2);
        if (dsu.allConnected()) {
            cout << "sol 2 : " << input[p.i1].x * input[p.i2].x << endl;
            break;
        }
    }

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
