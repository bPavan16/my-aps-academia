// Generic C++ implementation of Floyd-Warshall algorithm
#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>

class FloydWarshallAlgorithm {
private:
    int vertices;
    std::vector<std::vector<double>> distanceMatrix;
    std::vector<std::vector<int>> nextMatrix;
    const double INF = std::numeric_limits<double>::infinity();

public:
    // Constructor
    FloydWarshallAlgorithm(int numVertices) : vertices(numVertices) {
        distanceMatrix.assign(vertices, std::vector<double>(vertices, INF));
        nextMatrix.assign(vertices, std::vector<int>(vertices, -1));
        
        // Initialize diagonal to 0
        for (int i = 0; i < vertices; i++) {
            distanceMatrix[i][i] = 0.0;
        }
    }
    
    // Add weighted edge to the graph
    void addEdge(int from, int to, double weight) {
        distanceMatrix[from][to] = weight;
        nextMatrix[from][to] = to;
    }
    
    // Floyd-Warshall algorithm for all-pairs shortest paths
    void computeShortestPaths() {
        // Main Floyd-Warshall algorithm
        for (int k = 0; k < vertices; k++) {
            for (int i = 0; i < vertices; i++) {
                for (int j = 0; j < vertices; j++) {
                    // If path through k is shorter
                    if (distanceMatrix[i][k] != INF && 
                        distanceMatrix[k][j] != INF &&
                        distanceMatrix[i][k] + distanceMatrix[k][j] < distanceMatrix[i][j]) {
                        
                        distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];
                        nextMatrix[i][j] = nextMatrix[i][k];
                    }
                }
            }
        }
    }
    
    // Get shortest distance between two vertices
    double getDistance(int from, int to) const {
        return distanceMatrix[from][to];
    }
    
    // Reconstruct path between two vertices
    std::vector<int> getPath(int from, int to) const {
        std::vector<int> path;
        
        if (distanceMatrix[from][to] == INF) {
            return path;  // No path exists
        }
        
        int current = from;
        path.push_back(current);
        
        while (current != to) {
            current = nextMatrix[current][to];
            if (current == -1) {
                path.clear();
                break;
            }
            path.push_back(current);
        }
        
        return path;
    }
    
    // Check for negative cycles
    bool hasNegativeCycle() const {
        for (int i = 0; i < vertices; i++) {
            if (distanceMatrix[i][i] < 0) {
                return true;
            }
        }
        return false;
    }
};
