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
    for (int i = 1; i < readings.size(); i++) {
        if (readings[i] > readings[i - 1]) {
            count += 1;
        }
    }
    cout << count;
    return 0;
}