#pragma once

#include <filesystem>
#include <vector>

namespace Utils {
    void removeErrorFiles(const std::filesystem::directory_entry& inputDir);
    std::filesystem::path retrieveOutputFilePath(const std::filesystem::directory_entry& inputDir);
    std::vector<std::filesystem::path> retrieveFilePathsBySuffix(const std::filesystem::directory_entry& dir, const std::string& suffix);
    bool isCsvFilePath(const std::filesystem::path& filePath);
}
