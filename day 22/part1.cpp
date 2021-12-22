#include <bits/stdc++.h>

struct Point {
    int x;
    int y;
    int z;

    Point() {}

    Point(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    bool operator==(const Point& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    struct hash {
        size_t operator()(const Point& point) const {
            int hash = 17;
            hash = hash * 31 + point.x;
            hash = hash * 31 + point.y;
            hash = hash * 31 + point.z;
            return hash;
        }
    };
};

struct Cube {
    std::unordered_map<std::string, int> bounds;
    bool on;
};

using grid_t = std::unordered_set<Point, Point::hash>;

auto parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    std::regex regex(
        "^(on|off) x=(-?\\d+)\\.\\.(-?\\d+),"
        "y=(-?\\d+)\\.\\.(-?\\d+),"
        "z=(-?\\d+)\\.\\.(-?\\d+)$");
    std::vector<Cube> cubes;
    while (getline(input, line)) {
        std::smatch matches;
        std::regex_match(line, matches, regex);
        Cube cube;
        cube.on = (matches[1] == "on") ? true : false;
        cube.bounds["x_min"] = stoi(matches[2]);
        cube.bounds["x_max"] = stoi(matches[3]);
        cube.bounds["y_min"] = stoi(matches[4]);
        cube.bounds["y_max"] = stoi(matches[5]);
        cube.bounds["z_min"] = stoi(matches[6]);
        cube.bounds["z_max"] = stoi(matches[7]);
        cubes.push_back(cube);
    }
    return cubes;
}

auto initialize_cube(grid_t& grid, Cube& cube) {
    for (int x = cube.bounds["x_min"]; x < cube.bounds["x_max"] + 1; x++) {
        for (int y = cube.bounds["y_min"]; y < cube.bounds["y_max"] + 1; y++) {
            for (int z = cube.bounds["z_min"]; z < cube.bounds["z_max"] + 1;
                 z++) {
                if (cube.on)
                    grid.insert(Point(x, y, z));
                else
                    grid.erase(Point(x, y, z));
            }
        }
    }
}

auto solve_puzzle() {
    auto cubes = parse_input();
    grid_t grid;
    for (auto& cube : cubes) {
        bool good = true;
        for (const auto& [_, value] : cube.bounds) {
            if ((value < -50) || (value > 50)) good = false;
        }
        if (good) initialize_cube(grid, cube);
    }
    return grid.size();
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