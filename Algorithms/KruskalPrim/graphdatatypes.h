#ifndef GRAPHDATATYPES_H
#define GRAPHDATATYPES_H

#include <list>
#include <utility>

#include "matrix.h"

using Node = size_t;
using Edge = std::pair<Node, Node>;
using Cost = int;
using GraphMatrix = Matrix<Cost>;
using Tree = std::list<Edge>;

#endif // GRAPHDATATYPES_H
