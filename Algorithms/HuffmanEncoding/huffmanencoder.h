#pragma once

#include <set>
#include <map>
#include <string>
#include <vector>

#include "matrix.h"

using EncodingInput = Matrix<std::string>;
using EncodingOutput = std::map<char, std::string>;

class HuffmanEncoder
{
public:
    HuffmanEncoder();

    bool encode(const EncodingInput& encodingInput);
    EncodingOutput getEncodingResult() const;
    double getEncodingEfficiency() const;

private:
    struct Node
    {
        Node();

        char mCharacter;
        ssize_t mOccurrence;

        Node* mpLeftChild;
        Node* mpRightChild;
    };

    using CharOccurrenceMap = std::multimap<int, char>;
    using TreeContainer = std::vector<Node>;
    using CharSet = std::set<char>;

    bool _buildOccurrenceMap(const EncodingInput& encodingInput);
    void _buildTree();
    void _retrieveEncodingFromTree();

    /* Encoding efficiency is computed as reduction percentage of the total bit count when using Huffman encoding
       comparing to using fixed-size bit encoding with a minimum required bit count per symbol (character)
       For example if 6 symbols are encoded then minimum 3 bits are required per character (fixed-size encoding)
    */
    void _computeEncodingEfficiency();

    void _reset();

    // '0' / '1' chars used instead of bits for simplicity
    static constexpr char scBinaryDigit0{'0'};
    static constexpr char scBinaryDigit1{'1'};

    static constexpr char scNullCharacter{'\0'};
    static constexpr ssize_t scMinRequiredCharsCount{2};

    CharOccurrenceMap mOccurrenceMap;

    Node* mpRoot;
    TreeContainer mTreeContainer;

    EncodingOutput mEncodingResult;
    double mEncodingEfficiency;
};
