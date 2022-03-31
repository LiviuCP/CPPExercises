/* This is a small application that illustrates the "greedy" Kruskal algorithm for determining the minimum cost tree.
   The purpose is to build a loop-free tree that connects all graph nodes: n graph nodes connected by n-1 edges.
   Input is the whole graph where every node might be connected to every other node by an edge that has a specific value (cost).
   The output is a list of edges that create the loop-free tree connecting all nodes at minimum cost.
*/

#include <iostream>
#include <fstream>

#include "kruskalgraph.h"
#include "utils.h"
#include "matrixutils.h"

using namespace std;

static const string c_InFile{Utilities::c_InputOutputDir + "kruskalinput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "kruskaloutput.txt"};

void writeTreeToFile(ofstream& out, const KruskalGraph::GraphMatrix& graph, const KruskalGraph::Tree& tree);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    if(in.is_open() && out.is_open())
    {
        KruskalGraph::GraphMatrix graphMatrix;
        in >> graphMatrix;

        const size_t c_NodesCount{static_cast<size_t>(graphMatrix.getNrOfRows())};

        if (c_NodesCount > 0u)
        {
            KruskalGraph kruskalGraph;
            bool success{kruskalGraph.build(graphMatrix)};

            if (success)
            {
                out << "The MINIMUM cost Kruskal tree edges are: " << endl << endl;

                writeTreeToFile(out, graphMatrix, kruskalGraph.getMinTreeEdges());

                out << "========================================" << endl << endl;
                out << "The MAXIMUM cost Kruskal tree edges are: " << endl << endl;

                writeTreeToFile(out, graphMatrix, kruskalGraph.getMaxTreeEdges());

                // no need to check the count of the maximum tree (if any nodes are loose then both trees will be incomplete)
                if (c_NodesCount - 1 == kruskalGraph.getMinTreeEdges().size())
                {
                    cout << "Kruskal minimum and maximum cost trees successfully written to: " << endl << endl << c_OutFile << endl << endl;
                }
                else
                {
                    cout << "The Kruskal minimum and maximum cost trees have been incompletely built (insufficient edges provided)" << endl << endl;
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

void writeTreeToFile(ofstream& out, const KruskalGraph::GraphMatrix& graph, const KruskalGraph::Tree& tree)
{
    assert(out.is_open());

    KruskalGraph::Cost totalCost{0};

    if (tree.size() > 0u)
    {
        for (KruskalGraph::Tree::const_iterator it{tree.cbegin()}; it != tree.cend(); ++it)
        {
            // it's safe to convert the indexes to int as long as the same matrix passed to the KruskalGraph is passed to the current function
            KruskalGraph::Cost cost{graph.at(static_cast<int>(it->first), static_cast<int>(it->second))};
            totalCost += cost;

            out << "(" << (it->first + 1) << "," << (it->second + 1) << ") - cost: " << cost << endl;
        }

        out << endl << "Total cost: " << totalCost << endl << endl;
    }
    else
    {
        out << "No edges could be found" << endl;
    }
}
