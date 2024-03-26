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
        for (const auto *node: this->nodes) {
            delete node;
        }
    }

    void addNode(GraphNode &node) {
        nodes[node.getId()] = &node;
    }

    void removeNode(int nodeId) {
        nodes.erase(std::remove_if(nodes.begin() + 1, nodes.end(),
                       [&](GraphNode* node) {return node->getId() == nodeId;}));
        size -= 1;
    }

    void addEdgeById(int src, int dst) const {
        getNodeAt(src)->addAdj(*getNodeAt(dst));
    }

    [[nodiscard]] GraphNode *getNodeAt(int index) const {
        return nodes[index];
    }

    std::vector<GraphNode *> &getNodes() {
        return nodes;
    }

    [[nodiscard]] int getGoalId() const {
        return goal;
    }

    [[nodiscard]] int getSize() const {
        return size;
    }

    [[nodiscard]] int getNumEdges() const {
        return numEdges;
    }
};

#endif //ATTACKGRAPHGENERATOR_ATTACKGRAPH_HPP
