/* This is a small application that illustrates the "greedy" Kruskal algorithm for determining the minimum/maximum cost tree.
   The purpose is to build a loop-free tree that connects all graph nodes: n graph nodes connected by n-1 edges.
   Input is the whole graph where every node might be connected to every other node by an edge that has a specific value (cost).
   The output is a list of edges that create the loop-free tree connecting all nodes at minimum cost.
*/

#include "kruskal.h"
#include "utils.h"

using namespace std;

static const string c_InFile{Utilities::c_InputOutputDir + "kruskalpriminput.txt"};
static const string c_OutFile{Utilities::c_InputOutputDir + "kruskaloutput.txt"};

extern int treeAppMain(const std::string& inputFile, const std::string& outputFile, BaseEngine& treeEngine);

int main()
{
    KruskalEngine kruskalEngine;
    return treeAppMain(c_InFile, c_OutFile, kruskalEngine);
}
