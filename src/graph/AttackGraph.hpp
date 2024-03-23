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
    AttackGraph(int goal, int numNodes, int numEdges) {
        this->goal = goal;
        this->nodes = std::vector<GraphNode *>(numNodes + 1);
        this->size = numNodes;
        this->numEdges = numEdges;
    }

    ~AttackGraph() {
        for (const auto &node: this->nodes) {
            delete node;
        }
    }

    void addNode(GraphNode &node) {
        this->nodes[node.getId()] = &node;
    }

    void addEdgeById(int src, int dst) const {
        getNode(src).addAdj(getNode(dst));
    }

    [[nodiscard]] GraphNode &getNode(int index) const {
        return *this->nodes[index];
    }

    std::vector<GraphNode *>& getNodes() {
        return nodes;
    }

    [[nodiscard]] int getGoalId() const {
        return this->goal;
    }

    [[nodiscard]] int getSize() const {
        return this->size;
    }

    [[nodiscard]] int getNumEdges() const {
        return numEdges;
    }
};

#endif //ATTACKGRAPHGENERATOR_ATTACKGRAPH_HPP
