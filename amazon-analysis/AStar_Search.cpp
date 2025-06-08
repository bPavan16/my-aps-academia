#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <cmath>
using namespace std;

// Directions: up, down, left, right
const vector<pair<int, int>> directions = {{-1,0},{1,0},{0,-1},{0,1}};

struct Node {
    int x, y;
    int g, h, f;
    Node* parent;
    Node(int x, int y, int g, int h, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
};

struct CompareNode {
    bool operator()(const Node* a, const Node* b) const {
        return a->f > b->f;
    }
};

int heuristic(int x1, int y1, int x2, int y2) {
    // Manhattan distance
    return abs(x1 - x2) + abs(y1 - y2);
}

bool isValid(int x, int y, int rows, int cols, const vector<vector<int>>& grid) {
    return x >= 0 && x < rows && y >= 0 && y < cols && grid[x][y] == 0;
}

vector<pair<int, int>> reconstructPath(Node* endNode) {
    vector<pair<int, int>> path;
    Node* curr = endNode;
    while (curr) {
        path.push_back({curr->x, curr->y});
        curr = curr->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<pair<int, int>> aStarSearch(
    const vector<vector<int>>& grid,
    pair<int, int> start,
    pair<int, int> goal
) {
    int rows = grid.size(), cols = grid[0].size();
    vector<vector<bool>> closed(rows, vector<bool>(cols, false));
    priority_queue<Node*, vector<Node*>, CompareNode> open;

    Node* startNode = new Node(start.first, start.second, 0,
                               heuristic(start.first, start.second, goal.first, goal.second));
    open.push(startNode);

    while (!open.empty()) {
        Node* curr = open.top();
        open.pop();

        if (curr->x == goal.first && curr->y == goal.second) {
            vector<pair<int, int>> path = reconstructPath(curr);
            // Clean up allocated nodes
            while (!open.empty()) { delete open.top(); open.pop(); }
            return path;
        }

        closed[curr->x][curr->y] = true;

        for (const auto& dir : directions) {
            int nx = curr->x + dir.first, ny = curr->y + dir.second;
            if (isValid(nx, ny, rows, cols, grid) && !closed[nx][ny]) {
                int gNew = curr->g + 1;
                int hNew = heuristic(nx, ny, goal.first, goal.second);
                Node* neighbor = new Node(nx, ny, gNew, hNew, curr);
                open.push(neighbor);
            }
        }
    }
    return {}; // No path found
}

int main() {
    // 0 = free cell, 1 = obstacle
    vector<vector<int>> grid = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0},
        {0, 0, 0, 1, 0}
    };

    pair<int, int> start = {0, 0};
    pair<int, int> goal = {4, 4};

    vector<pair<int, int>> path = aStarSearch(grid, start, goal);

    if (!path.empty()) {
        cout << "Path found:\n";
        for (auto& p : path)
            cout << "(" << p.first << "," << p.second << ") ";
        cout << endl;
    } else {
        cout << "No path found." << endl;
    }

    return 0;
}
