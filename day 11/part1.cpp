#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Octopus {
    int x;
    int y;

    Octopus(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const Octopus& other) const {
        return (x == other.x && y == other.y);
    }

    struct hash {
        size_t operator()(const Octopus& octopus) const {
            int hash = 17;
            hash = hash * 31 + octopus.x;
            hash = hash * 31 + octopus.y;
            return hash;
        }
    };
};

using octopuses_t = std::unordered_map<Octopus, int, Octopus::hash>;

auto parse_input() {
    std::ifstream input("input.txt");
    std::istream_iterator<std::string> start(input), end;
    std::vector<std::string> raw(start, end);
    octopuses_t octopuses;
    for (int y = 0; y < raw.size(); y++) {
        for (int x = 0; x < raw[0].size(); x++) {
            Octopus octopus(x, y);
            octopuses[octopus] = (raw[y][x] - '0');
        }
    }
    return octopuses;
}

auto find_neighbors(const Octopus& octopus, const octopuses_t& octopuses) {
    // returns a vector of existing neighbors for a given octopus
    std::vector<Octopus> possible = {
        Octopus(octopus.x, octopus.y + 1),
        Octopus(octopus.x, octopus.y - 1),
        Octopus(octopus.x + 1, octopus.y),
        Octopus(octopus.x - 1, octopus.y),
        Octopus(octopus.x - 1, octopus.y - 1),
        Octopus(octopus.x + 1, octopus.y + 1),
        Octopus(octopus.x - 1, octopus.y + 1),
        Octopus(octopus.x + 1, octopus.y - 1),
    };
    std::vector<Octopus> neighbors;
    for (const auto& octopus : possible) {
        if (octopuses.contains(octopus)) neighbors.push_back(octopus);
    }
    return neighbors;
}

auto timestep(octopuses_t& octopuses) {
    for (auto& [octopus, energy] : octopuses) {
        energy += 1;
    }
    int flashes = 0;
    bool new_flashes = true;
    std::unordered_set<Octopus, Octopus::hash> flashed;
    while (new_flashes) {
        new_flashes = false;
        for (const auto& [octopus, energy] : octopuses) {
            if ((!flashed.contains(octopus)) && (energy > 9)) {
                flashed.insert(octopus);
                flashes += 1;
                new_flashes = true;
                auto neighbors = find_neighbors(octopus, octopuses);
                for (const auto& neighbor : neighbors) {
                    octopuses[neighbor] += 1;
                }
            }
        }
    }
    for (auto& [octopus, energy] : octopuses) {
        if (energy > 9) energy = 0;
    }
    return flashes;
}

auto solve_puzzle() {
    auto octopuses = parse_input();
    int solution = 0;
    for (int i = 0; i < 100; i++) {
        int flashes = timestep(octopuses);
        solution += flashes;
    }
    return solution;
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