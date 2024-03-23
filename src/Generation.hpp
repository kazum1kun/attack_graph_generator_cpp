#ifndef ATTACKGRAPHGENERATOR_GENERATION_HPP
#define ATTACKGRAPHGENERATOR_GENERATION_HPP

#include "AttackGraph.hpp"
#include "ShortestTrace.hpp"
#include "VirtualGraph.hpp"
#include <string>
#include <queue>
#include <list>
#include <ranges>
#include "effolkronium/random.hpp"

extern int verbosity;
extern bool manualStepping;


// Check if adding an edge from src to dst will cause a cycle
// An adaptation of the Kahn's algorithm
bool cycleExists(GraphNode &src, GraphNode &dst, AttackGraph &graph) {
    // Use a virtual graph for easy rollbacks
    auto vg = VirtualGraph(&graph);

    // Pretend the edge exists
    vg.addEdge(&src, dst);
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

inline bool addEdge(GraphNode *src, GraphNode &dst, const bool cycleOk, AttackGraph &graph) {
    if (verbosity > 1) {
        std::cout << "Attempting to add edge from " << src->getId() << " to " << dst.getId();
    }
    // Check for cycles if cycleOk is false (using Kahn's algorithm)
    if (!cycleOk && cycleExists(*src, dst, graph)) {
        if (verbosity > 1) {
            std::cout << "... failed: will form a cycle" << std::endl;
        }
        return false;
    }

    if (src->adjContains(dst)) {
        if (verbosity > 1) {
            std::cout << "... failed: already exists" << std::endl;
        }
        return false;
    }
    if (*src == dst) {
        if (verbosity > 1) {
            std::cout << "... failed: adding a self loop" << std::endl;
        }
        return false;
    }
    src->addAdj(dst);
    if (verbosity > 1) {
        std::cout << "... ok.\n" << std::endl;
    }

    return true;
}

inline AttackGraph *generateGraph(const int numOr, const int numAnd, const int numLeaf,
                                  int numEdge, const bool cycleOk, const bool relaxed, const bool alt, bool test,
                                  const unsigned long seed) {
    const int totalNode = numOr + numAnd + numLeaf;
    int totalEdge = 0;
    using Random = effolkronium::random_static;

    if (verbosity > 0) {
        std::cout << "Starting node generation." << std::endl;
    }

    Random::seed(seed);
    // Initialize a graph
    auto *graph = new AttackGraph(1, totalNode, numEdge);
    // Initialize nodes
    for (int i = 1; i <= totalNode; i++) {
        NodeType type;
        std::string desc;

        // Goal node
        if (i == 1) {
            type = OR;
            desc = "goal";
        } else if (i <= numOr) {
            type = OR;
            desc = "d" + std::to_string(i);
        } else if (i <= numOr + numLeaf) {
            type = LEAF;
            desc = "p" + std::to_string(i - numOr);
        } else {
            type = AND;
            desc = "r" + std::to_string(i - numOr - numLeaf);
        }

        auto typeStr = (type == LEAF) ? "LEAF" : (type == OR) ? "OR" : "AND";
        if (verbosity > 1) {
            std::cout << "Generated node: {id = " << i << ", type = " << typeStr << ", name = " << desc << "}"
                      << std::endl;
        }

        const auto node = new GraphNode(i, type, desc);
        graph->addNode(*node);
    }

    if (verbosity > 0) {
        std::cout << "Node generation complete.\n" << std::endl;
    }

    if (manualStepping) {
        std::cout << "\nr>";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }


    if (verbosity > 0) {
        std::cout << "Generating all possible edges...";
    }

    // Update the possible adjacent nodes at each node
    for (int srcId = 2; srcId <= totalNode; srcId++) {
        auto src = graph->getNodeAt(srcId);
        if (src->getType() == AND) {
            for (int dst = 1; dst <= numOr; dst++) {
                src->addPossibleAdj(*graph->getNodeAt(dst));
                totalEdge += 1;
            }
        } else {
            for (int dst = numOr + numLeaf + 1; dst <= totalNode; dst++) {
                src->addPossibleAdj(*graph->getNodeAt(dst));
                totalEdge += 1;
            }
        }
    }

    std::cout << " done.\n" << std::endl;

    if (verbosity > 2) {
        std::cout << "All possible edges: \n";
        int index = 0;
        for (int src = 2; src <= totalNode; src++) {
            for (auto &dst: graph->getNodeAt(src)->getPossibleAdj())
                std::cout << "index: " << index << " (" << src << ", " << dst->getId() << ") " << std::endl;
            index += 1;
        }
    }

    if (manualStepping) {
        std::cout << "\nr>";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (verbosity > 0) {
        std::cout << "Start picking edges" << std::endl;
    }

    // Generate an edge from an AND node to the goal node if alt mode is on
    if (alt) {
        auto src = graph->getNodeAt(Random::get(numOr + numLeaf + 1, totalNode));
        auto dst = graph->getNodeAt(graph->getGoalId());

        if (verbosity > 1) {
            std::cout << "Alt: generated an edge from " << src->getId() << " to " << dst->getId() << std::endl;
        }

        if (relaxed) {
            if (verbosity > 1) {
                std::cout << "Alt: removing edge: (" << src->getId() << ", " << dst->getId() << ")" << std::endl;
            }

            addEdge(src, *dst, cycleOk, *graph);
            totalEdge -= 1;
            src->removePossibleAdj(*dst);
        } else {
            if (verbosity > 1) {
                for (auto &d: src->getPossibleAdj()) {
                    std::cout << "Alt: removing edge: (" << src->getId() << ", " << d->getId() << ")" << std::endl;
                }
                std::cout << std::endl;
            }

            addEdge(src, *dst, cycleOk, *graph);
            totalEdge -= src->getAvailNum();
            src->clearPossibleAdj();
        }

        numEdge -= 1;

        if (manualStepping) {
            std::cout << "\nr>";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    while (numEdge > 0) {
        if (verbosity > 1) {
            std::cout << "Number of edge remaining: " << numEdge << std::endl;
        }

        // Randomly choose one from the permutations
        int chosen = Random::get(1, totalEdge);
        int temp = 0;
        GraphNode *src;

        // Add up available edge number of every node till we run over the chosen index
        for (int i = 1; i <= totalNode; i++) {
            src = graph->getNodeAt(i);
            temp += src->getAvailNum();

            // If we run over the chosen index, we are at the correct src
            if (temp >= chosen) break;
        }

        // Get the linked list index within the chosen src
        // The number we obtain is counting from the end *backwards*
        int innerIdx = temp - chosen;

        auto dstIt = src->queryPossibleAdjRev(innerIdx);
        auto dst = *dstIt;

        if (verbosity > 1) {
            std::cout << "Chosen index id: " << chosen
                      << ", corresponding edge: (" << src->getId() << ", " << dst->getId() << ")" << std::endl;
        }

        if (manualStepping) {
            std::cout << "\nr>";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (addEdge(src, *dst, cycleOk, *graph)) {
            // Delete the added edge from the list of all edges
            if (!relaxed && src->getType() == AND) {
                // Delete all edges that starts from this src, as only one can exist
                if (verbosity > 1) {
                    for (auto &d: src->getPossibleAdj()) {
                        std::cout << "removing edge: (" << src->getId() << ", "
                                  << d->getId() << ")" << std::endl;
                    }
                    std::cout << std::endl;
                }
                totalEdge -= src->getAvailNum();
                src->clearPossibleAdj();
            } else {
                if (verbosity > 1) {
                    std::cout << "removing edge: (" << src->getId() << ", " << dst->getId() << ")\n" << std::endl;
                }
                totalEdge -= 1;
                src->erasePossibleAdj(dstIt);
            }
            numEdge -= 1;
        }

        if (manualStepping) {
            std::cout << "\nr>";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (verbosity > 2) {
            std::cout << "Remaining possible edges: \n";
            int index = 0;
            for (int s = 2; s <= totalNode; s++) {
                for (auto &d: graph->getNodeAt(s)->getPossibleAdj())
                    std::cout << "index: " << index << " (" << s << ", " << d->getId() << ") " << std::endl;
                index += 1;
            }
        }

        if (manualStepping) {
            std::cout << "\nr>";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    if (test) {
        std::cout << "Testing if the generated graph has a feasible solution: ";
        auto result = Sat(*graph, std::nullopt);
        if (result != std::nullopt) {
            std::cout << "yes" << std::endl;
        } else {
            std::cout << "no" << std::endl;
        }
    }

    return graph;
}

#endif //ATTACKGRAPHGENERATOR_GENERATION_HPP
