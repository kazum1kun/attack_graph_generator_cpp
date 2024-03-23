#ifndef ATTACKGRAPHGENERATOR_CSVTOGRAPH_HPP
#define ATTACKGRAPHGENERATOR_CSVTOGRAPH_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "AttackGraph.hpp"

std::vector<std::string> split(std::string s, const std::string& delimiter){
    std::vector<std::string> res;

    size_t pos = 0;
    while(pos < s.size()){
        pos = s.find(delimiter);
        res.push_back(s.substr(0,pos));
        s.erase(0,pos+delimiter.length());
    }
    return res;
}


AttackGraph csvToGraph(const std::string& path) {
    // ifstream is used for reading files
    std::ifstream arcFile {path + "/ARCS.txt"};
    std::ifstream vertFile {path + "/VERTICES.txt"};

    // If we couldn't open the input file stream for reading
    if (!arcFile || !vertFile)
    {
        // Print an error and exit
        std::cerr << "Unable to open csv file for reading\n";
        std::exit(1);
    }

    std::string strInput{};
    std::getline(arcFile, strInput);
    int numEdges = std::stoi(strInput);
    std::getline(vertFile, strInput);
    int numVertices = std::stoi(strInput);

    auto graph = AttackGraph(1, numVertices, numEdges);

    // Construct vertices
    while (std::getline(vertFile, strInput)) {
        auto tokens = split(strInput, ",");
        int nodeId = std::stoi(tokens[0]);
        std::string name = tokens[1];
        NodeType type;
        if (tokens[2] == "OR") {
            type = OR;
        } else if (tokens[2] == "AND") {
            type = AND;
        } else if (tokens[2] == "LEAF") {
            type = LEAF;
        } else {
            std::cerr << "Unknown node type \"" << tokens[2] << "\"\n";
            continue;
        }
        double prob = std::stod(tokens[3]);

        const auto node = new GraphNode(nodeId, type, name);
        graph.addNode(*node);
    }

    // Construct edges
    while (std::getline(arcFile, strInput)) {
        auto tokens = split(strInput, ",");
        int dst = std::stoi(tokens[0]);
        int src = std::stoi(tokens[1]);
        double prob = std::stod(tokens[2]);

        graph.addEdgeById(src, dst);

    }

    return graph;

    // When inf goes out of scope, the ifstream
    // destructor will close the file
}

#endif //ATTACKGRAPHGENERATOR_CSVTOGRAPH_HPP
