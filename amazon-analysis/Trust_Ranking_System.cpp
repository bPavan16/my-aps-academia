// Generic C++ implementation of Trust Ranking System
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <iomanip>

class TrustRankingSystem {
private:
    struct TrustMetrics {
        double pageRankScore;
        double directFeedbackScore;
        double disputeResolutionScore;
        double networkTrustScore;
        double temporalStabilityScore;
        double compositeTrustScore;
        
        TrustMetrics() : pageRankScore(0.0), directFeedbackScore(0.0), 
                        disputeResolutionScore(0.0), networkTrustScore(0.0),
                        temporalStabilityScore(0.0), compositeTrustScore(0.0) {}
    };
    
    std::unordered_map<int, TrustMetrics> entityTrustMetrics;
    std::vector<double> weightVector;  // Weights for different trust components
    double pageRankWeight;
    double feedbackWeight;
    double disputeWeight;
    double networkWeight;
    double stabilityWeight;

public:
    // Constructor with component weights
    TrustRankingSystem(double prWeight = 0.3, double fbWeight = 0.25,
                      double dispWeight = 0.2, double netWeight = 0.15,
                      double stabWeight = 0.1)
        : pageRankWeight(prWeight), feedbackWeight(fbWeight), disputeWeight(dispWeight),
          networkWeight(netWeight), stabilityWeight(stabWeight) {
        
        // Normalize weights to sum to 1
        double totalWeight = prWeight + fbWeight + dispWeight + netWeight + stabWeight;
        pageRankWeight /= totalWeight;
        feedbackWeight /= totalWeight;
        disputeWeight /= totalWeight;
        networkWeight /= totalWeight;
        stabilityWeight /= totalWeight;
    }
    
    // Set PageRank scores for entities
    void setPageRankScores(const std::vector<std::pair<int, double>>& pageRankResults) {
        for (const auto& [entityId, score] : pageRankResults) {
            entityTrustMetrics[entityId].pageRankScore = score;
        }
    }
    
    // Calculate direct feedback score
    void calculateDirectFeedbackScore(int entityId, double averageRating, 
                                        int reviewCount, double ratingVariance) {
        if (reviewCount == 0) {
            entityTrustMetrics[entityId].directFeedbackScore = 0.0;
            return;
        }
        
        // Normalize rating to 0-1 scale
        double normalizedRating = (averageRating - 1.0) / 4.0;  // Assuming 1-5 scale
        
        // Review count factor (logarithmic scaling)
        double countFactor = std::min(1.0, std::log10(reviewCount + 1) / 3.0);
        
        // Consistency factor (lower variance is better)
        double consistencyFactor = 1.0 / (1.0 + ratingVariance);
        
        double feedbackScore = normalizedRating * countFactor * consistencyFactor;
        entityTrustMetrics[entityId].directFeedbackScore = feedbackScore;
    }
    
    // Calculate dispute resolution score
    void calculateDisputeResolutionScore(int entityId, int totalDisputes, 
                                          int resolvedDisputes, double avgResolutionTime,
                                          double customerSatisfactionRate) {
        if (totalDisputes == 0) {
            entityTrustMetrics[entityId].disputeResolutionScore = 1.0;  // Perfect score for no disputes
            return;
        }
        
        // Resolution rate
        double resolutionRate = (double)resolvedDisputes / totalDisputes;
        
        // Time factor (faster resolution is better)
        double timeFactor = 1.0 / (1.0 + avgResolutionTime / 24.0);  // Normalize by 24 hours
        
        // Satisfaction factor
        double satisfactionFactor = customerSatisfactionRate;
        
        // Penalty for high dispute rate
        double disputeRatePenalty = std::max(0.1, 1.0 - totalDisputes / 100.0);
        
        double disputeScore = resolutionRate * timeFactor * satisfactionFactor * disputeRatePenalty;
        entityTrustMetrics[entityId].disputeResolutionScore = std::min(1.0, disputeScore);
    }
    
