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
    int count = 0;
    for (int i = 3; i < readings.size(); i++) {
        int window1 = readings[i-1] + readings[i-2] + readings[i-3];
        int window2 = readings[i] + readings[i-1] + readings[i-2];
        if (window2 > window1) {
            count += 1;
        }
    }
    cout << count;
    return 0;
}