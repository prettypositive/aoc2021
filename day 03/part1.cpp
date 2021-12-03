#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

using namespace std;

auto parse_input() {
    ifstream input("input.txt");
    istream_iterator<string> start(input), end;
    vector<string> numbers(start, end);
    return numbers;
}

auto count_bits(const vector<string>& numbers) {
    map<int, int> counter;
    for (int i = 0; i < numbers[0].size(); i++) counter[i] = 0;
    for (const auto& number : numbers) {
        for (int i = 0; i < number.size(); i++) {
            if (number[i] == '1') counter[i] += 1;
        }
    }
    return counter;
}

void build_rates(const map<int, int>& counter, const vector<string>& numbers,
                 string& gamma_rate, string& epsilon_rate) {
    for (const auto& [key, value] : counter) {
        if (value > (numbers.size() / 2)) {
            gamma_rate.push_back('1');
            epsilon_rate.push_back('0');
        } else {
            gamma_rate.push_back('0');
            epsilon_rate.push_back('1');
        }
    }
}

int main() {
    auto numbers = parse_input();
    auto counter = count_bits(numbers);
    string gamma_rate, epsilon_rate;
    build_rates(counter, numbers, gamma_rate, epsilon_rate);
    cout << stoi(epsilon_rate, nullptr, 2) * stoi(gamma_rate, nullptr, 2);
    return 0;
}