#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

auto parse_input() {
    ifstream input("input.txt");
    string _;
    string digit;
    vector<string> digits;
    while (getline(input, _, '|')) {
        if (input.eof()) break;
        for (int i = 0; i < 4; i++) {
            input >> digit;
            digits.push_back(digit);
        }
    }
    return digits;
}

int solution() {
    auto digits = parse_input();
    erase_if(digits, [](string x) {
        return ((x.size() != 2) && (x.size() != 3) && (x.size() != 4) &&
                (x.size() != 7));
    });
    return digits.size();
}

int main() {
    auto start = chrono::steady_clock::now();
    cout << solution();
    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}