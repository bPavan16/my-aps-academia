/ Generic C++ implementation of A* algorithm
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <limits>

class AStarAlgorithm {
private:
    struct Node {
        int id;
        double x, y;  // Coordinates for heuristic calculation
        std::vector<std::pair<int, double>> neighbors;  // {neighbor_id, cost}
        
        Node(int _id, double _x, double _y) : id(_id), x(_x), y(_y) {}
    };
    
    struct AStarNode {
        int id;
        double gScore;  // Cost from start
        double fScore;  // gScore + heuristic
        int parent;
        
        AStarNode(int _id, double _g, double _f, int _parent)
            : id(_id), gScore(_g), fScore(_f), parent(_parent) {}
        
        // For priority queue (min-heap based on fScore)
        bool operator>(const AStarNode& other) const {
            return fScore > other.fScore;
        }
    };
    
    std::unordered_map<int, Node> nodes;
    
    // Calculate Euclidean distance heuristic
    double heuristic(int from, int to) {
        const Node& nodeA = nodes[from];
        const Node& nodeB = nodes[to];
        
        double dx = nodeA.x - nodeB.x;
        double dy = nodeA.y - nodeB.y;
        
        return std::sqrt(dx * dx + dy * dy);
    }

public:
    // Add node to the graph
    void addNode(int id, double x, double y) {
        nodes.emplace(id, Node(id, x, y));
    }
    
    // Add weighted edge between nodes
    void addEdge(int from, int to, double cost) {
        if (nodes.find(from) != nodes.end()) {
            nodes[from].neighbors.emplace_back(to, cost);
        }
    }
    
    // A* pathfinding algorithm
    std::pair<std::vector<int>, double> findPath(int start, int goal) {
        std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openSet;
        std::unordered_set<int> closedSet;
        std::unordered_map<int, double> gScore;
        std::unordered_map<int, int> parent;
        
        // Initialize start node
        gScore[start] = 0.0;
        double fScore = heuristic(start, goal);
        openSet.push(AStarNode(start, 0.0, fScore, -1));
        
        while (!openSet.empty()) {
            AStarNode current = openSet.top();
            openSet.pop();
            
            // Skip if already processed
            if (closedSet.find(current.id) != closedSet.end()) {
                continue;
            }
            
            closedSet.insert(current.id);
            
            // Goal reached
            if (current.id == goal) {
                std::vector<int> path;
                int nodeId = goal;
                double totalCost = current.gScore;
                
                // Reconstruct path
                while (nodeId != -1) {
                    path.push_back(nodeId);
                    nodeId = (parent.find(nodeId) != parent.end()) ? parent[nodeId] : -1;
                }
                
                std::reverse(path.begin(), path.end());
                return {path, totalCost};
            }
            
            // Process neighbors
            if (nodes.find(current.id) != nodes.end()) {
                for (const auto& [neighborId, edgeCost] : nodes[current.id].neighbors) {
                    if (closedSet.find(neighborId) != closedSet.end()) {
                        continue;
                    }
                    
                    double tentativeGScore = current.gScore + edgeCost;
                    
                    // Better path found
                    if (gScore.find(neighborId) == gScore.end() || 
                        tentativeGScore < gScore[neighborId]) {
                        
                        gScore[neighborId] = tentativeGScore;
                        parent[neighborId] = current.id;
                        
                        double fScoreNeighbor = tentativeGScore + heuristic(neighborId, goal);
                        openSet.push(AStarNode(neighborId, tentativeGScore, fScoreNeighbor, current.id));
                    }
                }
            }
        }
        
        // No path found
        return {{}, std::numeric_limits<double>::infinity()};
    }
};
