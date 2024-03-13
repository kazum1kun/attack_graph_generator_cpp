#ifndef ATTACKGRAPHGENERATOR_NODE_HPP
#define ATTACKGRAPHGENERATOR_NODE_HPP

#include <string>
#include <unordered_set>
#include <utility>

enum NodeType {
    LEAF = 0,
    AND = 1,
    OR = 2
};

class Node {
    int id;
    int iCap;
    int oCap;
    std::string desc;
    NodeType type;
    std::unordered_set<Node *> predecessor;
    std::unordered_set<Node *> adjacentNodes;
public:
    Node(int id, NodeType type, std::string desc, int iCap, int oCap);
    ~Node();
    void addPred(Node* node);
    void addPred(std::unordered_set<Node*> *predSet);
    void addAdj(Node* node);
    bool predContains(std::unordered_set<Node*> *otherSet) const;
    bool adjContains(Node *node) const;
    void decICap();
    void decOCap();
    int getId() const;
    int getICap() const;
    int getOCap() const;
    std::string getDesc() const;
    NodeType getType() const;
    std::unordered_set<Node*>* getPred();
    std::unordered_set<Node*>* getAdj();
};

inline Node::Node(const int id, const NodeType type, std::string desc, const int iCap, const int oCap) {
    this->id = id;
    this->type = type;
    this->desc = std::move(desc);
    this->iCap = iCap;
    this->oCap = oCap;

    this->predecessor = std::unordered_set<Node*>{};
    this->adjacentNodes = std::unordered_set<Node*>{};
}

inline Node::~Node() = default;

inline void Node::addPred(Node *node) {
    this->predecessor.insert(node);
}

inline void Node::addPred(std::unordered_set<Node*> *predSet) {
    for (auto pred: *predSet) {
        this->addPred(pred);
    }
}

inline void Node::addAdj(Node *node) {
    this->adjacentNodes.insert(node);
}

inline void Node::decICap() {
    this->iCap--;
}

inline void Node::decOCap() {
    this->oCap--;
}

inline int Node::getId() const {
    return this->id;
}

inline NodeType Node::getType() const {
    return this->type;
}

inline std::unordered_set<Node *> *Node::getPred() {
    return &this->predecessor;
}

inline std::unordered_set<Node *> * Node::getAdj() {
    return &this->adjacentNodes;
}

inline bool Node::adjContains(Node *node) const {
    return this->adjacentNodes.contains(node);
}

inline int Node::getOCap() const {
    return this->oCap;
}

inline int Node::getICap() const {
    return this->iCap;
}

inline bool Node::predContains(std::unordered_set<Node*> *otherSet) const {
    for (const auto& elem: *otherSet) {
        if (!this->predecessor.contains(elem)) {
            return false;
        }
    }
    return true;
}

std::string Node::getDesc() const {
    return this->desc;
}


#endif //ATTACKGRAPHGENERATOR_NODE_HPP
