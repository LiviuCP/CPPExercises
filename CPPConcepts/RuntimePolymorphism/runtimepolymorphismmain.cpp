/* This small application illustrates the runtime polymorphism concept presented by Sean Parent at a conference
   It is based on his example application
*/

#include <iostream>

#include "containers.h"

import datatypes;
import utils;

static const std::string c_OutFile{Utilities::c_InputOutputDir + "runtimepolymorphismoutput.txt"};

int main()
{
    std::ofstream out{c_OutFile};

    if (out.is_open())
    {
        Box box;
        Bag bag;

        box.emplace_back(Color::ORANGE);
        box.emplace_back(Thermometer{37.5});
        box.emplace_back(ColorPair{Color::YELLOW, Color::MAGENTA});
        box.emplace_back(box); // copy of the box and it's CURRENT content
        box.emplace_back(1.5);
        bag.emplace(std::make_pair("fruit", "banana"));
        bag.emplace(std::make_pair("muesli", FoodMix{"cereal", "dried fruit"}));
        bag.emplace(std::make_pair("color", Color::BROWN));
        bag.emplace(std::make_pair(
            "6 colors board\n",
            ColorBoard{2, 3, {Color::BLUE, Color::RED, Color::MAGENTA, Color::BROWN, Color::LILA, Color::YELLOW}}));
        bag.emplace(std::make_pair("number plate", 52445));
        bag.emplace(std::make_pair("plastic", 2.0));
        box.emplace_back(bag);

        display(box, out, 0);

        std::cout << "Objects displayed in: " << c_OutFile << std::endl;
        std::cout << "Total number of objects is: " << countContent(box) << std::endl;
    }
    else
    {
        std::cerr << "Error in opening output file: " << c_OutFile << std::endl << std::endl;
    }

    return 0;
}
