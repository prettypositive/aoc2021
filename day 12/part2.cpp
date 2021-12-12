#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using path_t = std::unordered_set<std::string>;
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

bool is_visitable(const path_t& path, const std::string& node,
                  const bool& used) {
    if (node == "start") return false;
    if (std::isupper(node[0])) return true;
    if (!used) return true;
    if (!path.contains(node)) return true;
    return false;
}

int traverse_cave(cave_t& cave, path_t path = {},
                  const std::string& current_node = "start",
                  bool used = false) {
    if (current_node == "end") return 1;
    int paths = 0;
    auto [_, unique] = path.insert(current_node);
    if (!unique && std::islower(current_node[0])) used = true;

    for (const auto& node : cave[current_node]) {
        if (is_visitable(path, node, used))
            paths += traverse_cave(cave, path, node, used);
    }
    return paths;
}

auto solve_puzzle() {
    auto cave = parse_input();
    int paths = traverse_cave(cave);
    return paths;
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