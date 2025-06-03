// Generic C++ implementation of Trust Network Builder
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>

class TrustNetworkBuilder {
private:
    struct Entity {
        int id;
        std::string type;  // "seller", "customer", "product"
        std::unordered_map<int, double> connections;  // id -> weight
        double trustScore;
        int transactionCount;
        double averageRating;
        
        Entity(int _id, const std::string& _type) 
            : id(_id), type(_type), trustScore(0.0), transactionCount(0), averageRating(0.0) {}
    };
    
    struct TrustSignal {
        int fromEntity;
        int toEntity;
        double rating;
        double transactionValue;
        long timestamp;
        std::string signalType;  // "review", "purchase", "dispute", "resolution"
        
        TrustSignal(int from, int to, double r, double value, 
                   long time, const std::string& type)
            : fromEntity(from), toEntity(to), rating(r), transactionValue(value), 
              timestamp(time), signalType(type) {}
    };
    
    std::unordered_map<int, Entity> entities;
    std::vector<TrustSignal> trustSignals;
    double timeDecayFactor;
    double minTrustThreshold;

public:
    // Constructor
    TrustNetworkBuilder(double decayFactor = 0.95, double minThreshold = 0.1)
        : timeDecayFactor(decayFactor), minTrustThreshold(minThreshold) {}
    
    // Add entity to the network
    void addEntity(int entityId, const std::string& type) {
        if (entities.find(entityId) == entities.end()) {
            entities[entityId] = Entity(entityId, type);
        }
    }
    
    // Add trust signal between entities
    void addTrustSignal(int fromEntity, int toEntity, double rating,
                      double transactionValue, long timestamp, 
                      const std::string& signalType) {
        addEntity(fromEntity, "customer");  // Default type, can be overridden
        addEntity(toEntity, "seller");
        
        trustSignals.emplace_back(fromEntity, toEntity, rating, transactionValue, 
                                 timestamp, signalType);
    }
    
    // Calculate time-weighted trust score
    double calculateTimeWeight(long signalTimestamp, long currentTime) const {
        long timeDifference = currentTime - signalTimestamp;
        double daysDifference = timeDifference / (24.0 * 60.0 * 60.0);  // Convert to days
        return std::pow(timeDecayFactor, daysDifference);
    }
    
    // Build trust network connections
    void buildTrustNetwork(long currentTime) {
        // Clear existing connections
        for (auto& [entityId, entity] : entities) {
            entity.connections.clear();
            entity.transactionCount = 0;
            entity.averageRating = 0.0;
        }
        
        // Process trust signals
        std::unordered_map<int, std::vector<double>> ratingHistory;
        
        for (const auto& signal : trustSignals) {
            double timeWeight = calculateTimeWeight(signal.timestamp, currentTime);
            double trustWeight = calculateTrustWeight(signal, timeWeight);
            
            if (trustWeight >= minTrustThreshold) {
                entities[signal.fromEntity].connections[signal.toEntity] += trustWeight;
                entities[signal.toEntity].transactionCount++;
                ratingHistory[signal.toEntity].push_back(signal.rating);
            }
        }
        
        // Calculate average ratings
        for (auto& [entityId, ratings] : ratingHistory) {
            if (!ratings.empty()) {
                double sum = 0.0;
                for (double rating : ratings) {
                    sum += rating;
                }
                entities[entityId].averageRating = sum / ratings.size();
            }
        }
    }
    
private:
    // Calculate trust weight for a signal
    double calculateTrustWeight(const TrustSignal& signal, double timeWeight) const {
        double baseWeight = 1.0;
        
        // Adjust weight based on signal type
        if (signal.signalType == "review") {
            baseWeight = signal.rating / 5.0;  // Normalize to 0-1
        } else if (signal.signalType == "dispute") {
            baseWeight = 0.1;  // Negative signal
        } else if (signal.signalType == "resolution") {
            baseWeight = 0.8;  // Positive resolution signal
        }
        
        // Adjust weight based on transaction value
        double valueWeight = std::min(1.0, signal.transactionValue / 100.0);  // Cap at $100
        
        return baseWeight * timeWeight * valueWeight;
    }

public:
    // Export trust network for PageRank computation
    std::pair<std::vector<std::pair<int, int>>, std::vector<double>> exportTrustNetwork() const {
        std::vector<std::pair<int, int>> edges;
        std::vector<double> weights;
        
        for (const auto& [fromId, entity] : entities) {
            for (const auto& [toId, weight] : entity.connections) {
                edges.emplace_back(fromId, toId);
                weights.push_back(weight);
            }
        }
        
        return {edges, weights};
    }
    
    // Get entities by type
    std::vector<int> getEntitiesByType(const std::string& type) const {
        std::vector<int> result;
        for (const auto& [entityId, entity] : entities) {
            if (entity.type == type) {
                result.push_back(entityId);
            }
        }
        return result;
    }
    
    // Get entity statistics
    void printEntityStats(int entityId) const {
        auto it = entities.find(entityId);
        if (it != entities.end()) {
            const Entity& entity = it->second;
            std::cout << "Entity " << entityId << " (" << entity.type << "):" << std::endl;
            std::cout << "  Transaction Count: " << entity.transactionCount << std::endl;
            std::cout << "  Average Rating: " << entity.averageRating << std::endl;
            std::cout << "  Outgoing Connections: " << entity.connections.size() << std::endl;
            std::cout << "  Trust Score: " << entity.trustScore << std::endl;
        }
    }
    
    // Detect potential manipulation attempts
    std::vector<int> detectAnomalousEntities() const {
        std::vector<int> suspicious;
        
        for (const auto& [entityId, entity] : entities) {
            // Check for unusual patterns
            bool suspiciousPattern = false;
            
            // Too many connections with low individual weights
            if (entity.connections.size() > 100) {
                double avgWeight = 0.0;
                for (const auto& [toId, weight] : entity.connections) {
                    avgWeight += weight;
                }
                avgWeight /= entity.connections.size();
                
                if (avgWeight < 0.2) {
                    suspiciousPattern = true;
                }
            }
            
            // Unusually high rating with low transaction count
            if (entity.averageRating > 4.8 && entity.transactionCount < 10) {
                suspiciousPattern = true;
            }
            
            if (suspiciousPattern) {
                suspicious.push_back(entityId);
            }
        }
        
        return suspicious;
    }
};

