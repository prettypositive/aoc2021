#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <unordered_map>
#include <vector>

struct Node {
    struct hash {
        size_t operator()(const Node& node) const {
            int hash = 17;
            hash = hash * 31 + node.x;
            hash = hash * 31 + node.y;
            return hash;
        }
    };

    int x;
    int y;
    int value;

    Node() {}

    Node(int x, int y, int value) {
        this->x = x;
        this->y = y;
        this->value = value;
    }

    Node(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const Node& other) const {
        return (x == other.x && y == other.y);
    }
};

struct PriorityNode {
    Node node;
    int priority;

    PriorityNode(Node node, int priority) {
        this->node = node;
        this->priority = priority;
    }

    bool operator>(const PriorityNode& other) const {
        return (priority > other.priority);
    }
};

using graph_t = std::unordered_map<Node, std::vector<Node>, Node::hash>;
auto parse_input() {
    std::ifstream input("input.txt");
    std::istream_iterator<std::string> start(input), end;
    std::vector<std::string> grid(start, end);
    input.close();

    std::vector<Node> nodes;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[0].size(); x++) {
            auto node = Node(x, y, (grid[y][x] - '0'));
            nodes.push_back(node);
        }
    }
    return nodes;
}

auto build_graph(const std::vector<Node>& nodes) {
    graph_t graph;
    for (auto& node : nodes) {
        std::vector<Node> possibilities = {
            Node(node.x, node.y + 1),
            Node(node.x, node.y - 1),
            Node(node.x + 1, node.y),
            Node(node.x - 1, node.y),
        };
        for (const auto& possibility : possibilities) {
            auto found = std::find(nodes.begin(), nodes.end(), possibility);
            if (found != nodes.end()) graph[node].push_back(*found);
        }
    }
    return graph;
}

int distance(const Node& a, const Node& b) {
    return (std::abs(a.x - b.x) + std::abs(a.y - b.y));
}

auto a_star_search(const graph_t& graph, const Node& start, const Node& end) {
    std::priority_queue<PriorityNode, std::vector<PriorityNode>,
                        std::greater<PriorityNode>>
        queue;
    std::unordered_map<Node, Node, Node::hash> came_from;
    std::unordered_map<Node, int, Node::hash> cost_so_far;
    queue.push(PriorityNode(start, 0));

    while (!queue.empty()) {
        Node current = queue.top().node;
        queue.pop();

        if (current == end) break;

        for (const auto& next : graph.at(current)) {
            int new_cost = cost_so_far[current] + next.value;
            if ((!cost_so_far.contains(next)) ||
                (new_cost < cost_so_far[next])) {
                cost_so_far[next] = new_cost;
                int priority = new_cost + distance(end, next);
                queue.push(PriorityNode(next, priority));
                came_from[next] = current;
            }
        }
    }

    Node current = end;
    std::vector<Node> path;
    while (current != start) {
        path.push_back(current);
        current = came_from[current];
    }
    return path;
}

auto solve_puzzle() {
    auto nodes = parse_input();
    auto graph = build_graph(nodes);
    Node start = nodes.front(), end = nodes.back();
    auto path = a_star_search(graph, start, end);
    int solution = std::accumulate(
        path.begin(), path.end(), 0,
        [](const auto& a, const auto& b) { return a + b.value; });
    return solution;
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