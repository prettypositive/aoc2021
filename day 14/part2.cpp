#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

using polymer_t = std::unordered_map<char, std::unordered_map<char, int64_t>>;
using rules_t = std::unordered_map<char, std::unordered_map<char, char>>;

std::tuple<char, polymer_t, rules_t> parse_input() {
    std::ifstream input("input.txt");
    std::string raw;
    std::getline(input, raw);
    char first = raw[0];  // first character doesn't get counted at the end
    polymer_t polymer;
    for (int i = 1; i < raw.size(); i++) {
        polymer[raw[i - 1]][raw[i]] += 1;
    }
    std::string pair, delimiter, insert;
    rules_t rules;
    while (input >> pair >> delimiter >> insert) {
        rules[pair[0]][pair[1]] = insert[0];
    }
    input.close();
    return {first, polymer, rules};
}

auto apply_rules(rules_t& rules, const polymer_t& polymer) {
    polymer_t new_polymer;
    for (const auto& [key, value] : polymer) {
        for (const auto& [key2, value2] : value) {
            auto insert = rules[key][key2];
            new_polymer[key][insert] += value2;
            new_polymer[insert][key2] += value2;
        }
    }
    return new_polymer;
}

auto solve_puzzle() {
    auto [first, polymer, rules] = parse_input();
    for (int i = 0; i < 40; i++) {
        polymer = apply_rules(rules, polymer);
    }
    std::unordered_map<char, int64_t> counter;
    counter[first] += 1;  // can't think of a better way to fix this
    for (const auto& [key, value] : polymer) {
        for (const auto& [key2, value2] : value) {
            counter[key2] += value2;
        }
    }
    std::vector<int64_t> counts;
    for (const auto& [key, value] : counter) {
        counts.push_back(value);
    }
    std::sort(counts.begin(), counts.end());

    return (counts.back() - counts.front());
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