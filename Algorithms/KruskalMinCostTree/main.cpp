/* This is a small application that illustrates the "greedy" Kruskal algorithm for determining the minimum cost tree.
   The purpose is to build a loop-free tree that connects all graph nodes: n graph nodes connected by n-1 edges.
   Input is the whole graph where every node might be connected to every other node by an edge that has a specific value (cost).
   The output is a list of edges that create the loop-free tree connecting all nodes at minimum cost.
*/

#include <iostream>
#include <fstream>
#include <cassert>

#include "kruskaltree.h"
#include "matrix.h"
#include "utils.h"
#include "matrixutils.h"

using namespace std;

const string c_InFile{"/tmp/kruskalinput.txt"};  // check CPPExercises/InputFileExamples directory
const string c_OutFile{"/tmp/kruskaloutput.txt"};

void writeOutputToFile(ofstream& out, const GraphMatrix& graph, const EdgeList& edgeList);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    if(in.is_open() && out.is_open())
    {
        GraphMatrix graphMatrix;
        in >> graphMatrix;

        const size_t c_NodesCount{static_cast<size_t>(graphMatrix.getNrOfRows())};

        if (c_NodesCount > 0u)
        {
            KruskalTree kruskalTree;
            bool success{kruskalTree.build(graphMatrix)};

            if (success)
            {
                writeOutputToFile(out, graphMatrix, kruskalTree.getTreeEdges());

                if (c_NodesCount - 1 == kruskalTree.getTreeEdges().size())
                {
                    cout << "Kruskal minimum cost tree successfully written to: " << endl << endl << c_OutFile << endl << endl;
                }
                else
                {
                    cout << "The Kruskal minimum cost tree has been incompletely built (insufficient edges provided)" << endl << endl;
                    cout << "Please check input file: " << endl << endl << c_InFile << endl << endl;
                }
            }
            else
            {
                cerr << "Invalid input provided" << endl << endl << "Please check input file: " << endl << endl << c_InFile << endl << endl;
            }
        }
        else
        {
            cerr << "Error! The graph contains no nodes" << endl << endl;
        }
    }
    else
    {
        cerr << "Error in opening input and/or output file" << endl << endl;
    }

    return 0;
}

void writeOutputToFile(ofstream& out, const GraphMatrix& graph, const EdgeList& edgeList)
{
    assert(out.is_open());

    Cost totalCost{0};

    if (edgeList.size() > 0u)
    {
        out << "The minimum cost Kruskal tree edges are: " << endl << endl;

        for (EdgeList::const_iterator it{edgeList.cbegin()}; it != edgeList.cend(); ++it)
        {
            // it's safe to convert the indexes to int as long as the same matrix passed to the KruskalTree is passed to the current function
            Cost cost{graph.at(static_cast<int>(it->first), static_cast<int>(it->second))};
            totalCost += cost;

            out << "(" << (it->first + 1) << "," << (it->second + 1) << ") - cost: " << cost << endl;
        }

        out << endl << "Total cost: " << totalCost << endl;
    }
    else
    {
        out << "No edges could be found" << endl;
    }
}
