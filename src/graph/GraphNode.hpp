#ifndef ATTACKGRAPHGENERATOR_GRAPHNODE_HPP
#define ATTACKGRAPHGENERATOR_GRAPHNODE_HPP

#include <string>
#include <unordered_set>
#include <list>
#include <utility>
#include "AttackGraph.hpp"

enum NodeType {
    LEAF = 0,
    AND = 1,
    OR = 2
};

struct Edge;

class GraphNode {
    int id;
    int inDegree;
    int outDegree;
    int availableAdj;
    std::string desc;
    NodeType type;
    std::unordered_set<GraphNode *> predecessor;
    std::list<GraphNode *> adjNodes;
    std::list<GraphNode *> possibleAdjNodes;
public:
    GraphNode(int id, NodeType type, std::string desc) {
        this -> id = id;
        this -> type = type;
        this -> desc = std::move(desc);
        inDegree = 0;
        outDegree = 0;
        availableAdj = 0;

        predecessor = std::unordered_set<GraphNode *>{};
        adjNodes = std::list<GraphNode *>{};
        possibleAdjNodes = std::list<GraphNode *>{};
    }

    ~GraphNode() = default;

    void addPred(GraphNode *pred) {
        predecessor.insert(pred);
    }

    void addPred(std::unordered_set<GraphNode *> *predSet) {
        for (auto pred: *predSet) {
            addPred(pred);
        }
    }

    void addPossibleAdj(GraphNode *dst) {
        possibleAdjNodes.insert(possibleAdjNodes.end(), dst);
        availableAdj += 1;
    }

    void removePossibleAdj(GraphNode *dst) {
        possibleAdjNodes.remove(dst);
        availableAdj -= 1;
    }

    std::list<GraphNode *>::const_iterator queryPossibleAdj(int index) {
        return std::next(possibleAdjNodes.cbegin(), index);
    }

    std::list<GraphNode *>::const_iterator queryPossibleAdjRev(int index) {
        return std::next(possibleAdjNodes.cend(), -index);
    }

    bool adjContains(GraphNode * query) {
        auto result = std::find(adjNodes.begin(), adjNodes.end(),query);
        return result != adjNodes.end();
    }

    void erasePossibleAdj(std::list<GraphNode *>::const_iterator it) {
        possibleAdjNodes.erase(it);
    }

    std::list<GraphNode *> getPossibleAdj() {
        return possibleAdjNodes;
    };

    void clearPossibleAdj() {
        possibleAdjNodes.clear();
        availableAdj = 0;
    }

    // Do the following
    // 1. Insert the node to the end of the adjacency list
    // 2. Increment the out-degree of the node
    // The main program loop will handle the possibleAdj as it's more efficient to do it there
    void addAdj(GraphNode *node) {
        adjNodes.insert(possibleAdjNodes.end(), node);
        outDegree += 1;
    }

    bool predContains(std::unordered_set<GraphNode *> *otherSet) const {
        for (const auto &elem: *otherSet) {
            if (!predecessor.contains(elem)) {
                return false;
            }
        }
        return true;
    }

    std::list<GraphNode *> getAdj() {
        return adjNodes;
    }

    int getAvailNum() const {
        return availableAdj;
    }

    void incInDegree() {
        inDegree += 1;
    }

    void incOutDegree() {
        outDegree += 1;
    }

    int getInDegree() const {
        return inDegree;
    }

    int getOutDegree() const {
        return outDegree;
    }

    int getId() const {
        return id;
    }

    std::string getDesc() const {
        return desc;
    }

    NodeType getType() const {
        return type;
    }

    std::unordered_set<GraphNode *> *getPred() {
        return &predecessor;
    }

    bool operator==(GraphNode const & other) const {
        return id == other.id;
    }

    bool operator!=(GraphNode const & other) const {
        return !(*this == other);
    }
};

struct Edge {
    GraphNode *src;
    GraphNode *dst;
};

#endif //ATTACKGRAPHGENERATOR_GRAPHNODE_HPP
