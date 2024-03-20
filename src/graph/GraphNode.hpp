#ifndef ATTACKGRAPHGENERATOR_GRAPHNODE_HPP
#define ATTACKGRAPHGENERATOR_GRAPHNODE_HPP

#include <string>
#include <unordered_set>
#include <unordered_map>
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
    std::string desc;
    NodeType type;
    std::unordered_set<GraphNode *> predecessor;
    std::unordered_set<GraphNode *> adjacentNodes;
    std::unordered_map<GraphNode *, std::list<Edge>::iterator> edgePointerMap;
public:
    GraphNode(int id, NodeType type, std::string desc, int iCap, int oCap);

    ~GraphNode();

    void addPred(GraphNode *node);

    void addPred(std::unordered_set<GraphNode *> *predSet);

    void addAdj(GraphNode *node);

    void addEdgePointer(GraphNode *dst, std::list<Edge>::iterator it);

    void removeEdgePointer(GraphNode *dst);

    std::list<Edge>::iterator getEdgePointer(GraphNode *dst);

    std::unordered_map<GraphNode *, std::list<Edge>::iterator> getAllEdgePointers();

    void clearEdgePointers();

    bool predContains(std::unordered_set<GraphNode *> *otherSet) const;

    bool adjContains(GraphNode *node) const;

    void incInDegree();

    void incOutDegree();

    int getInDegree();

    int getOutDegree();

    int getId() const;

    std::string getDesc() const;

    NodeType getType() const;

    std::unordered_set<GraphNode *> *getPred();

    std::unordered_set<GraphNode *> *getAdj();

    bool operator==(GraphNode const & other) const;

    bool operator!=(GraphNode const & other) const;
};

inline GraphNode::GraphNode(const int id, const NodeType type, std::string desc, const int iCap, const int oCap) {
    this->id = id;
    this->type = type;
    this->desc = std::move(desc);

    this->predecessor = std::unordered_set<GraphNode *>{};
    this->adjacentNodes = std::unordered_set<GraphNode *>{};
    this->edgePointerMap = std::unordered_map<GraphNode *, std::list<Edge>::iterator>{};
}

inline GraphNode::~GraphNode() = default;

inline void GraphNode::addPred(GraphNode *node) {
    this->predecessor.insert(node);
}

inline void GraphNode::addPred(std::unordered_set<GraphNode *> *predSet) {
    for (auto pred: *predSet) {
        this->addPred(pred);
    }
}

inline void GraphNode::addAdj(GraphNode *node) {
    this->adjacentNodes.insert(node);
}

inline int GraphNode::getId() const {
    return this->id;
}

inline NodeType GraphNode::getType() const {
    return this->type;
}

inline std::unordered_set<GraphNode *> *GraphNode::getPred() {
    return &this->predecessor;
}

inline std::unordered_set<GraphNode *> *GraphNode::getAdj() {
    return &this->adjacentNodes;
}

inline bool GraphNode::adjContains(GraphNode *node) const {
    return this->adjacentNodes.contains(node);
}

inline bool GraphNode::predContains(std::unordered_set<GraphNode *> *otherSet) const {
    for (const auto &elem: *otherSet) {
        if (!this->predecessor.contains(elem)) {
            return false;
        }
    }
    return true;
}

std::string GraphNode::getDesc() const {
    return this->desc;
}

bool GraphNode::operator==(const GraphNode &other) const {
    return this->id == other.id;
}

bool GraphNode::operator!=(const GraphNode &other) const {
    return !(*this == other);
}

void GraphNode::addEdgePointer(GraphNode *dst, std::list<Edge>::iterator it) {
    this->edgePointerMap.insert({dst, it});
}

void GraphNode::removeEdgePointer(GraphNode *dst) {
    this->edgePointerMap.erase(dst);
}

void GraphNode::clearEdgePointers() {
    this->edgePointerMap.clear();
}

std::list<Edge>::iterator GraphNode::getEdgePointer(GraphNode *dst) {
    return edgePointerMap[dst];
}

std::unordered_map<GraphNode *, std::list<Edge>::iterator> GraphNode::getAllEdgePointers() {
    return this->edgePointerMap;
}

void GraphNode::incInDegree() {
    this->inDegree += 1;
}

void GraphNode::incOutDegree() {
    this->outDegree += 1;
}

int GraphNode::getInDegree() {
    return this->inDegree;
}

int GraphNode::getOutDegree() {
    return this->outDegree;
}


struct Edge {
    GraphNode *src;
    GraphNode *dst;
};

#endif //ATTACKGRAPHGENERATOR_GRAPHNODE_HPP
