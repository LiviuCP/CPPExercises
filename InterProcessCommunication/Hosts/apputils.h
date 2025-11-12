#pragma once

#include <filesystem>
#include <vector>

namespace Utils
{
std::vector<std::filesystem::path> retrieveInputFilePaths(const std::filesystem::directory_entry& inputDir);
std::filesystem::path computeOutputFilePath(const std::filesystem::directory_entry& inputDir);
std::vector<std::filesystem::path> retrieveErrorFilePaths(const std::filesystem::directory_entry& inputDir);
bool isCsvFilePath(const std::filesystem::path& filePath);
bool errorFilesExist(const std::filesystem::directory_entry& inputDir);
} // namespace Utils
