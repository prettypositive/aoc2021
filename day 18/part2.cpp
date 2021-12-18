#include <bits/stdc++.h>

using number_t = std::pair<std::string::iterator, std::string::iterator>;
using pair_t = std::pair<std::string::iterator, std::string::iterator>;

number_t find_right_number(std::string& sum, std::string::iterator it);
number_t find_left_number(std::string& sum, std::string::iterator it);

auto parse_input() {
    std::ifstream input("input.txt");
    std::istream_iterator<std::string> start(input), end;
    std::vector<std::string> snail_numbers(start, end);
    input.close();
    return snail_numbers;
}

pair_t find_too_deep(std::string& sum) {
    // returns a pair of iterators to the first 5-deep pair
    int depth = 0;
    for (auto it = sum.begin(); it != sum.end(); ++it) {
        if (*it == '[')
            depth += 1;
        else if (*it == ']')
            depth -= 1;

        if (depth == 5) {
            auto start = it;
            while (*it != ']') {
                it += 1;
            }
            return {start, it};
        }
    }

    return {sum.end(), sum.end()};
}

number_t find_too_large(std::string& sum) {
    // returns a pair of iterators to the beginning and end of the first num >9
    number_t number = {sum.begin(), sum.begin()};
    while (number.first != sum.end()) {
        number = find_right_number(sum, number.second);
        if (number.first != number.second) return number;
    }
    return {sum.end(), sum.end()};
}

number_t find_right_number(std::string& sum, std::string::iterator it) {
    // returns a pair of iterators to the beginning and end of the first number
    // to the right of given iterator
    it += 1;
    while (it != sum.end()) {
        auto start = it;
        auto end = it;
        while (std::isdigit(*it)) {
            end = it;
            it += 1;
        }
        if (std::isdigit(*start)) return {start, end};
        it += 1;
    }
    return {sum.end(), sum.end()};
}

number_t find_left_number(std::string& sum, std::string::iterator it) {
    // returns a pair of iterators to the beginning and end of the first
    // number to the left of given iterator
    it -= 1;
    while (it != sum.begin()) {
        auto start = it;
        auto end = it;
        while (std::isdigit(*it)) {
            start = it;
            it -= 1;
        }
        if (std::isdigit(*start)) return {start, end};
        it -= 1;
    }
    return {sum.begin(), sum.begin()};
}

auto explode_pair(std::string& sum, pair_t& pair) {
    auto r_number = find_right_number(sum, pair.second);
    auto r_pair_number = find_left_number(sum, pair.second);
    if (r_number.first != sum.end()) {
        int add1 = stoi(std::string(r_number.first, r_number.second + 1));
        int add2 =
            stoi(std::string(r_pair_number.first, r_pair_number.second + 1));
        sum.replace(r_number.first, r_number.second + 1,
                    std::to_string(add1 + add2));
    }
    auto l_number = find_left_number(sum, pair.first);
    auto l_pair_number = find_right_number(sum, pair.first);
    int add1, add2;
    if (l_number.first != sum.begin()) {
        add1 = stoi(std::string(l_number.first, l_number.second + 1));
        add2 = stoi(std::string(l_pair_number.first, l_pair_number.second + 1));
    }
    sum.replace(pair.first, pair.second + 1, "0");
    if (l_number.first != sum.begin()) {
        sum.replace(l_number.first, l_number.second + 1,
                    std::to_string(add1 + add2));
    }
}

auto split_number(std::string& sum, number_t& number_pair) {
    std::string split;
    int number = stoi(std::string(number_pair.first, number_pair.second + 1));
    if (!(number % 2))
        split = '[' + std::to_string(number / 2) + ',' +
                std::to_string(number / 2) + ']';
    else
        split = '[' + std::to_string(number / 2) + ',' +
                std::to_string(number / 2 + 1) + ']';
    sum.replace(number_pair.first, number_pair.second + 1, split);
}

auto add_snail_numbers(const std::string& a, const std::string& b) {
    std::string sum = "[" + a + "," + b + "]";
    sum.reserve(100);
    bool reduced = false;
    while (!reduced) {
        auto too_deep = find_too_deep(sum);
        auto too_large = find_too_large(sum);
        if (too_deep.first != sum.end()) {
            explode_pair(sum, too_deep);
        } else if (too_large.first != sum.end()) {
            split_number(sum, too_large);
        } else
            reduced = true;
    }
    return sum;
}

int calculate_magnitude(std::string& snail_number) {
    auto it = snail_number.begin();
    while (!std::isdigit(snail_number[0])) {
        for (auto it = snail_number.begin(); it < snail_number.end(); ++it) {
            if (*it == '[') {
                auto start = it;
                auto number1 = find_right_number(snail_number, it);
                if (number1.first - it != 1) continue;
                it = number1.second + 1;
                auto number2 = find_right_number(snail_number, it);
                if (number2.first - it != 1) continue;
                int mag1 = stoi(std::string(number1.first, number1.second + 1));
                int mag2 = stoi(std::string(number2.first, number2.second + 1));
                int magnitude = (3 * mag1) + (2 * mag2);
                auto end = number2.second + 1;
                snail_number.replace(start, end + 1, std::to_string(magnitude));
                break;
            }
        }
    }
    return stoi(snail_number);
}

auto solve_puzzle() {
    auto snail_numbers = parse_input();
    std::vector<int> magnitudes;
    for (int i = 0; i < snail_numbers.size(); i++) {
        for (int j = 0; j < snail_numbers.size(); j++) {
            if (i == j) continue;
            auto sum = add_snail_numbers(snail_numbers[i], snail_numbers[j]);
            magnitudes.push_back(calculate_magnitude(sum));
        }
    }
    return *std::max_element(magnitudes.begin(), magnitudes.end());
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