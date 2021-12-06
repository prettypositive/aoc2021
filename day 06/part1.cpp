#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
using namespace std;

auto parse_input() {
    ifstream input("input.txt");
    string fish;
    array<int64_t, 9> population = {};
    while (getline(input, fish, ',')) {
        population[stoi(fish)] += 1;
    }
    return population;
}

auto timestep(const array<int64_t, 9>& population) {
    array<int64_t, 9> new_population = {};
    for (int i = 0; i < 9; i++) {
        int x = i - 1;
        if (x == -1) x = 8;
        new_population[x] = population[i];
    }
    new_population[6] += population[0];
    return new_population;
}

int main() {
    auto start = chrono::steady_clock::now();

    auto population = parse_input();
    for (int i = 0; i < 80; i++) {
        auto new_population = timestep(population);
        population = new_population;
    }

    int64_t sum = 0;
    for (const auto& group : population) {
        sum += group;
    }
    cout << sum;

    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
    return 0;
}