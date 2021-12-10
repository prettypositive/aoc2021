#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>
using namespace std;

auto parse_input() {
    ifstream input("input.txt");
    istream_iterator<string> start(input), end;
    vector<string> lines(start, end);
    return lines;
}

bool is_valid_line(const string& line, vector<char>& stack) {
    unordered_map<char, char> targets = {
        {'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
    char target, killer = '0';
    for (const auto& character : line) {
        switch (character) {
            case '(':
            case '[':
            case '{':
            case '<':
                target = targets[character];
                stack.push_back(character);
                break;
            case ')':
            case ']':
            case '}':
            case '>':
                if (character != target) {
                    killer = character;
                } else {
                    stack.pop_back();
                    target = targets[stack.back()];
                }
                break;
        }
        if (killer != '0') break;
    }
    return (killer == '0') ? true : false;
}

int64_t score_line(const string& line, const vector<char>& stack) {
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
        vector<char> stack;
        if (is_valid_line(line, stack)) {
            auto score = score_line(line, stack);
            scores.push_back(score);
        }
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