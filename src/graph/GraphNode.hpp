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

enum NodeColor {
    WHITE = 0,
    GRAY = 1,
    BLACK = 2
};

class GraphNode {
private:
    int id;
    int availableAdj;
    double weight;
    std::string desc;
    NodeType type;
    NodeColor color;
    GraphNode* parent;
    int done;
    int posInHeap;

    std::list<GraphNode *> adjNodes;
    std::list<GraphNode *> possibleAdjNodes;
    std::unordered_set<GraphNode *> revAdjNodes;
public:
    GraphNode(int id, NodeType type, std::string desc) {
        this -> id = id;
        this -> type = type;
        this -> desc = std::move(desc);
        availableAdj = 0;
        weight = 0.0;
        color = WHITE;
        done = 0;
        parent = nullptr;
        posInHeap = -1;
//
//        predecessor = std::unordered_set<GraphNode *>{};
//        adjNodes = std::list<GraphNode *>{};
//        possibleAdjNodes = std::list<GraphNode *>{};
    }

    ~GraphNode() = default;

    void addPossibleAdj(GraphNode *dst) {
        possibleAdjNodes.insert(possibleAdjNodes.end(), dst);
        availableAdj += 1;
    }

    void removePossibleAdj(GraphNode *dst) {
        possibleAdjNodes.remove(dst);
        availableAdj -= 1;
    }

    // Returns a constant_iterator that needs to be de-referenced before accessing the content it points to
    std::list<GraphNode *>::const_iterator queryPossibleAdj(int index) {
        return std::next(possibleAdjNodes.cbegin(), index);
    }

    // Returns a constant_iterator that needs to be de-referenced before accessing the content it points to
    std::list<GraphNode *>::const_iterator queryPossibleAdjRev(int index) {
        return std::next(possibleAdjNodes.cend(), -index-1);
    }

    bool adjContains(GraphNode * query) {
        auto result = std::find(adjNodes.begin(), adjNodes.end(),query);
        return result != adjNodes.end();
    }

    void erasePossibleAdj(std::list<GraphNode *>::const_iterator it) {
        possibleAdjNodes.erase(it);
        availableAdj -= 1;
    }

    std::list<GraphNode *> getPossibleAdj() {
        return possibleAdjNodes;
    };

    void clearPossibleAdj() {
        possibleAdjNodes.clear();
        availableAdj = 0;
    }

    void addAdj(GraphNode *dst) {
        // Forward edge
        adjNodes.insert(adjNodes.end(), dst);
        // Reverse edge (for calculating in-degree of dst other node)
        dst->addRevAdj(this);
    }

    void setPosInHeap(int index) {
        posInHeap = index;
    }

    int getPosInHeap() const {
        return posInHeap;
    }

    std::unordered_set<GraphNode *> getRevAdj() {
        return revAdjNodes;
    }

    void addRevAdj(GraphNode* src) {
        revAdjNodes.insert(src);
    }

    std::list<GraphNode *> getAdj() {
        return adjNodes;
    }

    int getAvailNum() const {
        return availableAdj;
    }

    size_t getInDegree() const {
        return revAdjNodes.size();
    }

    size_t getOutDegree() const {
        return adjNodes.size();
    }

    void setParent(GraphNode *newParent) {
        parent = newParent;
    }

    GraphNode *getParent() {
        return parent;
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

    double getWeight() const {
        return weight;
    }

    void setWeight(double newWeight) {
        weight = newWeight;
    }

    int getDone() const {
        return done;
    }

    void incDone() {
        done += 1;
    }

    NodeColor getColor() const {
        return color;
    }

    void setColor(NodeColor newColor) {
        color = newColor;
    }



    bool operator==(GraphNode const & other) const {
        return id == other.id;
    }

    bool operator!=(GraphNode const & other) const {
        return !(*this == other);
    }

    bool operator<(GraphNode const & other) const {
        return weight < other.getWeight();
    }
};

struct Edge {
    GraphNode *src;
    GraphNode *dst;
};

#endif //ATTACKGRAPHGENERATOR_GRAPHNODE_HPP
