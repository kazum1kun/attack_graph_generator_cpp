#ifndef ATTACKGRAPHGENERATOR_VIRTUALGRAPH_HPP
#define ATTACKGRAPHGENERATOR_VIRTUALGRAPH_HPP

#include <AttackGraph.hpp>
#include <stack>

enum OpType {
    AddEdge = 1,
    RemoveEdge = 2,
    // More can be added as needed
};

struct EdgeOpData {
    GraphNode& src;
    GraphNode& dst;
};

union GraphOpData {
    EdgeOpData e;
    // More to be added
};

struct RealGraphOp {
    OpType type;
    GraphOpData data;
};

class VirtualGraph {
private:
    AttackGraph *graph;
    std::stack<RealGraphOp> history;

public:
    explicit VirtualGraph(AttackGraph *graph) {
        this->graph = graph;
    }

    void addEdge(GraphNode *src, GraphNode &dst) {
        src->addAdj(dst);
        history.push({OpType::AddEdge, *src, dst});
    }

    void removeEdge(GraphNode *src, GraphNode &dst) {
        src->removeAdj(dst);
        history.push({OpType::RemoveEdge, *src, dst});
    }

    void rollback() {
        while (!history.empty()) {
            auto op = history.top();
            switch (op.type) {
                case AddEdge: {
                    op.data.e.src.removeAdj(op.data.e.dst);
                    break;
                }
                case RemoveEdge: {
                    op.data.e.src.addAdj(op.data.e.dst);
                    break;
                }
            }
            history.pop();
        }
    }
};

#endif //ATTACKGRAPHGENERATOR_VIRTUALGRAPH_HPP
