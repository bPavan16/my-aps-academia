// Generic C++ implementation of PageRank algorithm
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <iomanip>

class PageRankAlgorithm {
private:
    struct Node {
        int id;
        std::vector<int> outgoing;  // Outgoing link IDs
        std::vector<double> weights;  // Edge weights
        double pagerank;
        double newPagerank;
        
        Node(int _id) : id(_id), pagerank(1.0), newPagerank(0.0) {}
    };
    
    std::unordered_map<int, Node> nodes;
    double dampingFactor;
    double convergenceThreshold;
    int maxIterations;
    std::vector<double> personalizationVector;

public:
    // Constructor
    PageRankAlgorithm(double damping = 0.85, double threshold = 1e-6, int maxIter = 100) 
        : dampingFactor(damping), convergenceThreshold(threshold), maxIterations(maxIter) {}
    
    // Add node to the graph
    void addNode(int nodeId) {
        if (nodes.find(nodeId) == nodes.end()) {
            nodes[nodeId] = Node(nodeId);
        }
    }
    
    // Add weighted edge between nodes
    void addEdge(int from, int to, double weight = 1.0) {
        addNode(from);
        addNode(to);
        
        nodes[from].outgoing.push_back(to);
        nodes[from].weights.push_back(weight);
    }
    
    // Set personalization vector for topic-sensitive PageRank
    void setPersonalizationVector(const std::vector<double>& personalVector) {
        personalizationVector = personalVector;
    }
    
    // Initialize PageRank scores
    void initializePageRank() {
        double initialValue = 1.0 / nodes.size();
        for (auto& [nodeId, node] : nodes) {
            node.pagerank = initialValue;
            node.newPagerank = 0.0;
        }
    }
    
    // Compute PageRank using power iteration method
    std::pair<bool, int> computePageRank() {
        initializePageRank();
        int numNodes = nodes.size();
        
        for (int iteration = 0; iteration < maxIterations; iteration++) {
            // Reset new PageRank values
            for (auto& [nodeId, node] : nodes) {
                node.newPagerank = 0.0;
            }
            
            // Calculate new PageRank values
            for (auto& [nodeId, node] : nodes) {
                if (!node.outgoing.empty()) {
                    // Calculate total weight for normalization
                    double totalWeight = 0.0;
                    for (double weight : node.weights) {
                        totalWeight += weight;
                    }
                    
                    // Distribute PageRank to outgoing nodes
                    for (size_t i = 0; i < node.outgoing.size(); i++) {
                        int targetNode = node.outgoing[i];
                        double weight = node.weights[i];
                        double contribution = (node.pagerank * weight) / totalWeight;
                        nodes[targetNode].newPagerank += dampingFactor * contribution;
                    }
                } else {
                    // Handle dangling nodes (no outgoing links)
                    double contribution = node.pagerank / numNodes;
                    for (auto& [targetId, targetNode] : nodes) {
                        targetNode.newPagerank += dampingFactor * contribution;
                    }
                }
            }
            
            // Add random jump probability
            for (auto& [nodeId, node] : nodes) {
                if (personalizationVector.empty()) {
                    node.newPagerank += (1.0 - dampingFactor) / numNodes;
                } else {
                    // Use personalization vector if provided
                    if (nodeId < personalizationVector.size()) {
                        node.newPagerank += (1.0 - dampingFactor) * personalizationVector[nodeId];
                    }
                }
            }
            
            // Check for convergence
            double maxDifference = 0.0;
            for (auto& [nodeId, node] : nodes) {
                double difference = std::abs(node.newPagerank - node.pagerank);
                maxDifference = std::max(maxDifference, difference);
                node.pagerank = node.newPagerank;
            }
            
            if (maxDifference < convergenceThreshold) {
                return {true, iteration + 1};  // Converged
            }
        }
        
        return {false, maxIterations};  // Did not converge
    }
    
    // Get PageRank score for a specific node
    double getPageRank(int nodeId) const {
        auto it = nodes.find(nodeId);
        return (it != nodes.end()) ? it->second.pagerank : 0.0;
    }
    
    // Get all PageRank scores sorted by value
    std::vector<std::pair<int, double>> getAllPageRanks(bool sorted = true) const {
        std::vector<std::pair<int, double>> results;
        
        for (const auto& [nodeId, node] : nodes) {
            results.emplace_back(nodeId, node.pagerank);
        }
        
        if (sorted) {
            std::sort(results.begin(), results.end(), 
                     [](const auto& a, const auto& b) {
                         return a.second > b.second;  // Sort by PageRank descending
                     });
        }
        
        return results;
    }
    
    // Normalize PageRank scores to sum to 1
    void normalizePageRanks() {
        double totalPageRank = 0.0;
        for (const auto& [nodeId, node] : nodes) {
            totalPageRank += node.pagerank;
        }
        
        if (totalPageRank > 0.0) {
            for (auto& [nodeId, node] : nodes) {
                node.pagerank /= totalPageRank;
            }
        }
    }
    
    // Print PageRank results
    void printResults(int topK = 10) const {
        auto results = getAllPageRanks(true);
        
        std::cout << "Top " << std::min(topK, (int)results.size()) 
                  << " nodes by PageRank:" << std::endl;
        std::cout << std::fixed << std::setprecision(6);
        
        for (int i = 0; i < std::min(topK, (int)results.size()); i++) {
            std::cout << "Node " << results[i].first 
                      << ": " << results[i].second << std::endl;
        }
    }
    
    // Calculate network statistics
    void printNetworkStats() const {
        int totalNodes = nodes.size();
        int totalEdges = 0;
        int danglingNodes = 0;
        
        for (const auto& [nodeId, node] : nodes) {
            totalEdges += node.outgoing.size();
            if (node.outgoing.empty()) {
                danglingNodes++;
            }
        }
        
        std::cout << "Network Statistics:" << std::endl;
        std::cout << "Total Nodes: " << totalNodes << std::endl;
        std::cout << "Total Edges: " << totalEdges << std::endl;
        std::cout << "Dangling Nodes: " << danglingNodes << std::endl;
        std::cout << "Average Out-degree: " 
                  << (double)totalEdges / totalNodes << std::endl;
    }
};

