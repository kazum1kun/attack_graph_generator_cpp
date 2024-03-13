#ifndef ATTACKGRAPHGENERATOR_ATTACKGRAPH_HPP
#define ATTACKGRAPHGENERATOR_ATTACKGRAPH_HPP

#include <vector>
#include "Node.hpp"


class AttackGraph {
private:
    int goal;
    int size;
    int numEdges;
    std::vector<Node*> nodes;
public:
    AttackGraph(int goal, int numNodes, int numEdges);
    ~AttackGraph();
    void addNode(Node *node);
    [[nodiscard]] Node *getNode(int index) const;
    [[nodiscard]] int getGoal() const;
    [[nodiscard]] int getSize() const;
    [[nodiscard]] int getNumEdges() const;
};

inline AttackGraph::AttackGraph(const int goal, const int numNodes, const int numEdges) {
    this->goal = goal;
    this->nodes = std::vector<Node*>(numNodes + 1);
    this->size = numNodes;
    this->numEdges = numEdges;
    this->nodes.push_back(nullptr);
}

inline AttackGraph::~AttackGraph() = default;

inline void AttackGraph::addNode(Node *node) {
    this->nodes.push_back(node);
}

inline Node *AttackGraph::getNode(const int index) const {
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
