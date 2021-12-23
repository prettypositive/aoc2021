#include <bits/stdc++.h>

struct GameState {
    // A between 1-2, B between 2-3, C between 3-4, D between 4-5
    std::array<char, 7> hallway = {};
    std::unordered_map<char, std::vector<char>> homes;
};

struct Move {
    std::pair<char, int> source;
    std::pair<char, int> destination;
    char piece;
    int cost;
};

bool is_solved(const GameState& game) {
    if (game.homes.at('A') == std::vector{'A', 'A', 'A', 'A'} &&
        game.homes.at('B') == std::vector{'B', 'B', 'B', 'B'} &&
        game.homes.at('C') == std::vector{'C', 'C', 'C', 'C'} &&
        game.homes.at('D') == std::vector{'D', 'D', 'D', 'D'})
        return true;
    else
        return false;
}

bool is_path_clear(const GameState& game, std::pair<char, int> source,
                   std::pair<char, int> destination) {
    std::unordered_map<char, std::unordered_map<int, std::vector<int>>>
        blockers;
    blockers['A'] = {{0, {1}},    {1, {}},        {2, {}},          {3, {2}},
                     {4, {2, 3}}, {5, {2, 3, 4}}, {6, {2, 3, 4, 5}}};
    blockers['B'] = {{0, {1, 2}}, {1, {2}},    {2, {}},       {3, {}},
                     {4, {3}},    {5, {3, 4}}, {6, {3, 4, 5}}};
    blockers['C'] = {{0, {1, 2, 3}}, {1, {2, 3}}, {2, {3}},   {3, {}},
                     {4, {}},        {5, {4}},    {6, {4, 5}}};
    blockers['D'] = {{0, {1, 2, 3, 4}}, {1, {2, 3, 4}}, {2, {3, 4}}, {3, {4}},
                     {4, {}},           {5, {}},        {6, {5}}};
    bool clear = true;
    if (source.first == 'H') {
        for (const auto& blocker : blockers[destination.first][source.second]) {
            if (game.hallway[blocker]) clear = false;
        }
    } else {
        for (const auto& blocker : blockers[source.first][destination.second]) {
            if (game.hallway[blocker]) clear = false;
        }
    }
    return (clear) ? true : false;
}

void find_hallway_moves(const GameState& game, std::vector<Move>& legal_moves) {
    // pieces in a hallway can only move to their designated home
    // and only if the home is homogenous with their type
    std::unordered_map<char, std::unordered_map<int, int>> move_costs;
    move_costs['A'] = {{0, 6}, {1, 5},  {2, 5}, {3, 7},
                       {4, 9}, {5, 11}, {6, 12}};
    move_costs['B'] = {{0, 8}, {1, 7}, {2, 5}, {3, 5}, {4, 7}, {5, 9}, {6, 10}};
    move_costs['C'] = {{0, 10}, {1, 9}, {2, 7}, {3, 5}, {4, 5}, {5, 7}, {6, 8}};
    move_costs['D'] = {{0, 12}, {1, 11}, {2, 9}, {3, 7},
                       {4, 5},  {5, 5},  {6, 6}};
    std::unordered_map<char, int> piece_costs = {
        {'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};

    for (int i = 0; i < game.hallway.size(); i++) {
        if (!game.hallway[i]) continue;
        Move move;
        move.source = {'H', i};
        move.piece = game.hallway[i];
        move.destination = {move.piece, 0};
        move.cost = (move_costs[move.destination.first][i] -
                     game.homes.at(move.piece).size()) *
                    piece_costs[move.piece];

        bool valid_home = true;
        for (const auto& piece : game.homes.at(move.piece)) {
            if (piece != move.piece) valid_home = false;
        }
        if (!valid_home) continue;

        if (is_path_clear(game, move.source, move.destination))
            legal_moves.push_back(move);
    }
}

void find_home_moves(const GameState& game, char home,
                     std::vector<Move>& legal_moves) {
    // pieces in a home can only move to a hallway space (not really, but it
    // simplifies things)
    std::unordered_map<char, std::unordered_map<int, int>> move_costs;
    move_costs['A'] = {{0, 6}, {1, 5},  {2, 5}, {3, 7},
                       {4, 9}, {5, 11}, {6, 12}};
    move_costs['B'] = {{0, 8}, {1, 7}, {2, 5}, {3, 5}, {4, 7}, {5, 9}, {6, 10}};
    move_costs['C'] = {{0, 10}, {1, 9}, {2, 7}, {3, 5}, {4, 5}, {5, 7}, {6, 8}};
    move_costs['D'] = {{0, 12}, {1, 11}, {2, 9}, {3, 7},
                       {4, 5},  {5, 5},  {6, 6}};
    std::unordered_map<char, int> piece_costs = {
        {'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};

    if (game.homes.at(home).empty()) return;
    bool homogenous = true;
    for (const auto& piece : game.homes.at(home)) {
        if (piece != home) homogenous = false;
    }
    if (homogenous) return;

    for (int i = 0; i < game.hallway.size(); i++) {
        if (game.hallway[i]) continue;
        Move move;
        move.source = {home, 0};
        move.piece = game.homes.at(home).back();
        move.destination = {'H', i};
        move.cost = (move_costs[home][i] - game.homes.at(home).size() + 1) *
                    piece_costs[move.piece];

        if (is_path_clear(game, move.source, move.destination))
            legal_moves.push_back(move);
    }
}

GameState make_move(GameState game, const Move move) {
    if (move.source.first == 'H') {
        game.hallway[move.source.second] = false;
    } else {
        game.homes[move.source.first].pop_back();
    }
    if (move.destination.first == 'H') {
        game.hallway[move.destination.second] = move.piece;
    } else {
        game.homes[move.destination.first].push_back(move.piece);
    }
    return game;
}

void dfs_solve(const GameState& game, int& best_score, const int& score = 0) {
    if (score > best_score) return;
    if (is_solved(game)) {
        if (score < best_score) std::cout << score << std::endl;
        best_score = score;
        return;
    }
    std::vector<Move> legal_moves;
    find_hallway_moves(game, legal_moves);
    for (const auto& [home, _] : game.homes) {
        find_home_moves(game, home, legal_moves);
    }
    if (legal_moves.empty()) return;
    for (const auto& move : legal_moves) {
        dfs_solve(make_move(game, move), best_score, score + move.cost);
    }
    return;
}

auto solve_puzzle() {
    GameState game;
    game.homes['A'] = {'C', 'D', 'D', 'B'};
    game.homes['B'] = {'C', 'B', 'C', 'B'};
    game.homes['C'] = {'A', 'A', 'B', 'D'};
    game.homes['D'] = {'A', 'C', 'A', 'D'};
    int best_score = INT_MAX;
    dfs_solve(game, best_score);
    return best_score;
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