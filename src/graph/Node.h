#ifndef ATTACKGRAPHGENERATOR_NODE_H
#define ATTACKGRAPHGENERATOR_NODE_H

#include <string>
#include <unordered_set>
#include <utility>

enum NodeType {
    LEAF = 0,
    AND = 1,
    OR = 2
};

class Node {
private:
    int id;
    int iCap;
    int oCap;
    std::string desc;
    NodeType type;
    std::unordered_set<Node*> predecessor;
    std::unordered_set<Node*> adjacentNodes;
public:
    Node(int id, NodeType type, std::string desc, int iCap, int oCap);
    ~Node();
    void addPred(Node* pred);
    void addAdj(Node* node);
    void decICap();
    void decOCap();
    int getId() const;
    NodeType getType();
    std::unordered_set<Node*> getPred();
};

Node::Node(int id, NodeType type, std::string desc, int iCap, int oCap) {
    this->id = id;
    this->type = type;
    this->desc = std::move(desc);
    this->iCap = iCap;
    this->oCap = oCap;

    this->predecessor = std::unordered_set<Node*>();
    this->adjacentNodes = std::unordered_set<Node*>();
}

Node::~Node() = default;

void Node::addPred(Node *node) {
    this->predecessor.insert(node);
}

void Node::addAdj(Node *node) {
    this->adjacentNodes.insert(node);
}

void Node::decICap() {
    this->iCap--;
}

void Node::decOCap() {
    this->oCap--;
}

int Node::getId() const {
    return this->id;
}

NodeType Node::getType() {
    return this->type;
}

std::unordered_set<Node *> Node::getPred() {
    return this->predecessor;
}


#endif //ATTACKGRAPHGENERATOR_NODE_H
