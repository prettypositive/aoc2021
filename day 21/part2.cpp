#include <bits/stdc++.h>

struct Player {
    int score = 0;
    int position;

    bool operator==(const Player& other) const {
        return (score == other.score && position == other.position);
    }
};

struct Universe {
    std::pair<Player, Player> players;
    int turn = 0;
    int roll_sum = 0;
    int rolls_remaining = 3;
    int64_t count = 1;

    bool operator==(const Universe& other) const {
        return (turn == other.turn &&
                rolls_remaining == other.rolls_remaining &&
                roll_sum == other.roll_sum &&
                players.first == other.players.first &&
                players.second == other.players.second);
    }

    struct hash {
        size_t operator()(const Universe& universe) const {
            int hash = 17;
            hash = hash * 31 + universe.turn;
            hash = hash * 31 + universe.rolls_remaining;
            hash = hash * 31 + universe.players.first.score;
            hash = hash * 31 + universe.players.second.score;
            hash = hash * 31 + universe.players.first.position;
            hash = hash * 31 + universe.players.second.position;
            return hash;
        }
    };
};

auto parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    Universe universe;
    std::vector<Universe> universes;
    getline(input, line);
    universe.players.first.position = line.back() - '0';
    getline(input, line);
    universe.players.second.position = line.back() - '0';
    input.close();
    universes.push_back(universe);
    return universes;
}

auto make_move(Player& player, int distance) {
    player.position += distance;
    if (player.position > 10) player.position -= 10;
    player.score += player.position;
}

auto solve_puzzle() {
    auto universes = parse_input();
    int target = 21;
    std::vector<Universe> complete_universes;
    while (universes.size()) {
        auto new_universes = universes;
        for (auto& universe : universes) {
            if (!universe.rolls_remaining) {
                Universe new_universe = universe;
                Player* player = (new_universe.turn == 0)
                                     ? &new_universe.players.first
                                     : &new_universe.players.second;
                make_move(*player, new_universe.roll_sum);
                if (player->score >= target) {
                    complete_universes.push_back(new_universe);
                    new_universes.erase(std::find(
                        new_universes.begin(), new_universes.end(), universe));
                    continue;
                }
                new_universe.roll_sum = 0;
                new_universe.rolls_remaining = 3;
                new_universe.turn = abs(new_universe.turn - 1);
                auto it = std::find(new_universes.begin(), new_universes.end(),
                                    new_universe);
                if (it != new_universes.end())
                    it->count += new_universe.count;
                else
                    new_universes.push_back(new_universe);
            } else {
                for (int i = 1; i < 4; i++) {
                    Universe new_universe = universe;
                    new_universe.rolls_remaining -= 1;
                    new_universe.roll_sum += i;
                    auto it = std::find(new_universes.begin(),
                                        new_universes.end(), new_universe);
                    if (it != new_universes.end())
                        it->count += new_universe.count;
                    else
                        new_universes.push_back(new_universe);
                }
            }
            new_universes.erase(std::find(new_universes.begin(),
                                          new_universes.end(), universe));
        }
        universes = new_universes;
    }
    std::vector<int64_t> counter = {0, 0};
    for (const auto& universe : complete_universes) {
        counter[universe.turn] += universe.count;
    }
    return *std::max_element(counter.begin(), counter.end());
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