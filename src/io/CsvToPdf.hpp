#ifndef ATTACKGRAPHGENERATOR_CSVTOPDF_HPP
#define ATTACKGRAPHGENERATOR_CSVTOPDF_HPP

#include <string>
#include <filesystem>
#include <fstream>

void csvToPdf(std::string file, std::string arcSed, std::string vertSed) {
    auto basePath = std::filesystem::path(file).parent_path();
    std::fstream graphDotFile("AttackGraph.dot", std::ios::out | std::ios::trunc);

    graphDotFile << "digraph G {\n";

    // Convert vertices


}

#endif //ATTACKGRAPHGENERATOR_CSVTOPDF_HPP
