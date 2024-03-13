#ifndef ATTACKGRAPHGENERATOR_FSUTILS_HPP
#define ATTACKGRAPHGENERATOR_FSUTILS_HPP

#include <filesystem>

bool CreateDirectoryRecursive(std::string const & dirName, std::error_code & err)
{
    err.clear();
    if (!std::filesystem::create_directories(dirName, err))
    {
        if (std::filesystem::exists(dirName))
        {
            // The folder already exists:
            err.clear();
            return true;
        }
        return false;
    }
    return true;
}

#endif //ATTACKGRAPHGENERATOR_FSUTILS_HPP
