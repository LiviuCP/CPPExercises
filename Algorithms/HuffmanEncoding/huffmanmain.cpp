#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

import huffmanencoder;
import matrixutils;
import utils;

static const std::string c_InFile{Utilities::c_InputOutputDir + "huffmaninput.txt"};
static const std::string c_OutFile{Utilities::c_InputOutputDir + "huffmanoutput.txt"};

void writeToFile(std::ofstream& out, const EncodingOutput& encodingOutput, double efficiencyPercentage);

int main()
{
    std::ifstream in{c_InFile};
    std::ofstream out{c_OutFile};

    Utilities::clearScreen();

    if (in.is_open() && out.is_open())
    {
        EncodingInput encodingInput;
        in >> encodingInput;

        HuffmanEncoder encoder;
        bool success{encoder.encode(encodingInput)};

        if (success)
        {
            const EncodingOutput c_EncodingOutput{encoder.getEncodingResult()};
            const size_t c_EncodedCharsCount{c_EncodingOutput.size()};

            // the efficiency is provided as a double number (typically between -1 and 1) by the encoder and should be
            // converted to percentage
            const double c_EncodingEfficiencyPercentage{100.0 * encoder.getEncodingEfficiency()};

            writeToFile(out, c_EncodingOutput, c_EncodingEfficiencyPercentage);
            std::cout << c_EncodedCharsCount << " characters encoded by using the Huffman algorithm" << std::endl
                      << std::endl;
            std::cout << "Please check output file: " << std::endl << std::endl << c_OutFile << std::endl << std::endl;
        }
        else
        {
            std::cerr << "Invalid input provided" << std::endl
                      << std::endl
                      << "Please check input file: " << std::endl
                      << std::endl
                      << c_InFile << std::endl
                      << std::endl;
        }
    }
    else
    {
        std::cerr << "Error in opening input and/or output file" << std::endl << std::endl;
    }

    return 0;
}

void writeToFile(std::ofstream& out, const EncodingOutput& encodingOutput, double efficiencyPercentage)
{
    const size_t c_EncodedCharsCount{encodingOutput.size()};

    if (c_EncodedCharsCount > 0u)
    {
        out << "The resulting Huffman encoding is: " << std::endl << std::endl;

        for (EncodingOutput::const_iterator it{encodingOutput.cbegin()}; it != encodingOutput.cend(); ++it)
        {
            out << it->first << " : " << it->second << std::endl;
        }

        out << std::endl << "Number of encoded characters: " << c_EncodedCharsCount << std::endl;
        out << std::endl << "Encoding efficiency: " << std::setprecision(2) << efficiencyPercentage << "%" << std::endl;

        if (efficiencyPercentage <= 0.0)
        {
            out << std::endl << "The algorithm is inefficient for the provided character occurrences!" << std::endl;
        }
    }
}
