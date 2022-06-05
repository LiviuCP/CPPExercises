/* This is a small application that illustrates the "greedy" Prim algorithm for determining the minimum/maximum cost tree.
   The purpose is to build a loop-free tree that connects all graph nodes: n graph nodes connected by n-1 edges.
   Input is the whole graph where every node might be connected to every other node by an edge that has a specific value (cost).
   The output is a list of edges that create the loop-free tree connecting all nodes at minimum cost.
*/

#include <iostream>
#include <fstream>

#include "primgraph.h"
#include "utils.h"
#include "matrixutils.h"

using namespace std;

static const string c_InFile{Utilities::c_InputOutputDir + "kruskalpriminput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "primoutput.txt"};

void writeTreeToFile(ofstream& out, const PrimGraph::GraphMatrix& graph, const PrimGraph::Tree& tree);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    if(in.is_open() && out.is_open())
    {
        PrimGraph::GraphMatrix graphMatrix;
        in >> graphMatrix;

        const size_t c_NodesCount{static_cast<size_t>(graphMatrix.getNrOfRows())};

        if (c_NodesCount > 0u)
        {
            PrimGraph primGraph;
            bool success{primGraph.build(graphMatrix)};

            if (success)
            {
                out << "The MINIMUM cost Prim tree edges are: " << endl << endl;

                writeTreeToFile(out, graphMatrix, primGraph.getMinTree());

                out << "========================================" << endl << endl;
                out << "The MAXIMUM cost Prim tree edges are: " << endl << endl;

                writeTreeToFile(out, graphMatrix, primGraph.getMaxTree());

                // no need to check the count of the maximum tree (if any nodes are loose then both trees will be incomplete)
                if (c_NodesCount - 1 == primGraph.getMinTree().size())
                {
                    cout << "Prim minimum and maximum cost trees successfully written to: " << endl << endl << c_OutFile << endl << endl;
                }
                else
                {
                    cout << "The Prim minimum and maximum cost trees have been incompletely built (insufficient edges provided)" << endl << endl;
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

void writeTreeToFile(ofstream& out, const PrimGraph::GraphMatrix& graph, const PrimGraph::Tree& tree)
{
    assert(out.is_open());

    PrimGraph::Cost totalCost{0};

    if (tree.size() > 0u)
    {
        for (PrimGraph::Tree::const_iterator it{tree.cbegin()}; it != tree.cend(); ++it)
        {
            // it's safe to convert the indexes to int as long as the same matrix passed to the PrimGraph is passed to the current function
            PrimGraph::Cost cost{graph.at(static_cast<int>(it->first), static_cast<int>(it->second))};
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
