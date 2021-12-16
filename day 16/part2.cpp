#include <algorithm>
#include <bitset>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

std::string hex_char_to_binary(char c) {
    if (std::isalpha(c)) {
        return std::bitset<4>(c - 55).to_string();
    } else {
        return std::bitset<4>(c - '0').to_string();
    }
}

enum Op {
    SUM = 0,
    PRODUCT = 1,
    MINIMUM = 2,
    MAXIMUM = 3,
    LITERAL = 4,
    GREATER = 5,
    LESS = 6,
    EQUAL = 7
};

enum LengthType {
    BITS = '0',
    PACKETS = '1',
};

auto parse_input() {
    std::ifstream input("input.txt");
    char c;
    std::string packet;
    while (input >> c) {
        packet += hex_char_to_binary(c);
    }
    input.close();
    return packet;
}

int64_t parse_packet(const std::string& packet, std::string::iterator& it) {
    auto start = it;
    int64_t value;
    std::vector<int64_t> values;
    int version = stoi(std::string(it, it + 3), nullptr, 2);
    int packet_type = stoi(std::string(it + 3, it + 6), nullptr, 2);
    it += 6;

    if (packet_type == Op::LITERAL) {
        std::string value_bits;
        while (*it == '1') {
            value_bits += std::string(it + 1, it + 5);
            it += 5;
        }
        value_bits += std::string(it + 1, it + 5);
        it += 5;
        value = stoll(value_bits, nullptr, 2);

    } else if (*it == LengthType::BITS) {
        int total_bits = stoi(std::string(it + 1, it + 16), nullptr, 2);
        it += 16;
        auto end = it + total_bits;
        while (it != end) {
            values.push_back(parse_packet(packet, it));
        }

    } else if (*it == LengthType::PACKETS) {
        int total_packets = stoi(std::string(it + 1, it + 12), nullptr, 2);
        it += 12;
        for (int i = 0; i < total_packets; i++) {
            values.push_back(parse_packet(packet, it));
        }
    }

    if (packet_type == Op::SUM) {
        value = std::accumulate(values.begin(), values.end(), 0LL);
    } else if (packet_type == Op::PRODUCT) {
        value = std::accumulate(values.begin(), values.end(), 1LL,
                                std::multiplies<int64_t>());
    } else if (packet_type == Op::MINIMUM) {
        value = *std::min_element(values.begin(), values.end());
    } else if (packet_type == Op::MAXIMUM) {
        value = *std::max_element(values.begin(), values.end());
    } else if (packet_type == Op::GREATER) {
        value = (values[0] > values[1]) ? 1 : 0;
    } else if (packet_type == Op::LESS) {
        value = (values[0] < values[1]) ? 1 : 0;
    } else if (packet_type == Op::EQUAL) {
        value = (values[0] == values[1]) ? 1 : 0;
    }

    return value;
}

auto solve_puzzle() {
    auto packet = parse_input();
    auto it = packet.begin();
    auto solution = parse_packet(packet, it);
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