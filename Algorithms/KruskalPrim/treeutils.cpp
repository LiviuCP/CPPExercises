#include <iostream>

#include "baseengine.h"
#include "utils.h"
#include "matrixutils.h"
#include "datautils.h"

static void writeTreeToFile(std::ofstream& out, const GraphMatrix& graphMatrix, const Tree& tree)
{
    if (out.is_open())
    {
        Cost totalCost{0};

        if (!tree.empty())
        {
            for (const auto& edge : tree)
            {
                const Cost currentEdgeCost{graphMatrix.at(edge.first, edge.second)};
                totalCost += currentEdgeCost;

                out << "(" << (edge.first + 1) << "," << (edge.second + 1) << ") - cost: " << currentEdgeCost << "\n";
            }

            out << "\nTotal cost: " << totalCost << "\n\n";
        }
        else
        {
            out << "No edges could be found\n";
        }
    }
    else
    {
        assert(false);
    }
}

int treeAppMain(const std::string& inputFile, const std::string& outputFile, BaseEngine& treeEngine)
{
    const std::string c_AlgorithmName{treeEngine.getName()};

    std::ifstream in{inputFile};
    std::ofstream out{outputFile};

    Utilities::clearScreen();

    if(in.is_open() && out.is_open())
    {
        GraphMatrix graphMatrix;
        in >> graphMatrix;

        if (!graphMatrix.isEmpty())
        {
            bool success{treeEngine.buildTrees(graphMatrix)};

            if (success)
            {
                out << "The " << c_AlgorithmName << " MINIMUM cost tree edges are: \n\n";

                writeTreeToFile(out, graphMatrix, treeEngine.getMinTree());

                out << "========================================\n\n";
                out << "The " << c_AlgorithmName << " MAXIMUM cost tree edges are: \n\n";

                writeTreeToFile(out, graphMatrix, treeEngine.getMaxTree());

                const matrix_size_t c_NodesCount{graphMatrix.getNrOfRows()};
                const matrix_size_t c_RequiredEdgesCount{c_NodesCount - 1};

                // no need to check the count of the maximum tree (if any nodes are loose then both trees will be incomplete)
                if (c_RequiredEdgesCount == treeEngine.getMinTree().size())
                {
                    std::cout << c_AlgorithmName << " minimum and maximum cost trees successfully written to: \n\n" << outputFile << "\n\n";
                }
                else
                {
                    std::cout << "The " << c_AlgorithmName << " minimum and maximum cost trees have been incompletely built (insufficient edges provided)\n\n";
                    std::cout << "Please check input file: \n\n" << inputFile << "\n\n";
                }
            }
            else
            {
                std::cerr << "Invalid input provided\n\nPlease check input file: \n\n" << inputFile << "\n\n";
            }
        }
        else
        {
            std::cerr << "Error! The graph contains no nodes\n\n";
        }
    }
    else
    {
        std::cerr << "Error in opening input and/or output file\n\n";
    }

    return 0;
}

