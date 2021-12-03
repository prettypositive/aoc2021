#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

auto parse_input() {
    ifstream input("input.txt");
    istream_iterator<string> start(input), end;
    vector<string> numbers(start, end);
    return numbers;
}

auto most_common_bit(const vector<string>& numbers, int position) {
    int zeroes = 0, ones = 0;
    for (const auto& number : numbers) {
        if (number[position] == '0')
            zeroes += 1;
        else if (number[position] == '1')
            ones += 1;
    }
    if (zeroes > ones)
        return '0';
    else
        return '1';
}
auto least_common_bit(const vector<string>& numbers, int position) {
    if (most_common_bit(numbers, position) == '0')
        return '1';
    else
        return '0';
}
auto find_rating(vector<string> numbers, const string& rating_type) {
    for (int i = 0; i < numbers[0].size(); i++) {
        char target;
        if (rating_type == "oxygen")
            target = most_common_bit(numbers, i);
        else if (rating_type == "co2")
            target = least_common_bit(numbers, i);
        vector<string> new_numbers;
        for (const auto& number : numbers) {
            if (number[i] == target) new_numbers.push_back(number);
        }
        numbers = new_numbers;
        if (numbers.size() == 1) break;
    }
    return numbers[0];
}

int main() {
    auto numbers = parse_input();
    string oxygen_rating = find_rating(numbers, "oxygen");
    string co2_rating = find_rating(numbers, "co2");
    cout << stoi(oxygen_rating, nullptr, 2) * stoi(co2_rating, nullptr, 2);
    return 0;
}