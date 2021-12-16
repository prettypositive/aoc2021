#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

std::string hex_char_to_binary(char c) {
    switch (c) {
        case '0':
            return "0000";
        case '1':
            return "0001";
        case '2':
            return "0010";
        case '3':
            return "0011";
        case '4':
            return "0100";
        case '5':
            return "0101";
        case '6':
            return "0110";
        case '7':
            return "0111";
        case '8':
            return "1000";
        case '9':
            return "1001";
        case 'A':
            return "1010";
        case 'B':
            return "1011";
        case 'C':
            return "1100";
        case 'D':
            return "1101";
        case 'E':
            return "1110";
        case 'F':
            return "1111";
    }
    return "";
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

std::tuple<int64_t, int> parse_packet(const std::string& packet) {
    auto start = packet.begin(), it = start;
    int64_t value;
    std::vector<int64_t> values;
    int version = stoi(std::string(it, it + 3), nullptr, 2);
    int packet_type = stoi(std::string(it + 3, it + 6), nullptr, 2);
    it += 6;

    if (packet_type == Op::LITERAL) {
        std::string number_bits;
        while (*it == '1') {
            number_bits += std::string(it + 1, it + 5);
            it += 5;
        }
        number_bits += std::string(it + 1, it + 5);
        it += 5;
        value = stoll(number_bits, nullptr, 2);

    } else if (*it == '0') {
        int total_bits = stoi(std::string(it + 1, it + 16), nullptr, 2);
        it += 16;
        int used_bits = 0;
        std::string packets(it, it + total_bits);
        while (used_bits < total_bits) {
            auto [value, length] = parse_packet(packets);
            used_bits += length;
            values.push_back(value);
            packets.assign(it + used_bits, it + total_bits);
        }
        it += total_bits;

    } else if (*it == '1') {
        int total_packets = stoi(std::string(it + 1, it + 12), nullptr, 2);
        it += 12;
        int used_bits = 0;
        std::string packets(it, packet.end());
        for (int i = 0; i < total_packets; i++) {
            auto [value, length] = parse_packet(packets);
            used_bits += length;
            values.push_back(value);
            packets.assign(it + used_bits, packet.end());
        }
        it += used_bits;
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

    auto length = it - start;
    return {value, length};
}

auto solve_puzzle() {
    auto packet = parse_input();
    auto [solution, _] = parse_packet(packet);
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