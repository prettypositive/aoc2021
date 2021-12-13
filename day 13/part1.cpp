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

auto fold_vertical(points_t& points, const int line) {
    for (const auto& point : points) {
        if (point.y < line) continue;
        Point new_point;
        new_point.x = point.x;
        new_point.y = line - (point.y - line);
        points.insert(new_point);
        points.erase(point);
    }
}

auto fold_horizontal(points_t& points, const int line) {
    for (const auto& point : points) {
        if (point.x < line) continue;
        Point new_point;
        new_point.y = point.y;
        new_point.x = line - (point.x - line);
        points.insert(new_point);
        points.erase(point);
    }
}

auto solve_puzzle() {
    auto [points, folds] = parse_input();
    int solution = 0;
    if (folds[0].axis == 'y')
        fold_vertical(points, folds[0].line);
    else if (folds[0].axis == 'x')
        fold_horizontal(points, folds[0].line);
    return points.size();
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