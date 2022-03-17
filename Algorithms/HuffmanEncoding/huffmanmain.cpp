#include <iostream>
#include <fstream>
#include <iomanip>

#include "huffmanencoder.h"
#include "matrixutils.h"

using namespace std;

const string c_InFile{"/tmp/huffmaninput.txt"};  // check CPPExercises/InputFileExamples directory
const string c_OutFile{"/tmp/huffmanoutput.txt"};

void writeToFile(ofstream& out, const EncodingOutput& encodingOutput, double efficiencyPercentage);

int main()
{
    ifstream in{c_InFile};
    ofstream out{c_OutFile};

    Utilities::clearScreen();

    if(in.is_open() && out.is_open())
    {
        EncodingInput encodingInput;
        in >> encodingInput;

        HuffmanEncoder encoder;
        bool success{encoder.encode(encodingInput)};

        if (success)
        {
            const EncodingOutput c_EncodingOutput{encoder.getEncodingResult()};
            const size_t c_EncodedCharsCount{c_EncodingOutput.size()};

            // the efficiency is provided as a double number (typically between -1 and 1) by the encoder and should be converted to percentage
            const double c_EncodingEfficiencyPercentage{100.0 * encoder.getEncodingEfficiency()};

            writeToFile(out, c_EncodingOutput, c_EncodingEfficiencyPercentage);
            cout << c_EncodedCharsCount << " characters encoded by using the Huffman algorithm" << endl << endl;
            cout << "Please check output file: " << endl << endl << c_OutFile << endl << endl;
        }
        else
        {
            cerr << "Invalid input provided" << endl << endl << "Please check input file: " << endl << endl << c_InFile << endl << endl;
        }
    }
    else
    {
        cerr << "Error in opening input and/or output file" << endl << endl;
    }

    return 0;
}

void writeToFile(ofstream& out, const EncodingOutput& encodingOutput, double efficiencyPercentage)
{
    const size_t c_EncodedCharsCount{encodingOutput.size()};

    if (c_EncodedCharsCount > 0u)
    {
        out << "The resulting Huffman encoding is: " << endl << endl;

        for (EncodingOutput::const_iterator it{encodingOutput.cbegin()}; it != encodingOutput.cend(); ++it)
        {
            out << it->first << " : " << it->second << endl;
        }

        out << endl << "Number of encoded characters: " << c_EncodedCharsCount << endl;
        out << endl << "Encoding efficiency: " << setprecision(2) << efficiencyPercentage << "%" << endl;

        if (efficiencyPercentage <= 0.0)
        {
            out << endl << "The algorithm is inefficient for the provided character occurrences!" << endl;
        }
    }
}
