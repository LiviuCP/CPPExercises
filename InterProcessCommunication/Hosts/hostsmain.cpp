#include <iostream>

#include "csvparsingqueue.h"
#include "csvaggregator.h"
#include "apputils.h"

static constexpr size_t c_MaxCSVFilesCount{5};

void removeErrorFiles(const std::filesystem::directory_entry& inputDir)
{
    const std::vector<std::filesystem::path> c_ErrorFilesToDelete{Utils::retrieveErrorFilePaths(inputDir)};

    // cleanup existing error files to ensure the output is consistent in case input csv files have been modified
    for (const auto& path : c_ErrorFilesToDelete)
    {
        std::filesystem::remove(path);
    }
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::filesystem::directory_entry inputDir{argv[1]};

        if (inputDir.is_directory())
        {
            std::cout << "Input directory path is: " << std::filesystem::canonical(inputDir.path()).string() << "\n";
            const std::vector<std::filesystem::path> c_InFilePaths{Utils::retrieveInputFilePaths(inputDir)};

            if (c_InFilePaths.empty())
            {
                std::cerr << "No CSV files found in input directory!\n";
            }
            else if (const size_t c_CSVFilesCount{c_InFilePaths.size()}; c_CSVFilesCount > c_MaxCSVFilesCount)
            {
                std::cerr << "\nThe actual count of CSV files (" << c_CSVFilesCount << ") exceeds the maximum allowed (" << c_MaxCSVFilesCount << ")!\n";
                std::cerr << "Please remove part of the files and try again.\n";
            }
            else
            {
                removeErrorFiles(inputDir);

                const std::filesystem::path c_OutFilePath{Utils::computeOutputFilePath(inputDir)};
                CSVAggregator csvAggregator{c_OutFilePath};

                std::cout << "\n" << c_CSVFilesCount << " CSV files have been found. Starting work...\n";

                CSVParsingQueue parsingQueue{csvAggregator, 4};

                const bool c_IsParsingActive{parsingQueue.parseCSVFiles(c_InFilePaths)};

                if (c_IsParsingActive)
                {
                    parsingQueue.finishParsingAndStop();

                    const bool c_Success{csvAggregator.writeDataToOutputCSV()};

                    if (c_Success)
                    {
                        std::cout << "\nDone!\n";
                        std::cout << "Please check: " << c_OutFilePath.string() << "\n";

                        if (Utils::errorFilesExist(inputDir))
                        {
                            std::cerr << "\nInvalid entries have been detected. Please check the error files from the input directory\n";
                        }
                    }
                    else
                    {
                        std::cerr << "An error occurred when writing to output file: " << c_OutFilePath.string() << "\n";
                    }
                }
            }
        }
        else
        {
            std::cerr << "Invalid input directory path: " << argv[1] << "\n";
        }
    }
    else
    {
        std::cout << "No path entered for input directory!";
    }

    return 0;
}
