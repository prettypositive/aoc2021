#include <bits/stdc++.h>

struct Player {
    int score = 0;
    int position;
};

struct Die {
    int value = 1;
    int rolls = 0;

    int roll(const int times) {
        int sum = 0;
        for (int i = 0; i < times; i++) {
            sum += value;
            value += 1;
            if (value == 101) value = 1;
        }
        rolls += times;
        return sum;
    }
};

auto parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    std::pair<Player, Player> players;
    getline(input, line);
    players.first.position = line.back() - '0';
    getline(input, line);
    players.second.position = line.back() - '0';
    input.close();
    return players;
}

auto make_move(Player& player, int distance) {
    distance = distance % 10;
    player.position += distance;
    if (player.position > 10) player.position -= 10;
    player.score += player.position;
}

auto solve_puzzle() {
    auto players = parse_input();
    Die die;
    int turn = 0;
    while ((players.first.score < 1000) && (players.second.score < 1000)) {
        Player* player = (turn == 0) ? &players.first : &players.second;
        make_move(*player, die.roll(3));
        turn = abs(turn - 1);
    }
    auto loser = (players.first.score >= 1000) ? players.second : players.first;
    return (loser.score * die.rolls);
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