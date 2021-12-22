#include <bits/stdc++.h>

struct Cube {
    std::pair<int64_t, int64_t> x;
    std::pair<int64_t, int64_t> y;
    std::pair<int64_t, int64_t> z;
    bool on;
};

struct Checkpoint {
    int value;
    bool start;
    int id;

    Checkpoint() {}

    Checkpoint(int value, bool start, int id) {
        this->value = value;
        this->start = start;
        this->id = id;
    }
};

auto parse_input() {
    std::ifstream input("input.txt");
    std::string line;
    std::regex regex(
        "^(on|off) x=(-?\\d+)\\.\\.(-?\\d+),"
        "y=(-?\\d+)\\.\\.(-?\\d+),"
        "z=(-?\\d+)\\.\\.(-?\\d+)$");
    std::vector<Cube> steps;
    while (getline(input, line)) {
        std::smatch matches;
        std::regex_match(line, matches, regex);
        Cube cube;
        cube.on = (matches[1] == "on") ? true : false;
        cube.x.first = stoll(matches[2]);
        cube.x.second = stoll(matches[3]);
        cube.y.first = stoll(matches[4]);
        cube.y.second = stoll(matches[5]);
        cube.z.first = stoll(matches[6]);
        cube.z.second = stoll(matches[7]);
        steps.push_back(cube);
    }
    return steps;
}

auto solve_puzzle() {
    // https://stackoverflow.com/questions/66135217/how-to-subdivide-set-of-overlapping-aabb-into-non-overlapping-set-of-aabbs
    auto steps = parse_input();
    std::vector<Checkpoint> x_checkpoints;
    for (int i = 0; i < steps.size(); i++) {
        x_checkpoints.push_back(Checkpoint(steps[i].x.first, true, i));
        x_checkpoints.push_back(Checkpoint(steps[i].x.second, false, i));
    }
    std::sort(x_checkpoints.begin(), x_checkpoints.end(),
              [](const auto& a, const auto& b) { return a.value < b.value; });
    std::vector<std::pair<std::set<int64_t>, int64_t>> x_intervals;
    std::pair<std::set<int64_t>, int64_t> interval;
    Checkpoint prev_checkpoint;
    for (const auto& checkpoint : x_checkpoints) {
        interval.second = checkpoint.value - prev_checkpoint.value;
        if (checkpoint.start && !prev_checkpoint.start) interval.second -= 1;
        if (!checkpoint.start && prev_checkpoint.start) interval.second += 1;
        if (!interval.first.empty() && interval.second != 0)
            x_intervals.push_back(interval);
        if (checkpoint.start)
            interval.first.insert(checkpoint.id);
        else
            interval.first.erase(checkpoint.id);

        prev_checkpoint = checkpoint;
    }

    std::vector<Checkpoint> y_checkpoints;
    for (int i = 0; i < steps.size(); i++) {
        y_checkpoints.push_back(Checkpoint(steps[i].y.first, true, i));
        y_checkpoints.push_back(Checkpoint(steps[i].y.second, false, i));
    }
    std::sort(y_checkpoints.begin(), y_checkpoints.end(),
              [](const auto& a, const auto& b) { return a.value < b.value; });
    std::vector<std::pair<std::set<int64_t>, int64_t>> y_intervals;
    for (const auto& checkpoint : y_checkpoints) {
        interval.second = checkpoint.value - prev_checkpoint.value;
        if (checkpoint.start && !prev_checkpoint.start) interval.second -= 1;
        if (!checkpoint.start && prev_checkpoint.start) interval.second += 1;
        if (!interval.first.empty() && interval.second != 0)
            y_intervals.push_back(interval);
        if (checkpoint.start)
            interval.first.insert(checkpoint.id);
        else
            interval.first.erase(checkpoint.id);

        prev_checkpoint = checkpoint;
    }

    std::vector<Checkpoint> z_checkpoints;
    for (int i = 0; i < steps.size(); i++) {
        z_checkpoints.push_back(Checkpoint(steps[i].z.first, true, i));
        z_checkpoints.push_back(Checkpoint(steps[i].z.second, false, i));
    }
    std::sort(z_checkpoints.begin(), z_checkpoints.end(),
              [](const auto& a, const auto& b) { return a.value < b.value; });
    std::vector<std::pair<std::set<int64_t>, int64_t>> z_intervals;
    for (const auto& checkpoint : z_checkpoints) {
        interval.second = checkpoint.value - prev_checkpoint.value;
        if (checkpoint.start && !prev_checkpoint.start) interval.second -= 1;
        if (!checkpoint.start && prev_checkpoint.start) interval.second += 1;
        if (!interval.first.empty() && interval.second != 0)
            z_intervals.push_back(interval);
        if (checkpoint.start)
            interval.first.insert(checkpoint.id);
        else
            interval.first.erase(checkpoint.id);

        prev_checkpoint = checkpoint;
    }

    int64_t total_area = 0;
    for (int i = 0; i < x_intervals.size(); i++) {
        std::cout << i << std::endl;
        for (int j = 0; j < y_intervals.size(); j++) {
            for (int k = 0; k < z_intervals.size(); k++) {
                std::vector<int64_t> res1, res2;
                std::set_intersection(
                    x_intervals[i].first.begin(), x_intervals[i].first.end(),
                    y_intervals[j].first.begin(), y_intervals[j].first.end(),
                    std::back_inserter(res1));
                if (res1.empty()) continue;
                std::set_intersection(z_intervals[k].first.begin(),
                                      z_intervals[k].first.end(), res1.begin(),
                                      res1.end(), std::back_inserter(res2));
                if (res2.empty()) continue;
                int64_t area = x_intervals[i].second * y_intervals[j].second *
                               z_intervals[k].second;
                if (steps[res2.back()].on) total_area += area;
            }
        }
    }
    return total_area;
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