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

struct Interval {
    long long start;
    long long end;
};

struct Input {
    vector<string> intervalStrings;
    vector<string> idStrings;
};

auto parseInput(char delimiter = '\n') {
    vector<string> intervalStrings, idStrings;

    string s;
    while(getline(cin, s, delimiter)) {
        if (s.empty()) break;
        intervalStrings.push_back(s);
    }
    while(getline(cin, s, delimiter)) {
        if (s.empty()) break;
        idStrings.push_back(s);
    }

    return Input { intervalStrings, idStrings };
}

Interval parseInterval(const string& s) {
    Interval interval;

    auto pos = s.find('-');
    if (pos != string::npos) {
        interval.start = stoll(s.substr(0, pos));
        interval.end = stoll(s.substr(pos + 1));
    }

    return interval;
}

void solve1(Input input) {
    int sum = 0;
    map<long long, long long> m;

    for (string s: input.intervalStrings) {
        Interval interval = parseInterval(s);
        if (m.find(interval.start) == m.end()) {
            m[interval.start] = interval.end;
        } else {
            if (m.at(interval.start) < interval.end) m[interval.start] = interval.end;
        }
    }

    // after getting the ranges, go thru inputs
    for (string s: input.idStrings) {
        long long id = stoll(s);

        for (auto r: m) {
            if (id < r.first) break;
            if (id <= r.second) {

                sum++;
                break;
            }
        }

    }

    cout << "sol 1 : " << sum << "\n";
    return;
}


void solve2(Input input) {
    long long sum = 0;
    vector<Interval> intervals;

    for (const string& intervalString: input.intervalStrings) {
        Interval newInterval = parseInterval(intervalString);

        int idxStart = -1, idxEnd = -1;
        bool startOverlaps = false, endOverlaps = false;

        for (int i = 0; i < intervals.size(); i++) {
            Interval current = intervals[i];


            if (!startOverlaps && newInterval.start >= current.start && newInterval.start <= current.end) {
                startOverlaps = true;
                idxStart = i;
            }
            if (!endOverlaps && newInterval.end >= current.start && newInterval.end <= current.end) {
                endOverlaps = true;
                idxEnd = i;
            }
            if (startOverlaps && endOverlaps) break;
        }

        // case where no overlaps were found -> create new range in the list
        if (!startOverlaps && !endOverlaps) {
            intervals.push_back({ newInterval.start, newInterval.end });
            continue;
        }

        // case where overlaps for both sides were found
        if (startOverlaps && endOverlaps) {
            // if the ranges are the same, don't do anything
            if (idxStart == idxEnd) continue;
            // otherwise remove both from the ranges array and then add a new one that's the combination of the two
            Interval combined = { intervals[idxStart].start, intervals[idxEnd].end };
            intervals.erase(intervals.begin() + max(idxStart, idxEnd));
            intervals.erase(intervals.begin() + min(idxStart, idxEnd));
            intervals.push_back(combined);
            continue;
        }
        // otherwise there's only changes for one or the other
        if (startOverlaps && intervals[idxStart].end < newInterval.end) {
            intervals[idxStart] = { intervals[idxStart].start, newInterval.end };
        }
        if (endOverlaps && intervals[idxEnd].start > newInterval.start) {
            intervals[idxEnd] = { newInterval.start, intervals[idxEnd].end };
        }
    }

    for (const Interval& interval: intervals) {
        sum += (interval.end - interval.start) + 1;
    }


    cout << "sol 2 : " << sum << "\n";
    return;
}


int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    #endif

    Input input = parseInput();

    solve1(input);
    solve2(input);
}
