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

using points_t = unordered_map<Point, int, Point::hash>;

auto parse_input() {
    ifstream input("input.txt");
    istream_iterator<string> start(input), end;
    vector<string> raw(start, end);
    points_t points;
    for (int y = 0; y < raw.size(); y++) {
        for (int x = 0; x < raw[0].size(); x++) {
            Point point(x, y);
            points[point] = (raw[y][x] - '0');
        }
    }
    return points;
}

auto find_neighbors(const Point& point, const points_t& points) {
    // returns a vector of existing neighbors for a given point
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

void build_basin(const Point& point, points_t& points, vector<Point>& basin) {
    // recursively expands from a point until reaching an edge or a 9
    if (points[point] == 9) return;
    basin.push_back(point);
    auto neighbors = find_neighbors(point, points);
    for (const auto& neighbor : neighbors) {
        if (find(basin.begin(), basin.end(), neighbor) == basin.end())
            build_basin(neighbor, points, basin);
    }
    return;
}

auto is_low_point(const int value, const vector<Point>& neighbors,
                  points_t& points) {
    int less = 0;
    for (const auto& neighbor : neighbors) {
        if (points[neighbor] > value) less += 1;
    }
    return (less == neighbors.size());
}

int solve_puzzle() {
    auto points = parse_input();
    vector<Point> low_points;
    vector<vector<Point>> basins;
    for (const auto& [point, value] : points) {
        auto neighbors = find_neighbors(point, points);
        if (is_low_point(value, neighbors, points)) low_points.push_back(point);
    }
    for (const auto& point : low_points) {
        vector<Point> basin;
        build_basin(point, points, basin);
        basins.push_back(basin);
    }
    sort(basins.begin(), basins.end(),
         [](const vector<Point>& a, const vector<Point>& b) {
             return a.size() > b.size();
         });
    return (basins[0].size() * basins[1].size() * basins[2].size());
}

int main() {
    auto start = chrono::steady_clock::now();
    cout << solve_puzzle();
    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}