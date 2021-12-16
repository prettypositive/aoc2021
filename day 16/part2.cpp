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

std::tuple<int64_t, int> parse_packet(std::string& packet) {
    auto start = packet.begin();
    auto it = start;
    int64_t number;
    std::vector<int64_t> numbers;
    int packet_type = stoi(std::string(it + 3, it + 6), nullptr, 2);
    it += 6;
    if (packet_type == 4) {
        std::string number_bits;
        while (*it == '1') {
            number_bits += std::string(it + 1, it + 5);
            it += 5;
        }
        number_bits += std::string(it + 1, it + 5);
        it += 5;
        number = stoll(number_bits, nullptr, 2);
        return {number, (it - start)};

    } else if (*it == '0') {
        it += 1;
        int total_bits = stoi(std::string(it, it + 15), nullptr, 2);
        it += 15;
        int used_bits = 0;
        std::string packets(it, it + total_bits);
        while (used_bits < total_bits) {
            auto [number, new_used_bits] = parse_packet(packets);
            used_bits += new_used_bits;
            numbers.push_back(number);
            packets.assign(it + used_bits, it + total_bits);
        }
        it += total_bits;

    } else if (*it == '1') {
        it += 1;
        int total_packets = stoi(std::string(it, it + 11), nullptr, 2);
        it += 11;
        int used_bits = 0;
        std::string packets(it, packet.end());
        for (int i = 0; i < total_packets; i++) {
            auto [number, new_used_bits] = parse_packet(packets);
            used_bits += new_used_bits;
            numbers.push_back(number);
            packets.assign(it + used_bits, packet.end());
        }
        it += used_bits;
    }

    if (packet_type == 0) {
        number = std::accumulate(numbers.begin(), numbers.end(), 0LL);
    } else if (packet_type == 1) {
        number = std::accumulate(numbers.begin(), numbers.end(), 1LL,
                                 std::multiplies<int64_t>());
    } else if (packet_type == 2) {
        number = *std::min_element(numbers.begin(), numbers.end());
    } else if (packet_type == 3) {
        number = *std::max_element(numbers.begin(), numbers.end());
    } else if (packet_type == 5) {
        number = (numbers[0] > numbers[1]) ? 1 : 0;
    } else if (packet_type == 6) {
        number = (numbers[0] < numbers[1]) ? 1 : 0;
    } else if (packet_type == 7) {
        number = (numbers[0] == numbers[1]) ? 1 : 0;
    }

    return {number, (it - start)};
}

auto solve_puzzle() {
    auto packet = parse_input();
    auto [number, _] = parse_packet(packet);
    return number;
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