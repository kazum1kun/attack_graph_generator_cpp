#ifndef ATTACKGRAPHGENERATOR_CSVTOPDF_HPP
#define ATTACKGRAPHGENERATOR_CSVTOPDF_HPP

#include <string>
#include <filesystem>
#include <fstream>


std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result;
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != nullptr)
            result += buffer;
    }
    return result;
}

void csvToPdf(const std::string& path, const std::string& arcSed, const std::string& vertSed) {
    std::fstream graphDotFile(path + "/AttackGraph.dot", std::ios::out | std::ios::trunc);

    graphDotFile << "digraph G {\n";

    // Convert vertices
    auto vertResults = exec(("sed -f " + vertSed + " " + path + "/VERTICES.CSV").c_str());
    graphDotFile << vertResults;

    // Convert arcs
    auto arcResults = exec(("sed -f " + arcSed + " " + path + "/ARCS.CSV").c_str());
    graphDotFile << arcResults;\

    graphDotFile << "}\n";
    graphDotFile.close();

    // Convert to pdf
    auto epsContent = exec(("dot -Tps " + path + "/AttackGraph.dot").c_str());
    std::fstream epsFile(path + "/AttackGraph.eps", std::ios::out | std::ios::trunc);
    epsFile << epsContent;
    epsFile.close();
    exec(("epstopdf " + path + "/AttackGraph.eps").c_str());
}

#endif //ATTACKGRAPHGENERATOR_CSVTOPDF_HPP
