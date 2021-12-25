#include <bits/stdc++.h>

auto parse_input() {
    std::ifstream input("input.txt");
    std::istream_iterator<std::string> start(input), end;
    std::vector<std::string> grid(start, end);
    input.close();
    return grid;
}

int timestep(std::vector<std::string>& grid) {
    std::vector<std::string> new_grid = grid;
    int moves = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {
            if (grid[y][x] == '>') {
                int next_x = (x + 1) % grid[0].size();
                if (grid[y][next_x] == '.') {
                    moves += 1;
                    new_grid[y][next_x] = '>';
                    new_grid[y][x] = '.';
                }
            }
        }
    }
    grid = new_grid;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {
            if (grid[y][x] == 'v') {
                int next_y = (y + 1) % grid.size();
                if (grid[next_y][x] == '.') {
                    moves += 1;
                    new_grid[next_y][x] = 'v';
                    new_grid[y][x] = '.';
                }
            }
        }
    }
    grid = new_grid;
    return moves;
}

auto solve_puzzle() {
    auto grid = parse_input();
    int steps = 1;
    while (timestep(grid)) {
        steps += 1;
    }
    return steps;
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