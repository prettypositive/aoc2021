#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using board_t = array<array<int, 5>, 5>;

tuple<vector<int>, vector<board_t>> parse_input() {
    ifstream input("input.txt");
    string input_numbers;
    getline(input, input_numbers);

    vector<int> numbers;
    stringstream ss(input_numbers);
    string number;
    while (getline(ss, number, ',')) {
        numbers.push_back(stoi(number));
    }
    reverse(numbers.begin(), numbers.end());

    int r1, r2, r3, r4, r5;
    int i = 0;
    vector<board_t> boards;
    array<array<int, 5>, 5> board = {};
    while (input >> r1 >> r2 >> r3 >> r4 >> r5) {
        if (i == 5) {
            boards.push_back(board);
            board.fill({});
            i = 0;
        }
        board[i][0] = r1;
        board[i][1] = r2;
        board[i][2] = r3;
        board[i][3] = r4;
        board[i][4] = r5;
        i += 1;
    }
    boards.push_back(board);
    return {numbers, boards};
}

bool check_row(const vector<int>& called_numbers, const board_t& board,
               const int index) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        if (find(called_numbers.begin(), called_numbers.end(),
                 board[index][i]) != called_numbers.end())
            sum += 1;
    }
    return (sum == 5) ? true : false;
}

bool check_column(const vector<int>& called_numbers, const board_t& board,
                  const int index) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        if (find(called_numbers.begin(), called_numbers.end(),
                 board[i][index]) != called_numbers.end())
            sum += 1;
    }
    return (sum == 5) ? true : false;
}
auto check_boards(const vector<int>& called_numbers, vector<board_t>& boards) {
    vector<board_t> winners;
    for (int i = 0; i < boards.size(); i++) {
        for (int j = 0; j < 5; j++) {
            if (check_row(called_numbers, boards[i], j) ||
                check_column(called_numbers, boards[i], j)) {
                winners.push_back(boards[i]);
                boards.erase(boards.begin() + i);
                break;
            }
        }
    }
    return winners;
}

int compute_answer(const board_t& board, const vector<int>& called_numbers) {
    int answer = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (find(called_numbers.begin(), called_numbers.end(),
                     board[i][j]) == called_numbers.end()) {
                answer += board[i][j];
            }
        }
    }
    for (const auto& number : called_numbers) {
    }
    answer *= called_numbers.back();
    return answer;
}

int main() {
    auto [numbers, boards] = parse_input();
    int winner = -1;
    vector<int> called_numbers;
    vector<board_t> winners;
    int all_boards = boards.size();
    while (winners.size() != all_boards) {
        called_numbers.push_back(numbers.back());
        numbers.pop_back();
        auto new_winners = check_boards(called_numbers, boards);
        winners.insert(winners.end(), new_winners.begin(), new_winners.end());
    }
    int answer = compute_answer(winners.back(), called_numbers);
    cout << answer;
}