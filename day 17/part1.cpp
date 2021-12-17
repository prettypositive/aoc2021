#include <bits/stdc++.h>

auto parse_input() {
    std::ifstream input("input.txt");
    std::stringstream buffer;
    buffer << input.rdbuf();
    input.close();
    std::string raw(buffer.str());
    std::regex regex("^.+x=(-?\\d+)\\.\\.(-?\\d+), y=(-?\\d+)\\.\\.(-?\\d+)$");
    std::smatch matches;
    std::regex_match(raw, matches, regex);
    return stoi(matches[3]);
}

auto solve_puzzle() {
    auto y_min = parse_input();
    int solution = 0;
    for (int i=1; i<std::abs(y_min);i++) {
        solution += i;
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