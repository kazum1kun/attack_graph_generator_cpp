#ifndef ATTACKGRAPHGENERATOR_ATTACKGRAPH_H
#define ATTACKGRAPHGENERATOR_ATTACKGRAPH_H

#include <vector>
#include "Node.h"


class AttackGraph {
private:
    int goal;
    std::vector<Node*> nodes;
public:
    AttackGraph(int goal, int numNodes);
    ~AttackGraph();
    void addNode(Node *node);
    Node *getNode(int index);
    int getGoal() const;
};

AttackGraph::AttackGraph(int goal, int numNodes) {
    this->goal = goal;
    this->nodes = std::vector<Node*>(numNodes + 1);
    this->nodes.push_back(nullptr);
}

AttackGraph::~AttackGraph() = default;

void AttackGraph::addNode(Node *node) {
    this->nodes.push_back(node);
}

Node *AttackGraph::getNode(int index) {
    return this->nodes[index];
}

int AttackGraph::getGoal() const {
    return this->goal;
}

#endif //ATTACKGRAPHGENERATOR_ATTACKGRAPH_H
