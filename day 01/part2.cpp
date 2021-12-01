#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

vector<int> parse_input() {
    ifstream input_file("input.txt");
    int reading;
    vector<int> readings;
    while (input_file >> reading) {
        readings.push_back(reading);
    }
    input_file.close();
    return readings;
}

int main() {
    vector<int> readings = parse_input();
    vector<int> windows;
    for (int i = 2; i < readings.size(); i++) {
        windows.push_back(readings[i] + readings[i - 1] + readings[i - 2]);
    }
    int count = 0;
    for (int i = 1; i < windows.size(); i++) {
        if (windows[i] > windows[i - 1]) count += 1;
    }
    cout << count;
    return 0;
}