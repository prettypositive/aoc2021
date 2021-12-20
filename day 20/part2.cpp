#include <bits/stdc++.h>

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

using image_t = std::unordered_map<Point, char, Point::hash>;

std::tuple<std::vector<char>, image_t> parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    std::vector<char> lookup;
    input >> line;
    for (const auto& character : line) {
        (character == '#') ? lookup.push_back('1') : lookup.push_back('0');
    }
    std::vector<std::string> raw_image;
    while (input >> line) {
        raw_image.push_back(line);
    }
    image_t image;
    for (int i = 0; i < raw_image.size(); i++) {
        for (int j = 0; j < raw_image[0].size(); j++) {
            image[Point(j, i)] = (raw_image[i][j] == '#') ? '1' : '0';
        }
    }
    input.close();
    return {lookup, image};
}

auto enhance_image(const std::vector<char>& lookup, const image_t& image,
                   char parity) {
    std::vector<std::pair<int, int>> directions = {{-1, -1}, {0, -1}, {1, -1},
                                                   {-1, 0},  {0, 0},  {1, 0},
                                                   {-1, 1},  {0, 1},  {1, 1}};
    image_t new_image;
    std::vector<Point> second_pass;
    for (const auto& [point, value] : image) {
        std::string new_pixel;
        for (const auto& direction : directions) {
            Point neighbor(point.x + direction.first,
                           point.y + direction.second);
            if (!image.contains(neighbor)) {
                second_pass.push_back(neighbor);
                new_pixel += parity;
            } else {
                new_pixel += image.at(neighbor);
            }
        }
        new_image[point] = lookup[std::stoi(new_pixel, nullptr, 2)];
    }
    for (const auto& point : second_pass) {
        std::string new_pixel;
        for (const auto& direction : directions) {
            Point neighbor(point.x + direction.first,
                           point.y + direction.second);
            if (!image.contains(neighbor)) {
                new_pixel += parity;
            } else {
                new_pixel += image.at(neighbor);
            }
        }
        new_image[point] = lookup[std::stoi(new_pixel, nullptr, 2)];
    }
    return new_image;
}

auto solve_puzzle() {
    auto [lookup, image] = parse_input();
    for (int i = 0; i < 25; i++) {
        image = enhance_image(lookup, image, '0');
        image = enhance_image(lookup, image, '1');
    }
    int sum = 0;
    for (const auto& [_, value] : image) {
        sum += (value - '0');
    }
    return sum;
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