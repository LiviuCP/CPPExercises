#pragma once

#include <list>
#include <utility>

#include "matrix.h"

using Node = size_t;
using Edge = std::pair<Node, Node>;
using Cost = size_t;
using GraphMatrix = Matrix<Cost>;
using Tree = std::list<Edge>;
