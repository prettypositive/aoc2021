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

auto count_bits(const vector<string>& numbers) {
    vector<int> counter;
    counter.resize(numbers[0].size());
    for (const string& number : numbers) {
        for (int i = 0; i < number.size(); i++) {
            if (number[i] == '1') counter[i] += 1;
        }
    }
    return counter;
}

void build_rates(const vector<int>& counter, const vector<string>& numbers,
                 string& gamma_rate, string& epsilon_rate) {
    for (const int& count : counter) {
        if (count > (numbers.size() / 2)) {
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