#include <algorithm>
#include <bitset>
#include <cctype>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

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

auto handle_bits_length(std::string::iterator& it);
auto handle_packets_length(std::string::iterator& it);
int64_t parse_packet(std::string::iterator& it);

std::string hex_char_to_binary(const char c) {
    if (std::isalpha(c)) {
        return std::bitset<4>(c - 55).to_string();
    } else {
        return std::bitset<4>(c - '0').to_string();
    }
}

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

int64_t do_op(const std::vector<int64_t>& values, const int packet_type) {
    switch (packet_type) {
        case Op::SUM:
            return std::accumulate(values.begin(), values.end(), 0LL);
        case Op::PRODUCT:
            return std::accumulate(values.begin(), values.end(), 1LL,
                                   std::multiplies<int64_t>());
        case Op::MINIMUM:
            return *std::min_element(values.begin(), values.end());
        case Op::MAXIMUM:
            return *std::max_element(values.begin(), values.end());
        case Op::GREATER:
            return (values[0] > values[1]) ? 1 : 0;
        case Op::LESS:
            return (values[0] < values[1]) ? 1 : 0;
        case Op::EQUAL:
            return (values[0] == values[1]) ? 1 : 0;
    }

    throw std::invalid_argument("invalid packet type");
}

int64_t read_value(std::string::iterator& it) {
    std::string value_bits;
    while (true) {
        value_bits += std::string(it + 1, it + 5);
        it += 5;
        if (*(it - 5) == '0') break;
    }
    return stoll(value_bits, nullptr, 2);
}

auto handle_bits_length(std::string::iterator& it) {
    std::vector<int64_t> values;
    int total_bits = stoi(std::string(it + 1, it + 16), nullptr, 2);
    it += 16;
    auto end = it + total_bits;
    while (it != end) {
        values.push_back(parse_packet(it));
    }
    return values;
}

auto handle_packets_length(std::string::iterator& it) {
    std::vector<int64_t> values;
    int total_packets = stoi(std::string(it + 1, it + 12), nullptr, 2);
    it += 12;
    for (int i = 0; i < total_packets; i++) {
        values.push_back(parse_packet(it));
    }
    return values;
}

int64_t parse_packet(std::string::iterator& it) {
    int version = stoi(std::string(it, it + 3), nullptr, 2);
    int packet_type = stoi(std::string(it + 3, it + 6), nullptr, 2);
    it += 6;

    if (packet_type == Op::LITERAL)
        return read_value(it);
    else if (*it == LengthType::BITS)
        return do_op(handle_bits_length(it), packet_type);
    else if (*it == LengthType::PACKETS)
        return do_op(handle_packets_length(it), packet_type);

    throw std::invalid_argument("malformed packet");
}

auto solve_puzzle() {
    auto packet = parse_input();
    auto it = packet.begin();
    auto solution = parse_packet(it);
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