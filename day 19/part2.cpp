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

    void rotate(const char axis) {
        if (axis == 'x') {
            int new_y = -z, new_z = y;
            y = new_y;
            z = new_z;
        } else if (axis == 'y') {
            int new_x = z, new_z = -x;
            x = new_x;
            z = new_z;
        } else if (axis == 'z') {
            int new_x = -y, new_y = x;
            x = new_x;
            y = new_y;
        }
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

struct Scanner {
    Point position;
    std::vector<Point> beacons;
    std::vector<std::vector<Point>> all_orientations;
    std::vector<Point> fixed_beacons;
    bool fixed = false;

    void rotate_all_beacons(const char axis) {
        for (auto& beacon : beacons) {
            beacon.rotate(axis);
        }
    }

    void shift_all_beacons(const int x, const int y, const int z) {
        for (auto& beacon : beacons) {
            beacon.x += x;
            beacon.y += y;
            beacon.z += z;
        }
    }

    void generate_orientations() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                rotate_all_beacons('z');
                all_orientations.push_back(beacons);
            }
            rotate_all_beacons('x');
        }
        rotate_all_beacons('y');
        for (int j = 0; j < 4; j++) {
            rotate_all_beacons('z');
            all_orientations.push_back(beacons);
        }
        rotate_all_beacons('y');
        rotate_all_beacons('y');
        for (int j = 0; j < 4; j++) {
            rotate_all_beacons('z');
            all_orientations.push_back(beacons);
        }
        rotate_all_beacons('y');
    }
};

auto parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    Scanner scanner;
    std::vector<Scanner> scanners;
    getline(input, line);
    while (!input.eof()) {
        getline(input, line);
        if (line == "") continue;
        if (line[1] == '-') {
            scanner.generate_orientations();
            scanners.push_back(scanner);
            scanner.all_orientations.clear();
            scanner.beacons.clear();
            continue;
        }
        std::stringstream ss(line);
        std::string sx, sy, sz;
        getline(ss, sx, ',');
        getline(ss, sy, ',');
        ss >> sz;
        Point beacon(stoi(sx), stoi(sy), stoi(sz));
        scanner.beacons.push_back(beacon);
    }
    scanner.generate_orientations();
    scanners.push_back(scanner);
    input.close();
    return scanners;
}

bool all_scanners_fixed(const std::vector<Scanner>& scanners) {
    for (const auto& scanner : scanners) {
        if (!scanner.fixed) return false;
    }
    return true;
}

int distance(const Point& a, const Point& b) {
    return (abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z));
}

auto solve_puzzle() {
    std::srand(std::time(nullptr));
    auto scanners = parse_input();
    scanners[0].position = Point(0, 0, 0);
    scanners[0].fixed_beacons = scanners[0].beacons;
    scanners[0].fixed = true;
    while (!all_scanners_fixed(scanners)) {
        for (int i = 0; i < scanners.size(); i++) {
            if (!scanners[i].fixed) continue;
            for (auto& scanner : scanners) {
                if (scanner.fixed) continue;
                for (const auto& orientation : scanner.all_orientations) {
                    scanner.beacons = orientation;
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> distrib(
                        0, scanners[i].fixed_beacons.size() - 1);
                    int j = distrib(gen);
                    int k = distrib(gen);
                    int x_diff =
                        scanners[i].fixed_beacons[j].x - scanner.beacons[k].x;
                    int y_diff =
                        scanners[i].fixed_beacons[j].y - scanner.beacons[k].y;
                    int z_diff =
                        scanners[i].fixed_beacons[j].z - scanner.beacons[k].z;
                    scanner.shift_all_beacons(x_diff, y_diff, z_diff);
                    std::unordered_set<Point, Point::hash> cmp(
                        scanners[i].fixed_beacons.begin(),
                        scanners[i].fixed_beacons.end());
                    int matches = 0;
                    for (const auto& beacon : scanner.beacons) {
                        if (cmp.contains(beacon)) matches += 1;
                    }
                    if (matches >= 12) {
                        scanner.position = Point(x_diff, y_diff, z_diff);
                        scanner.fixed_beacons = scanner.beacons;
                        scanner.fixed = true;
                        break;
                    }
                    scanner.shift_all_beacons(-x_diff, -y_diff, -z_diff);
                }
            }
        }
    }

    std::vector<int> distances;
    for (int i = 0; i < scanners.size(); i++) {
        for (int j = 0; j < scanners.size(); j++) {
            if (i == j) continue;
            distances.push_back(
                distance(scanners[i].position, scanners[j].position));
        }
    }
    return *std::max_element(distances.begin(), distances.end());
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