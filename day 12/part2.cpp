#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using path_t = std::vector<std::string>;
using cave_t = std::unordered_map<std::string, std::vector<std::string>>;

auto parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    cave_t cave;
    while (input >> line) {
        std::stringstream ss(line);
        std::string node1, node2;
        getline(ss, node1, '-');
        ss >> node2;
        cave[node1].push_back(node2);
        cave[node2].push_back(node1);
    }
    input.close();
    return cave;
}

bool is_visitable(const path_t& path, const std::string& node) {
    if (node == "start") return false;
    if (node == "end") return true;
    if (std::isupper(node[0])) return true;
    // only lowercase nodes remain
    std::unordered_map<std::string, int> counter;
    for (const auto& path_node : path) {
        if (std::islower(path_node[0])) counter[path_node] += 1;
    }
    bool used = false;
    for (const auto& [_, value] : counter) {
        if (value > 1) used = true;
    }
    if (!used) return true;  // if no small caves visited twice
    if (!counter.contains(node))
        return true;  // otherwise, if current node not visited yet
    return false;     // otherwise, double visit already used up
}

void traverse_cave(cave_t& cave, std::vector<path_t>& paths, path_t path = {},
                   const std::string& current_node = "start") {
    path.push_back(current_node);
    if (current_node == "end") {
        paths.push_back(path);
        return;
    }
    for (const auto& node : cave[current_node]) {
        if (is_visitable(path, node)) traverse_cave(cave, paths, path, node);
    }
    return;
}

auto solve_puzzle() {
    auto cave = parse_input();
    std::vector<path_t> paths;
    traverse_cave(cave, paths);
    return paths.size();
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