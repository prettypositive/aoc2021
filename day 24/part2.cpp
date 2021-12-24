#include <bits/stdc++.h>

struct Instruction {
    bool literal = true;
    std::string op;
    char a;
    int b;
};

class ALU {
    std::array<int, 4> registers = {};

    void inp(const char a, const int b) { registers[a - 'w'] = b; }
    void add(const char a, const int b) { registers[a - 'w'] += b; }
    void mul(const char a, const int b) { registers[a - 'w'] *= b; }
    void div(const char a, const int b) { registers[a - 'w'] /= b; }
    void mod(const char a, const int b) { registers[a - 'w'] %= b; }
    void eql(const char a, const int b) {
        registers[a - 'w'] = (registers[a - 'w'] == b) ? 1 : 0;
    }

   public:
    int run(std::vector<Instruction> code, std::array<int, 14> input) {
        int i = 0;
        for (auto& instruction : code) {
            if (!instruction.literal)
                instruction.b = registers[instruction.b - 'w'];
            if (instruction.op == "inp") {
                if (input[i] == 0) break;
                inp(instruction.a, input[i]);
                i++;
            }
            if (instruction.op == "add") add(instruction.a, instruction.b);
            if (instruction.op == "mul") mul(instruction.a, instruction.b);
            if (instruction.op == "div") div(instruction.a, instruction.b);
            if (instruction.op == "mod") mod(instruction.a, instruction.b);
            if (instruction.op == "eql") eql(instruction.a, instruction.b);
        }
        int result = registers[3];
        registers.fill(0);
        return result;
    }
};

auto parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    std::vector<Instruction> code;
    while (getline(input, line)) {
        Instruction instruction;
        instruction.op = line.substr(0, 3);
        instruction.a = line[4];
        if (instruction.op == "inp") {
            code.push_back(instruction);
            continue;
        }
        if (std::isalpha(line[6])) {
            instruction.literal = false;
            instruction.b = line[6];

        } else {
            instruction.b = std::stoi(line.substr(6, std::string::npos));
        }
        code.push_back(instruction);
    }
    return code;
}

auto test(int i) {
    int x, y;
    y = i;
    x = y % 26;
    if (!(x > 6 && x < 16)) return false;
    y /= 26;
    x = y % 26;
    if (!(x > 10 && x < 20)) return false;
    y /= 26;
    x = y % 26;
    if (!(x > 12 && x < 22)) return false;
    y /= 26;
    x = y % 26;
    if (!(x > 3 && x < 13)) return false;
    y /= 26;
    x = y % 26;
    if (!(x > 5 && x < 15)) return false;
    y /= 26;
    if (y == 0) return true;
    return false;
}

auto last_five(int i) {
    int x, y;
    std::string five;
    y = i;
    x = y % 26;
    five += std::to_string(x - 6);
    y /= 26;
    x = y % 26;
    five += std::to_string(x - 10);
    y /= 26;
    x = y % 26;
    five += std::to_string(x - 12);
    y /= 26;
    x = y % 26;
    five += std::to_string(x - 3);
    y /= 26;
    x = y % 26;
    five += std::to_string(x - 5);
    return five;
}

std::string brute_force(ALU& alu, std::vector<Instruction>& code) {
    std::array<int, 14> input = {};
    std::vector<std::string> results;
    for (int i = 111'111'111; i < 1'000'000'000; i++) {
        if (!(i % 1'000'000)) std::cout << i << std::endl;
        std::string x = std::to_string(i);
        if (x.find('0') != std::string::npos) continue;
        input[0] = x[0] - '0';
        input[1] = x[1] - '0';
        input[2] = x[2] - '0';
        input[3] = x[3] - '0';
        input[4] = x[4] - '0';
        input[5] = x[5] - '0';
        input[6] = x[6] - '0';
        input[7] = x[7] - '0';
        input[8] = x[8] - '0';
        int result = alu.run(code, input);
        if (test(result)) {
            x += last_five(result);
            return x;
        }
    }
    return "";
}

auto solve_puzzle() {
    auto code = parse_input();
    ALU alu;
    auto result = brute_force(alu, code);
    return result;
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