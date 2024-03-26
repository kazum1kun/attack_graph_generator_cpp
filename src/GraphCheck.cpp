#include <iostream>
#include <filesystem>
#include "CsvToGraph.hpp"
#include "ShortestTrace.hpp"
#include <string>
#include <set>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Please provide a directory where graphs are." << std::endl;
        std::exit(1);
    }

    fs::path dirPath = argv[1];
    if (!fs::is_directory(dirPath)) {
        std::cout << argv[1] << " does not exist, please check your input." << std::endl;
        std::exit(1);
    }

    for (auto &entry: dirPath) {
        if (fs::is_directory(entry)) {
            // Check for ARCS.CSV and VERTICES.CSV
            auto arcPath = entry / "ARCS.CSV";
            auto vertPath = entry / "VERTICES.CSV";

            if (!(fs::exists(arcPath) && fs::exists(vertPath))) continue;

            auto graph = csvToGraph(arcPath, vertPath);

            int numOr = 0, numPf = 0, numAnd = 0;
            for (auto &node: graph.getNodes()) {
                if (node == nullptr) continue;
                if (node->getType() == OR) numOr += 1;
                else if (node->getType() == LEAF) numPf += 1;
                else if (node->getType() == AND) numAnd += 1;
            }

            std::string hasCycle = cycleExists(graph) ? "✓ (yes)" : "✗ (no)";
            std::string hasTrace = (sat(graph) == std::nullopt) ? "✗ (no)" : "✓ (yes)";

            std::cout << "==========" << entry.filename() << "==========\n";
            std::cout << "Total number of nodes: " << graph.getSize() << std::endl;
            std::cout << "├──OR nodes: " << numOr << std::endl;
            std::cout << "├──PF nodes: " << numPf << std::endl;
            std::cout << "└──AND nodes: " << numAnd << std::endl;
            std::cout << "Number of edges: " << graph.getNumEdges() << std::endl;
            std::cout << "Has cycle: " << hasCycle << std::endl;
            std::cout << "Has an attack trace: " << hasTrace << std::endl;
        }
    }
}