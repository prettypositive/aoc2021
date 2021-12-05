#include <boost/functional/hash.hpp>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
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
        stringstream ss(x1y1), ss2(x2y2);
        getline(ss, rx1, ',');
        getline(ss, ry1, ',');
        getline(ss2, rx2, ',');
        getline(ss2, ry2, ',');
        int x1 = stoi(rx1), y1 = stoi(ry1);
        int x2 = stoi(rx2), y2 = stoi(ry2);
        pair line = {make_pair(x1, y1), make_pair(x2, y2)};
        lines.push_back(line);
    }
    return lines;
}

bool is_horizontal(const line_t& line) {
    return (line.first.second == line.second.second);
}

bool is_vertical(const line_t& line) {
    return (line.first.first == line.second.first);
}

int slope(const line_t& line) {
    return ((line.second.second - line.first.second) /
            (line.second.first - line.first.first));
}

auto draw_lines(const vector<line_t>& lines) {
    unordered_map<pair<int, int>, int, boost::hash<pair<int, int>>> points;
    for (const auto& line : lines) {
        if (is_horizontal(line)) {
            int direction = (line.first.first < line.second.first) ? 1 : -1;
            int length = abs(line.first.first - line.second.first) + 1;
            for (int i = 0; i < length; i++) {
                pair point = {(line.first.first + (direction * i)),
                              line.first.second};
                points[point] += 1;
            }
        } else if (is_vertical(line)) {
            int direction = (line.first.second < line.second.second) ? 1 : -1;
            int length = abs(line.first.second - line.second.second) + 1;
            for (int i = 0; i < length; i++) {
                pair point = {line.first.first,
                              (line.first.second + (direction * i))};
                points[point] += 1;
            }
        } else if (slope(line) == 1) {
            int direction = (line.first.first < line.second.first) ? 1 : -1;
            int length = abs(line.first.first - line.second.first) + 1;
            for (int i = 0; i < length; i++) {
                pair point = {(line.first.first + (direction * i)),
                              (line.first.second + (direction * i))};
                points[point] += 1;
            }
        } else {
            int x_direction = (line.first.first < line.second.first) ? 1 : -1;
            int y_direction = -x_direction;
            int length = abs(line.first.first - line.second.first) + 1;
            for (int i = 0; i < length; i++) {
                pair point = {(line.first.first + (x_direction * i)),
                              (line.first.second + (y_direction * i))};
                points[point] += 1;
            }
        }
    }
    return points;
}

int main() {
    auto start = chrono::steady_clock::now();

    auto lines = parse_input();
    auto points = draw_lines(lines);
    int duplicate_points = 0;
    for (const auto& [point, value] : points) {
        if (value > 1) duplicate_points += 1;
    }
    cout << duplicate_points;

    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}