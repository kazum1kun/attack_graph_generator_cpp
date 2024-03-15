#ifndef ATTACKGRAPHGENERATOR_GRAPHNODE_HPP
#define ATTACKGRAPHGENERATOR_GRAPHNODE_HPP

#include <string>
#include <unordered_set>
#include <utility>

enum NodeType {
    LEAF = 0,
    AND = 1,
    OR = 2
};

class GraphNode {
    int id;
    int iCap;
    int oCap;
    std::string desc;
    NodeType type;
    std::unordered_set<GraphNode *> predecessor;
    std::unordered_set<GraphNode *> adjacentNodes;
public:
    GraphNode(int id, NodeType type, std::string desc, int iCap, int oCap);

    ~GraphNode();

    void addPred(GraphNode *node);

    void addPred(std::unordered_set<GraphNode *> *predSet);

    void addAdj(GraphNode *node);

    bool predContains(std::unordered_set<GraphNode *> *otherSet) const;

    bool adjContains(GraphNode *node) const;

    void decICap();

    void decOCap();

    int getId() const;

    int getICap() const;

    int getOCap() const;

    std::string getDesc() const;

    NodeType getType() const;

    std::unordered_set<GraphNode *> *getPred();

    std::unordered_set<GraphNode *> *getAdj();
};

inline GraphNode::GraphNode(const int id, const NodeType type, std::string desc, const int iCap, const int oCap) {
    this->id = id;
    this->type = type;
    this->desc = std::move(desc);
    this->iCap = iCap;
    this->oCap = oCap;

    this->predecessor = std::unordered_set<GraphNode *>{};
    this->adjacentNodes = std::unordered_set<GraphNode *>{};
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

inline void GraphNode::decICap() {
    this->iCap--;
}

inline void GraphNode::decOCap() {
    this->oCap--;
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

inline int GraphNode::getOCap() const {
    return this->oCap;
}

inline int GraphNode::getICap() const {
    return this->iCap;
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

struct Edge {
    GraphNode *src;
    GraphNode *dst;
};

#endif //ATTACKGRAPHGENERATOR_GRAPHNODE_HPP
