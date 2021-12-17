#include <bits/stdc++.h>

std::tuple<int, int, int, int> parse_input() {
    std::ifstream input("input.txt");
    std::stringstream buffer;
    buffer << input.rdbuf();
    input.close();
    std::string raw(buffer.str());
    std::regex regex("^.+x=(-?\\d+)\\.\\.(-?\\d+), y=(-?\\d+)\\.\\.(-?\\d+)$");
    std::smatch matches;
    std::regex_match(raw, matches, regex);
    int x_min = stoi(matches[1]), x_max = stoi(matches[2]);
    int y_min = stoi(matches[3]), y_max = stoi(matches[4]);
    return {x_min, x_max, y_min, y_max};
}

auto timestep(int& x_pos, int& y_pos, int& x_vel, int& y_vel) {
    x_pos += x_vel;
    y_pos += y_vel;
    if (x_vel != 0) x_vel -= 1;
    y_vel -= 1;
}

auto solve_puzzle() {
    auto [x_min, x_max, y_min, y_max] = parse_input();
    int solution = 0;
    for (int i = 0; i < x_max + 1; i++) {
        for (int j = y_min; j < std::abs(y_min); j++) {
            int x_vel = i, y_vel = j;
            int x_pos = 0, y_pos = 0;
            while ((x_pos <= x_max) && (y_pos >= y_min)) {
                timestep(x_pos, y_pos, x_vel, y_vel);
                if ((x_pos >= x_min) && (y_pos >= y_min) && (x_pos <= x_max) &&
                    (y_pos <= y_max)) {
                    solution += 1;
                    break;
                }
            }
        }
    }
    return solution;
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