#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <tuple>
#include <unordered_map>
#include <vector>
using namespace std;

auto parse_input() {
    ifstream input("input.txt");
    istream_iterator<string> start(input), end;
    vector<string> lines(start, end);
    return lines;
}

tuple<bool, vector<char>> parse_line(const string& line) {
    vector<char> stack;
    unordered_map<char, char> pairs = {
        {'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
    char target;
    bool valid = true;
    for (const auto& character : line) {
        if (pairs.contains(character)) {
            target = pairs[character];
            stack.push_back(character);
        } else {
            valid = (character == target);
            if (!valid) break;
            stack.pop_back();
            target = pairs[stack.back()];
        }
    }
    return {valid, stack};
}

auto score_line(const string& line, const vector<char>& stack) {
    unordered_map<char, int> score_lookup = {
        {'(', 1}, {'[', 2}, {'{', 3}, {'<', 4}};
    int64_t score = 0;
    for (auto it = stack.rbegin(); it < stack.rend(); it++) {
        score *= 5;
        score += score_lookup[*it];
    }
    return score;
}

int64_t solve_puzzle() {
    auto lines = parse_input();
    vector<int64_t> scores;
    for (const auto& line : lines) {
        auto [valid, stack] = parse_line(line);
        if (valid) scores.push_back(score_line(line, stack));
    }
    sort(scores.begin(), scores.end());
    return scores[scores.size() / 2];
}

int main() {
    auto start = chrono::steady_clock::now();
    cout << solve_puzzle();
    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}