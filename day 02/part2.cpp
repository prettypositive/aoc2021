#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

auto parse_input() {
    ifstream input("input.txt");
    vector<pair<string, int>> instructions;
    string direction;
    int amount;
    while (input >> direction >> amount) {
        pair<string, int> instruction = {direction, amount};
        instructions.push_back(instruction);
    }
    return instructions;
}

int main() {
    auto instructions = parse_input();
    int depth = 0, position = 0, aim = 0;
    for (const auto& instruction : instructions) {
        if (instruction.first == "up")
            aim -= instruction.second;
        else if (instruction.first == "down")
            aim += instruction.second;
        else if (instruction.first == "forward") {
            position += instruction.second;
            depth += (aim * instruction.second);
        }
    }
    cout << depth * position;
    return 0;
}