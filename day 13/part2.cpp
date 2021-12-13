#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <unordered_set>
#include <vector>

struct Point {
    int x;
    int y;

    Point() {}

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y);
    }

    struct hash {
        size_t operator()(const Point& point) const {
            int hash = 17;
            hash = hash * 31 + point.x;
            hash = hash * 31 + point.y;
            return hash;
        }
    };
};

struct Fold {
    char axis;
    int line;

    Fold() {}

    Fold(char axis, int line) {
        this->axis = axis;
        this->line = line;
    }
};

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << '(' << point.x << ',' << point.y << ')';
    return os;
}

std::ostream& operator<<(std::ostream& os, const Fold& fold) {
    os << fold.axis << '=' << fold.line;
    return os;
}

using points_t = std::unordered_set<Point, Point::hash>;

std::tuple<points_t, std::vector<Fold>> parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    points_t points;
    while (input >> line) {
        if (line == "fold") break;
        std::stringstream ss(line);
        std::string x, y;
        std::getline(ss, x, ',');
        ss >> y;
        points.insert(Point(std::stoi(x), std::stoi(y)));
    }
    std::string word;
    std::vector<Fold> folds;
    while (input >> word) {
        if ((word[0] != 'x') && (word[0] != 'y')) continue;
        Fold fold;
        fold.axis = word[0];
        fold.line = std::stoi(word.substr(2, std::string::npos));
        folds.push_back(fold);
    }
    input.close();
    return {points, folds};
}

void fold_points(points_t& points, const Fold& fold) {
    points_t new_points;
    for (const auto& point : points) {
        Point new_point;
        if (fold.axis == 'y') {
            if (point.y < fold.line) {
                new_point = point;
            } else {
                new_point.x = point.x;
                new_point.y = fold.line - (point.y - fold.line);
            }
        } else if (fold.axis == 'x') {
            if (point.x < fold.line) {
                new_point = point;
            } else {
                new_point.y = point.y;
                new_point.x = fold.line - (point.x - fold.line);
            }
        }
        new_points.insert(new_point);
    }
    points = new_points;
}

auto convert_to_ascii(const points_t& points) {
    std::vector<std::string> output_v;
    int max_x, max_y;
    max_x = std::max_element(
                points.begin(), points.end(),
                [](const Point& a, const Point& b) { return a.x < b.x; })
                ->x;
    max_y = std::max_element(
                points.begin(), points.end(),
                [](const Point& a, const Point& b) { return a.y < b.y; })
                ->y;
    for (int i = 0; i < max_y + 1; i++) {
        std::string blank(max_x * 2, '.');
        output_v.push_back(blank);
    }
    for (const auto& point : points) {
        output_v[point.y][point.x * 2] = '#';
        output_v[point.y][point.x * 2 + 1] = '#';
    }
    std::string output;
    for (const auto& line : output_v) {
        output.append(line);
        output += '\n';
    }
    return output;
}
auto solve_puzzle() {
    auto [points, folds] = parse_input();
    int solution = 0;
    for (const auto& fold : folds) {
        fold_points(points, fold);
    }
    auto output = convert_to_ascii(points);
    return output;
}

int main() {
    auto start = std::chrono::steady_clock::now();
    std::cout << solve_puzzle();
    auto end = std::chrono::steady_clock::now();
    std::cout << " ["
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms]" << std::endl;
    return 0;
}