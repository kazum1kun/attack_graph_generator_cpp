#ifndef ATTACKGRAPHGENERATOR_ATTACKGRAPH_HPP
#define ATTACKGRAPHGENERATOR_ATTACKGRAPH_HPP

#include <vector>
#include "GraphNode.hpp"


class AttackGraph {
private:
    int goal;
    int size;
    int numEdges;
    std::vector<GraphNode *> nodes;
public:
    AttackGraph(int goal, int numNodes, int numEdges);

    ~AttackGraph();

    void addNode(GraphNode *node);

    [[nodiscard]] GraphNode *getNode(int index) const;

    [[nodiscard]] int getGoal() const;

    [[nodiscard]] int getSize() const;

    [[nodiscard]] int getNumEdges() const;
};

inline AttackGraph::AttackGraph(const int goal, const int numNodes, const int numEdges) {
    this->goal = goal;
    this->nodes = std::vector<GraphNode *>(numNodes + 1);
    this->size = numNodes;
    this->numEdges = numEdges;
}

inline AttackGraph::~AttackGraph() {
    for (const auto &node: this->nodes) {
        delete node;
    }
}

inline void AttackGraph::addNode(GraphNode *node) {
    this->nodes[node->getId()] = node;
}

inline GraphNode *AttackGraph::getNode(const int index) const {
    return this->nodes[index];
}

inline int AttackGraph::getGoal() const {
    return this->goal;
}

inline int AttackGraph::getSize() const {
    return this->size;
}

int AttackGraph::getNumEdges() const {
    return numEdges;
}

#endif //ATTACKGRAPHGENERATOR_ATTACKGRAPH_HPP
