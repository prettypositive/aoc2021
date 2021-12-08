#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

using signal_t = pair<vector<string>, vector<string>>;

auto parse_input() {
    ifstream input("input.txt");
    string digit, delimiter;
    signal_t signal;
    vector<signal_t> signals;
    while (true) {
        vector<string> unique_digits, output_digits;
        for (int i = 0; i < 10; i++) {
            input >> digit;
            unique_digits.push_back(digit);
        }
        if (input.eof()) break;
        input >> delimiter;
        for (int i = 0; i < 4; i++) {
            input >> digit;
            output_digits.push_back(digit);
        }
        signal = {unique_digits, output_digits};
        signals.push_back(signal);
    }
    return signals;
}

auto solve_segments(const vector<string>& digits) {
    //  aaaa
    // b    c
    // b    c
    //  dddd
    // e    f
    // e    f
    //  gggg

    // e appears in exactly 4 digits
    // b appears in exactly 6 digits
    // f appears in exactly 9 digits
    // d, g appear in exactly 7 digits
    // solve d by checking what is in 4-length and not in 2-length, and not "b"
    // a, c appear in exactly 8 digits
    // solve a by checking what is in 3-length and not in 2-length

    unordered_map<char, int> counter;
    string len_2, len_3, len_4;
    for (const auto& digit : digits) {
        if (digit.size() == 2) len_2 = digit;
        if (digit.size() == 3) len_3 = digit;
        if (digit.size() == 4) len_4 = digit;
        for (const auto& segment : digit) {
            counter[segment] += 1;
        }
    }
    unordered_map<char, char> segments;
    for (const auto& [key, value] : counter) {
        if (value == 4) segments['e'] = key;
        if (value == 6) segments['b'] = key;
        if (value == 9) segments['f'] = key;
    }
    for (const auto& [key, value] : counter) {
        if (value == 7) {
            if ((len_2.find(key) == string::npos) &&
                (len_4.find(key) != string::npos) && (key != segments['b']))
                segments['d'] = key;
            else
                segments['g'] = key;
        }
        if (value == 8) {
            if ((len_2.find(key) == string::npos) &&
                (len_3.find(key) != string::npos))
                segments['a'] = key;
            else
                segments['c'] = key;
        }
    }
    return segments;
}

auto solve_digits(unordered_map<char, char>& seg) {
    vector<string> digits;
    digits.resize(10);
    digits[0] = {seg['a'], seg['b'], seg['c'], seg['e'], seg['f'], seg['g']};
    digits[1] = {seg['c'], seg['f']};
    digits[2] = {seg['a'], seg['c'], seg['d'], seg['e'], seg['g']};
    digits[3] = {seg['a'], seg['c'], seg['d'], seg['f'], seg['g']};
    digits[4] = {seg['b'], seg['c'], seg['d'], seg['f']};
    digits[5] = {seg['a'], seg['b'], seg['d'], seg['f'], seg['g']};
    digits[6] = {seg['a'], seg['b'], seg['d'], seg['e'], seg['f'], seg['g']};
    digits[7] = {seg['a'], seg['c'], seg['f']};
    digits[8] = {seg['a'], seg['b'], seg['c'], seg['d'],
                 seg['e'], seg['f'], seg['g']};
    digits[9] = {seg['a'], seg['b'], seg['c'], seg['d'], seg['f'], seg['g']};
    for (auto& digit : digits) {
        sort(digit.begin(), digit.end());
    }
    return digits;
}

int read_output(const vector<string>& digits, vector<string> output_digits) {
    string output;
    for (auto& digit : output_digits) {
        sort(digit.begin(), digit.end());
        for (int i = 0; i < size(digits); i++) {
            if (digit == digits[i]) output.push_back('0' + i);
        }
    }
    return stoi(output);
}

int solution() {
    auto signals = parse_input();
    int sum;
    for (const auto& signal : signals) {
        auto segments = solve_segments(signal.first);
        auto digits = solve_digits(segments);
        int output = read_output(digits, signal.second);
        sum += output;
    }
    return sum;
}

int main() {
    auto start = chrono::steady_clock::now();
    cout << solution();
    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}