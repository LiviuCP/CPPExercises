#include <iostream>

#include "csvparsingqueue.h"
#include "csvaggregator.h"
#include "apputils.h"

static constexpr size_t c_MaxCSVFilesCount{5};

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::filesystem::directory_entry inputDir{argv[1]};

        if (inputDir.is_directory())
        {
            std::cout << "Input directory path is: " << std::filesystem::canonical(inputDir.path()).string() << "\n";
            const std::vector<std::filesystem::path> c_CSVFiles{Utils::retrieveFilePathsBySuffix(inputDir, ".csv")};

            if (c_CSVFiles.empty())
            {
                std::cerr << "No CSV files found in input directory!\n";
            }
            else if (const size_t c_CSVFilesCount{c_CSVFiles.size()}; c_CSVFilesCount > c_MaxCSVFilesCount)
            {
                std::cerr << "\nThe actual count of CSV files (" << c_CSVFilesCount << ") exceeds the maximum allowed (" << c_MaxCSVFilesCount << ")!\n";
                std::cerr << "Please remove part of the files and try again.\n";
            }
            else
            {
                Utils::removeErrorFiles(inputDir);
                const std::filesystem::path c_OutFilePath{Utils::retrieveOutputFilePath(inputDir)};

                CSVAggregator csvAggregator{c_OutFilePath.string()};

                std::cout << "\n" << c_CSVFilesCount << " CSV files have been found. Starting work...\n";

                CSVParsingQueue parsingQueue{csvAggregator, 4};

                const bool c_IsParsingActive{parsingQueue.parseCSVFiles(c_CSVFiles)};

                if (c_IsParsingActive)
                {
                    parsingQueue.finishParsingAndStop();

                    const bool c_Success{csvAggregator.writeDataToOutputCSV()};

                    if (c_Success)
                    {
                        std::cout << "\nDone!\n";
                        std::cout << "Please check: " << c_OutFilePath.string() << "\n";

                        const std::vector<std::filesystem::path> c_ErrorFiles{Utils::retrieveFilePathsBySuffix(inputDir, "_error.txt")};

                        if (!c_ErrorFiles.empty())
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
