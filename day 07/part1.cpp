#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

auto parse_input() {
    ifstream input("input.txt");
    string position;
    vector<int> positions;
    while (getline(input, position, ',')) {
        positions.push_back(stoi(position));
    }
    return positions;
}

int main() {
    auto start = chrono::steady_clock::now();

    auto positions = parse_input();
    auto highest = *max_element(positions.begin(), positions.end());
    vector<int> fuel;
    fuel.resize(highest);
    for (int i = 0; i < fuel.size(); i++) {
        for (const auto& position : positions) {
            fuel[i] += abs(position - i);
        }
    }
    cout << *min_element(fuel.begin(), fuel.end());

    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}