#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <vector>
#include <map>

#include "datawrapper.h"

using Box = std::vector<DataWrapper>;

void display(const Box& box, std::ofstream& out, size_t indentation);
size_t countContent(const Box& box);

using Bag = std::map<std::string, DataWrapper>;

void display(const Bag& bag, std::ofstream& out, size_t indentation);
size_t countContent(const Bag& bag);

#endif // CONTAINERS_H
