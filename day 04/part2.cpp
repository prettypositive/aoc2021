#include <algorithm>
#include <array>
#include <chrono>
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
    string raw_numbers;
    getline(input, raw_numbers);

    vector<int> numbers;
    stringstream ss(raw_numbers);
    string number;
    while (getline(ss, number, ',')) {
        numbers.push_back(stoi(number));
    }
    reverse(numbers.begin(), numbers.end());

    vector<board_t> boards;
    array<array<int, 5>, 5> board = {};
    while (true) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                input >> board[i][j];
            }
        }
        if (input.eof()) break;
        boards.push_back(board);
    }
    return {numbers, boards};
}

bool check_row(const vector<int>& called_numbers, const board_t& board,
               const int row) {
    int marked = 0;
    for (int i = 0; i < 5; i++) {
        if (find(called_numbers.begin(), called_numbers.end(), board[row][i]) !=
            called_numbers.end())
            marked += 1;
    }
    return (marked == 5) ? true : false;
}

bool check_column(const vector<int>& called_numbers, const board_t& board,
                  const int column) {
    int marked = 0;
    for (int i = 0; i < 5; i++) {
        if (find(called_numbers.begin(), called_numbers.end(),
                 board[i][column]) != called_numbers.end())
            marked += 1;
    }
    return (marked == 5) ? true : false;
}

auto check_boards(const vector<int>& called_numbers, vector<board_t>& boards) {
    vector<board_t> winners;
    for (auto it = boards.begin(); it != boards.end(); it++) {
        for (int i = 0; i < 5; i++) {
            if (check_row(called_numbers, *it, i) ||
                check_column(called_numbers, *it, i)) {
                winners.push_back(*it);
                boards.erase(it--);
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
                     board[i][j]) == called_numbers.end())
                answer += board[i][j];
        }
    }
    answer *= called_numbers.back();
    return answer;
}

int main() {
    auto start = chrono::steady_clock::now();

    auto [numbers, boards] = parse_input();
    vector<int> called_numbers;
    vector<board_t> winners;
    int all_boards = boards.size();
    while (winners.size() != all_boards) {
        called_numbers.push_back(numbers.back());
        numbers.pop_back();
        auto new_winners = check_boards(called_numbers, boards);
        winners.insert(winners.end(), new_winners.begin(), new_winners.end());
    }
    cout << compute_answer(winners.back(), called_numbers);

    auto end = chrono::steady_clock::now();
    cout << " [" << chrono::duration<double, milli>(end - start).count()
         << " ms]" << endl;
}