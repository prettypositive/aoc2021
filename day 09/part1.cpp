#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y);
    }

    struct hash {
        size_t operator()(const Point& point) const {
            size_t x_hash = std::hash<int>()(point.x);
            size_t y_hash = std::hash<int>()(point.y);
            return (x_hash ^ y_hash);
        }
    };
};

auto parse_input() {
    ifstream input("input.txt");
    istream_iterator<string> start(input), end;
    vector<string> raw(start, end);
    unordered_map<Point, int, Point::hash> points;
    for (int y = 0; y < raw.size(); y++) {
        for (int x = 0; x < raw[0].size(); x++) {
            Point point(x, y);
            points[point] = (raw[y][x] - '0');
        }
    }
    return points;
}

auto find_neighbors(const Point& point,
                    const unordered_map<Point, int, Point::hash>& points) {
    vector<Point> possible = {
        Point(point.x, point.y + 1),
        Point(point.x, point.y - 1),
        Point(point.x + 1, point.y),
        Point(point.x - 1, point.y),
    };
    vector<Point> neighbors;
    for (const auto& point : possible) {
        if (points.contains(point)) neighbors.push_back(point);
    }
    return neighbors;
}

int solve_puzzle() {
    auto points = parse_input();
    int solution = 0;
    for (const auto& [point, value] : points) {
        auto neighbors = find_neighbors(point, points);
        int less = 0;
        for (const auto& neighbor : neighbors) {
            if (points[neighbor] > value) less += 1;
        }
        if (less == neighbors.size()) solution += (value + 1);
    }
    return solution;
}

int main() {
    auto start = chrono::steady_clock::now();
    cout << solve_puzzle();
    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}