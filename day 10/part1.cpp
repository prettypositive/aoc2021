#include <chrono>
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

char check_line(const string& line) {
    vector<char> stack;
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
    return killer;
}

int solve_puzzle() {
    auto lines = parse_input();
    int score = 0;
    unordered_map<char, int> scores = {
        {'0', 0}, {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
    for (const auto& line : lines) {
        char killer = check_line(line);
        score += scores[killer];
    }
    return score;
}

int main() {
    auto start = chrono::steady_clock::now();
    cout << solve_puzzle();
    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}