    // Calculate network trust score
    void calculateNetworkTrustScore(int entityId, int trustedConnections,
                                   int totalConnections, double averageConnectionTrust) {
        if (totalConnections == 0) {
            entityTrustMetrics[entityId].networkTrustScore = 0.5;  // Neutral score
            return;
        }
        
        // Trust ratio
        double trustRatio = (double)trustedConnections / totalConnections;
        
        // Average trust quality
        double trustQuality = averageConnectionTrust;
        
        // Network diversity bonus (connections with different entity types)
        double diversityBonus = std::min(1.2, 1.0 + totalConnections / 1000.0);
        
        double networkScore = trustRatio * trustQuality * diversityBonus;
        entityTrustMetrics[entityId].networkTrustScore = std::min(1.0, networkScore);
    }
    
    // Calculate temporal stability score
    void calculateTemporalStabilityScore(int entityId, 
                                          const std::vector<double>& monthlyRatings) {
        if (monthlyRatings.size() < 2) {
            entityTrustMetrics[entityId].temporalStabilityScore = 0.5;
            return;
        }
        
        // Calculate variance in monthly ratings
        double mean = 0.0;
        for (double rating : monthlyRatings) {
            mean += rating;
        }
        mean /= monthlyRatings.size();
        
        double variance = 0.0;
        for (double rating : monthlyRatings) {
            variance += (rating - mean) * (rating - mean);
        }
        variance /= monthlyRatings.size();
        
        // Stability score (lower variance = higher stability)
        double stabilityScore = 1.0 / (1.0 + variance);
        
        // Bonus for longer history
        double historyBonus = std::min(1.2, 1.0 + monthlyRatings.size() / 12.0);
        
        entityTrustMetrics[entityId].temporalStabilityScore = std::min(1.0, stabilityScore * historyBonus);
    }
    
    // Calculate composite trust score
    void calculateCompositeTrustScore(int entityId) {
        TrustMetrics& metrics = entityTrustMetrics[entityId];
        
        metrics.compositeTrustScore = 
            metrics.pageRankScore * pageRankWeight +
            metrics.directFeedbackScore * feedbackWeight +
            metrics.disputeResolutionScore * disputeWeight +
            metrics.networkTrustScore * networkWeight +
            metrics.temporalStabilityScore * stabilityWeight;
    }
    
    // Get trust ranking for all entities
    std::vector<std::pair<int, double>> getTrustRanking(bool sorted = true) {
        std::vector<std::pair<int, double>> ranking;
        
        for (const auto& [entityId, metrics] : entityTrustMetrics) {
            ranking.emplace_back(entityId, metrics.compositeTrustScore);
        }
        
        if (sorted) {
            std::sort(ranking.begin(), ranking.end(),
                     [](const auto& a, const auto& b) {
                         return a.second > b.second;
                     });
        }
        
        return ranking;
    }
    
    // Get detailed trust metrics for an entity
    TrustMetrics getTrustMetrics(int entityId) const {
        auto it = entityTrustMetrics.find(entityId);
        return (it != entityTrustMetrics.end()) ? it->second : TrustMetrics();
    }
    
    // Print trust analysis for an entity
    void printTrustAnalysis(int entityId) const {
        auto it = entityTrustMetrics.find(entityId);
        if (it == entityTrustMetrics.end()) {
            std::cout << "Entity " << entityId << " not found." << std::endl;
            return;
        }
        
        const TrustMetrics& metrics = it->second;
        
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Trust Analysis for Entity " << entityId << ":" << std::endl;
        std::cout << "  PageRank Score: " << metrics.pageRankScore << " (weight: " << pageRankWeight << ")" << std::endl;
        std::cout << "  Direct Feedback: " << metrics.directFeedbackScore << " (weight: " << feedbackWeight << ")" << std::endl;
        std::cout << "  Dispute Resolution: " << metrics.disputeResolutionScore << " (weight: " << disputeWeight << ")" << std::endl;
        std::cout << "  Network Trust: " << metrics.networkTrustScore << " (weight: " << networkWeight << ")" << std::endl;
        std::cout << "  Temporal Stability: " << metrics.temporalStabilityScore << " (weight: " << stabilityWeight << ")" << std::endl;
        std::cout << "  Composite Trust Score: " << metrics.compositeTrustScore << std::endl;
    }
};
