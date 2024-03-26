#ifndef ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP
#define ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP

#include "AttackGraph.hpp"
#include "PriorityQueue.hpp"
#include "VirtualGraph.hpp"
#include <optional>
#include <vector>
#include <queue>
#include <limits>
#include <forward_list>


std::forward_list<Edge> constructTrace(AttackGraph &graph) {
    std::queue<GraphNode *> q;
    std::forward_list<Edge> results;

    q.push(graph.getNodeAt(graph.getGoalId()));
    while (!q.empty()) {
        auto v = q.front();
        q.pop();

        if (v->getType() == OR) {
            auto u = v->getParent();
            q.push(u);
            results.emplace_front(*u, *v);
        }
        if (v->getType() == AND) {
            for (auto u: v->getRevAdj()) {
                q.push(u);
                results.emplace_front(*u, *v);
            }
        }
    }
    return results;
}

// Implements Alg. 1 of the IoTDI paper SAT(G)
std::optional<std::forward_list<Edge>> sat(AttackGraph &graph,
                                           const std::optional<std::vector<double>> &weight = std::nullopt) {
    auto pq = PriorityQueue(graph.getSize());
    const double INFINITY = std::numeric_limits<double>::max();

    // Initialize the data structure
    // Note: type, done, inDegree, and color is already correctly set up before the graph is passed in here
    for (auto *node: graph.getNodes()) {
        if (node == nullptr) continue;
        if (node->getType() == OR) {
            node->setWeight(INFINITY);
        } else if (node->getType() == AND) {
            node->setWeight(-INFINITY);
        } else {
            pq.push(node);
            node->setColor(GRAY);
        }
    }

    while (!pq.empty()) {
        auto u = pq.pop();
        if (u->getId() == graph.getGoalId()) {
            goto trace_found;
        }
        for (auto *v: u->getAdj()) {
            if (u->getColor() == BLACK) break;
            // For now just ignore the weights (default to IoTA 2022 settings: edges = 1, nodes = 0)
            double temp = u->getWeight() + 1 + 0;
            if (v->getType() == OR) {
                if (v->getDone() == 0) {
                    v->setWeight(temp);
                    v->setParent(*u);
                    pq.push(v);
                    v->setColor(GRAY);
                } else if (temp < v->getWeight()) {
                    v->setWeight(temp);
                    v->setParent(*u);
                    pq.decreaseKey(v->getPosInHeap(), temp);
                }
                v->incDone();
            } else if (v->getType() == AND) {
                v->setWeight(std::max(v->getWeight(), temp));
                v->incDone();
                if (v->getDone() == v->getInDegree()) {
                    pq.push(v);
                    v->setColor(GRAY);
                }
            }
        }
    }
    // No attack trace is found
    return std::nullopt;
    trace_found:
    // Extract the trace
    return constructTrace(graph);
}

// Check if adding an edge from src to dst will cause a cycle
// An adaptation of the Kahn's algorithm
inline bool cycleExists(AttackGraph &graph,
                        std::optional<GraphNode*> src = std::nullopt, std::optional<GraphNode*> dst = std::nullopt) {
    // Use a virtual graph for easy rollbacks
    auto vg = VirtualGraph(&graph);

    // Pretend the edge exists
    if (src != std::nullopt && dst != std::nullopt) {
        vg.addEdge(src.value(), *dst.value());
    }

    size_t edgeCount = 0;
    std::queue<GraphNode *> q;

    // Scan the vertexes for leaf nodes
    for (auto &node: graph.getNodes()) {
        if (node == nullptr) continue;
        auto deg = node->getInDegree();

        if (deg == 0) q.push(node);
        else edgeCount += deg;
    }

    // "Remove" edges that originates from the source
    while (!q.empty()) {
        auto u = q.front();
        q.pop();

        for (auto &v: u->getAdj()) {
            vg.removeEdge(u, *v);
            edgeCount -= 1;
            if (v->getInDegree() == 0) q.push(v);
        }
    }

    vg.rollback();
    return edgeCount > 0;
}

#endif //ATTACKGRAPHGENERATOR_SHORTESTTRACE_HPP
