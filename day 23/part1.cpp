#include <bits/stdc++.h>

auto parse_input() {
    std::ifstream input("input.txt");
    input.close();
    return "solved it by hand lol";
}

auto solve_puzzle() {
    auto solution = parse_input();
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