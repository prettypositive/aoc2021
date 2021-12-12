#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using path_t = std::unordered_map<std::string, int>;
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

bool is_visitable(path_t& path, const std::string& node,
                  const bool& used) {
    if (node == "start") return false;
    if (!used) return true;
    if (std::isupper(node[0])) return true;
    if (path[node] > 0) return false;
    return true;
}

int traverse_cave(const cave_t& cave, path_t& path,
                  const std::string& current_node = "start",
                  bool used = false) {
    if (current_node == "end") return 1;
    path[current_node] += 1;
    if (std::islower(current_node[0]) && (path[current_node] == 2)) used = true;
    int paths = 0;
    for (const auto& node : cave.at(current_node)) {
        if (is_visitable(path, node, used))
            paths += traverse_cave(cave, path, node, used);
    }
    path[current_node] -= 1;
    return paths;
}

auto solve_puzzle() {
    auto cave = parse_input();
    path_t path;
    int paths = traverse_cave(cave, path);
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