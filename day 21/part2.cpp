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
    std::unordered_map<Universe, int64_t, Universe::hash> universe_map;
    while (!universes.empty()) {
        for (auto universe = universes.begin(); universe != universes.end();
             ++universe) {
            universe_map.erase(*universe);
            if (!universe->rolls_remaining) {
                Player* player = (universe->turn == 0)
                                     ? &universe->players.first
                                     : &universe->players.second;
                make_move(*player, universe->roll_sum);
                if (player->score >= target) {
                    complete_universes.push_back(*universe);
                    continue;
                }
                universe->roll_sum = 0;
                universe->rolls_remaining = 3;
                universe->turn = abs(universe->turn - 1);
                universe_map[*universe] += universe->count;
            } else {
                universe->rolls_remaining -= 1;
                for (int i = 0; i < 3; i++) {
                    universe->roll_sum += 1;
                    universe_map[*universe] += universe->count;
                }
            }
        }
        universes.clear();
        for (auto& [universe, count] : universe_map) {
            Universe new_universe = universe;
            new_universe.count = count;
            universes.push_back(new_universe);
        }
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