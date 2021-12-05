#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <utility>
#include <vector>
using namespace std;

using line_t = pair<pair<int, int>, pair<int, int>>;

auto parse_input() {
    ifstream input("input.txt");
    string x1y1, x2y2;
    string delimiter;
    vector<line_t> lines;
    while (input >> x1y1 >> delimiter >> x2y2) {
        string rx1, ry1, rx2, ry2;
        stringstream ss(x1y1);
        getline(ss, rx1, ',');
        getline(ss, ry1, ',');
        stringstream ss2(x2y2);
        getline(ss2, rx2, ',');
        getline(ss2, ry2, ',');
        int x1 = stoi(rx1), y1 = stoi(ry1);
        int x2 = stoi(rx2), y2 = stoi(ry2);
        pair line = {make_pair(x1, y1), make_pair(x2, y2)};
        lines.push_back(line);
    }
    return lines;
}

bool is_orthogonal(const line_t& line) {
    return ((line.first.first == line.second.first) !=
            (line.first.second == line.second.second));
}

bool is_horizontal(const line_t& line) {
    return (line.first.second == line.second.second);
}

bool is_vertical(const line_t& line) {
    return (line.first.first == line.second.first);
}

int main() {
    auto start = chrono::steady_clock::now();

    auto lines = parse_input();
    vector<pair<int, int>> all_points;
    set<pair<int, int>> unique_points;
    for (const auto& line : lines) {
        int direction, length;
        if (is_horizontal(line)) {
            direction = (line.first.first < line.second.first) ? 1 : -1;
            length = abs(line.first.first - line.second.first) + 1;
            for (int i = 0; i < length; i++) {
                pair point = {(line.first.first + (direction * i)),
                              line.first.second};
                unique_points.insert(point);
                all_points.push_back(point);
            }
        } else if (is_vertical(line)) {
            direction = (line.first.second < line.second.second) ? 1 : -1;
            length = abs(line.first.second - line.second.second) + 1;
            for (int i = 0; i < length; i++) {
                pair point = {line.first.first,
                              (line.first.second + (direction * i))};
                unique_points.insert(point);
                all_points.push_back(point);
            }
        }
    }
    int duplicate_points = 0;
    for (const auto& point : unique_points) {
        if (count(all_points.begin(), all_points.end(), point) > 1)
            duplicate_points += 1;
    }
    cout << duplicate_points;

    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}