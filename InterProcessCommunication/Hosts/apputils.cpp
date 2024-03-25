#include <algorithm>
#include <cctype>
#include <cassert>
#include <regex>

#include "apputils.h"

static const char* c_ErrorFileSuffix{"_error.txt"};
static const char* c_OutputCSVFileName{"output.csv"};

void Utils::removeErrorFiles(const std::filesystem::directory_entry& inputDir)
{
    const std::vector<std::filesystem::path> c_ErrorFilesToDelete{Utils::retrieveFilePathsBySuffix(inputDir, c_ErrorFileSuffix)};

    // cleanup existing error files to ensure the output is consistent in case input csv files have been modified
    for (const auto& path : c_ErrorFilesToDelete)
    {
        std::filesystem::remove(path);
    }
}

std::filesystem::path Utils::retrieveOutputFilePath(const std::filesystem::directory_entry& inputDir)
{
    std::filesystem::path outFilePath{std::filesystem::canonical(inputDir.path())};
    outFilePath = outFilePath.parent_path();
    outFilePath /= c_OutputCSVFileName;

    return outFilePath;
}

std::vector<std::filesystem::path> Utils::retrieveFilePathsBySuffix(const std::filesystem::directory_entry& dir, const std::string& suffix)
{
    assert(dir.is_directory() && !suffix.empty());
    std::vector<std::filesystem::path> result;

    for (const auto& entry : std::filesystem::directory_iterator{dir.path()})
    {
        if (entry.is_regular_file())
        {
            std::string filePathStr{entry.path().string()};
            std::transform(filePathStr.cbegin(), filePathStr.cend(), filePathStr.begin(), [](unsigned char c){return std::tolower(c);});

            if (filePathStr.ends_with(suffix))
            {
                result.push_back(entry.path());
            }
        }
    }

    return result;
}

bool Utils::isCsvFilePath(const std::filesystem::path& filePath)
{
    const std::filesystem::path c_FileNameExtension{filePath.extension()};
    std::regex csvExtensionRe{"\\.[c|C][s|S][v|V]"};

    return std::regex_match(c_FileNameExtension.string(), csvExtensionRe);
}
