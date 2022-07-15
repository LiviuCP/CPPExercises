#include "containers.h"

void display(const Box& box, std::ofstream &out, size_t indentation)
{
    if (out.is_open())
    {
        out << std::string(indentation, ' ') << "<box>" << std::endl;

        for (const auto& obj : box)
        {
            displayData(obj, out, indentation + 2);
        }

        out << std::string(indentation, ' ') << "</box>" << std::endl;
    }
}

size_t countContent(const Box& box)
{
    size_t elementsCount{1};

    for (const auto& item : box)
    {
        elementsCount += countDataContent(item);
    }

    return elementsCount;
}

void display(const Bag& bag, std::ofstream& out, size_t indentation)
{
    if (out.is_open())
    {
        out << std::string(indentation, ' ') << "<bag>" << std::endl;

        for (const auto& [label, item] : bag)
        {
            std::string currentLabel{label};
            std::string separator{": "};
            size_t objectIndentation{0};

            out << std::string(indentation + 2, ' ');

            // this implementation is pretty rudimentary but it is just for the sake of demonstration/exercise
            if (!label.empty())
            {
                size_t lastCharPos{label.size() - 1};

                if ('\n' == label[lastCharPos])
                {
                    objectIndentation = indentation + 4;
                    currentLabel.replace(lastCharPos, 1, "");
                    separator.append("\n");
                }
            }

            out << currentLabel << separator;

            displayData(item, out, objectIndentation);
        }

        out << std::string(indentation, ' ') << "</bag>" << std::endl;
    }
    else
    {
        assert(false);
    }
}

size_t countContent(const Bag& bag)
{
    size_t elementsCount{1};

    for (auto it{bag.cbegin()}; it != bag.cend(); ++it)
    {
        elementsCount += countDataContent(it->second);
    }

    return elementsCount;
}
