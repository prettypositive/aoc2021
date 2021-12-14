#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

using rules_t = std::unordered_map<char, std::unordered_map<char, char>>;

std::tuple<std::string, rules_t> parse_input() {
    std::ifstream input("input.txt");
    std::string polymer;
    std::getline(input, polymer);

    std::string pair, delimiter, insert;
    rules_t rules;
    while (input >> pair >> delimiter >> insert) {
        rules[pair[0]][pair[1]] = insert[0];
    }
    input.close();
    return {polymer, rules};
}

auto apply_rules(rules_t& rules, const std::string& polymer) {
    std::string new_pairs, new_polymer;
    for (int i = 1; i < polymer.size(); i++) {
        new_pairs += rules[polymer[i - 1]][polymer[i]];
    }
    for (int i = 0; i < polymer.size() - 1; i++) {
        new_polymer += polymer[i];
        new_polymer += new_pairs[i];
    }
    new_polymer += polymer.back();
    return new_polymer;
}

auto solve_puzzle() {
    auto [polymer, rules] = parse_input();
    for (int i = 0; i < 10; i++) {
        polymer = apply_rules(rules, polymer);
    }

    std::unordered_map<char, int> counter;
    for (const auto& character : polymer) {
        counter[character] += 1;
    }
    std::vector<int> counts;
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