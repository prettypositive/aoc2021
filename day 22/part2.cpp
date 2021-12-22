#include <bits/stdc++.h>

struct Polygon {
    std::pair<int64_t, int64_t> x;
    std::pair<int64_t, int64_t> y;
    std::pair<int64_t, int64_t> z;
    bool on;
};

auto parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    std::regex regex(
        "^(on|off) x=(-?\\d+)\\.\\.(-?\\d+),"
        "y=(-?\\d+)\\.\\.(-?\\d+),"
        "z=(-?\\d+)\\.\\.(-?\\d+)$");
    std::vector<Polygon> steps;
    while (getline(input, line)) {
        std::smatch matches;
        std::regex_match(line, matches, regex);
        Polygon cube;
        cube.on = (matches[1] == "on") ? true : false;
        cube.x.first = stoll(matches[2]);
        cube.x.second = stoll(matches[3]);
        cube.y.first = stoll(matches[4]);
        cube.y.second = stoll(matches[5]);
        cube.z.first = stoll(matches[6]);
        cube.z.second = stoll(matches[7]);
        steps.push_back(cube);
    }
    return steps;
}

auto subtract_polygons(Polygon lhs, const Polygon& rhs) {
    // returns up to 4 polygons that construct the new area of lhs after being
    // intersected with rhs, returns original polygon if no intersection
    std::vector<Polygon> result;
    std::unordered_set<std::string> overlaps;
    if (lhs.x.first >= rhs.x.first && lhs.x.first <= rhs.x.second)
        overlaps.insert("x_min");
    if (lhs.x.second >= rhs.x.first && lhs.x.second <= rhs.x.second)
        overlaps.insert("x_max");
    if (lhs.y.first >= rhs.y.first && lhs.y.first <= rhs.y.second)
        overlaps.insert("y_min");
    if (lhs.y.second >= rhs.y.first && lhs.y.second <= rhs.y.second)
        overlaps.insert("y_max");
    if (lhs.z.first >= rhs.z.first && lhs.z.first <= rhs.z.second)
        overlaps.insert("z_min");
    if (lhs.z.second >= rhs.z.first && lhs.z.second <= rhs.z.second)
        overlaps.insert("z_max");
    // overlap possibilities:
    // 0 overlaps - return original
    // 3 overlaps, 1 of each axis - return 3 polygons
    // 4 overlaps, 1-1-2 - return 2 polygons
    // 5 overlaps, 1-2-2 - return 1 polygon
    // 6 overlaps - return empty vector
    if (overlaps.size() == 0) {
        result.push_back(lhs);
    } else if (overlaps.size() == 3) {
        Polygon polygon = lhs;
        if (overlaps.contains("z_min")) {
            polygon.z.first = rhs.z.second + 1;
            lhs.z.second = rhs.z.second;
        } else if (overlaps.contains("z_max")) {
            polygon.z.second = rhs.z.first - 1;
            lhs.z.first = rhs.z.first;
        }
        result.push_back(polygon);
        polygon = lhs;
        if (overlaps.contains("y_min")) {
            polygon.y.first = rhs.y.second + 1;
            lhs.y.second = rhs.y.second;
        } else if (overlaps.contains("y_max")) {
            polygon.y.second = rhs.y.first - 1;
            lhs.y.first = rhs.y.first;
        }
        result.push_back(polygon);
        polygon = lhs;
        if (overlaps.contains("x_min")) {
            polygon.x.first = rhs.x.second + 1;
        } else if (overlaps.contains("x_max")) {
            polygon.x.second = rhs.x.first - 1;
        }
        result.push_back(polygon);
    } else if (overlaps.size() == 4) {
        Polygon polygon;
        if (overlaps.contains("z_min") && overlaps.contains("z_max")) {
            polygon = lhs;
            if (overlaps.contains("y_min")) {
                polygon.y.first = rhs.y.second + 1;
                lhs.y.second = rhs.y.second;
            } else if (overlaps.contains("y_max")) {
                polygon.y.second = rhs.y.first - 1;
                lhs.y.first = rhs.y.first;
            }
            result.push_back(polygon);
            polygon = lhs;
            if (overlaps.contains("x_min")) {
                polygon.x.first = rhs.x.second + 1;
            } else if (overlaps.contains("x_max")) {
                polygon.x.second = rhs.x.first - 1;
            }
            result.push_back(polygon);
        } else if (overlaps.contains("y_min") && overlaps.contains("y_max")) {
            polygon = lhs;
            if (overlaps.contains("z_min")) {
                polygon.z.first = rhs.z.second + 1;
                lhs.z.second = rhs.z.second;
            } else if (overlaps.contains("z_max")) {
                polygon.z.second = rhs.z.first - 1;
                lhs.z.first = rhs.z.first;
            }
            result.push_back(polygon);
            polygon = lhs;
            if (overlaps.contains("x_min")) {
                polygon.x.first = rhs.x.second + 1;
            } else if (overlaps.contains("x_max")) {
                polygon.x.second = rhs.x.first - 1;
            }
            result.push_back(polygon);
        } else if (overlaps.contains("x_min") && overlaps.contains("x_max")) {
            polygon = lhs;
            if (overlaps.contains("z_min")) {
                polygon.z.first = rhs.z.second + 1;
                lhs.z.second = rhs.z.second;
            } else if (overlaps.contains("z_max")) {
                polygon.z.second = rhs.z.first - 1;
                lhs.z.first = rhs.z.first;
            }
            result.push_back(polygon);
            polygon = lhs;
            if (overlaps.contains("y_min")) {
                polygon.y.first = rhs.y.second + 1;
            } else if (overlaps.contains("y_max")) {
                polygon.y.second = rhs.y.first - 1;
            }
            result.push_back(polygon);
        }
    } else if (overlaps.size() == 5) {
        Polygon polygon = lhs;
        if (!overlaps.contains("z_min")) {
            polygon.z.second = rhs.z.first - 1;
        } else if (!overlaps.contains("z_max")) {
            polygon.z.first = rhs.z.second + 1;
        } else if (!overlaps.contains("y_min")) {
            polygon.y.second = rhs.y.first - 1;
        } else if (!overlaps.contains("y_max")) {
            polygon.y.first = rhs.y.second + 1;
        } else if (!overlaps.contains("x_min")) {
            polygon.x.second = rhs.x.first - 1;
        } else if (!overlaps.contains("x_max")) {
            polygon.x.first = rhs.x.second + 1;
        }
        result.push_back(polygon);
    }

    return result;
}

auto solve_puzzle() {
    auto steps = parse_input();
    std::vector<Polygon> grid;
    for (const auto& step : steps) {
        std::vector<Polygon> new_grid;
        for (const auto& polygon : grid) {
            for (const auto& new_polygon : subtract_polygons(polygon, step)) {
                new_grid.push_back(new_polygon);
            }
        }
        if (step.on) new_grid.push_back(step);
        grid = new_grid;
    }
    int64_t total_area;
    for (const auto& polygon : grid) {
        int64_t area = (((polygon.x.second - polygon.x.first) + 1) *
                        ((polygon.y.second - polygon.y.first) + 1) *
                        ((polygon.z.second - polygon.z.first) + 1));
        total_area += area;
    }
    return total_area;
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