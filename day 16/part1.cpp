#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>

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

int parse_packet(std::string& packet, std::string::iterator it,
                 int& version_sum) {
    auto start = it;
    int version = stoi(std::string(it, it + 3), nullptr, 2);
    version_sum += version;
    it += 3;
    int packet_type = stoi(std::string(it, it + 3), nullptr, 2);
    it += 3;
    if (packet_type == 4) {
        std::string number_bits;
        while (*it == '1') {
            number_bits += std::string(it + 1, it + 5);
            it += 5;
        }
        number_bits += std::string(it + 1, it + 5);
        it += 5;
        int64_t number = stoll(number_bits, nullptr, 2);

    } else if (*it == '0') {
        it += 1;
        int total_bits = stoi(std::string(it, it + 15), nullptr, 2);
        it += 15;
        int used_bits = 0;
        std::string packets(it, it + total_bits);
        while (used_bits < total_bits) {
            used_bits += parse_packet(packets, packets.begin(), version_sum);
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
            used_bits += parse_packet(packets, packets.begin(), version_sum);
            packets.assign(it + used_bits, packet.end());
        }
        it += used_bits;
    }
    return (it - start);
}

auto solve_puzzle() {
    auto packet = parse_input();
    int version_sum = 0;
    parse_packet(packet, packet.begin(), version_sum);
    return version_sum;
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