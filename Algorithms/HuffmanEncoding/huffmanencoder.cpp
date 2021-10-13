#include <sstream>
#include <cassert>

#include "huffmanencoder.h"

HuffmanEncoder::HuffmanEncoder()
    : mpRoot{nullptr}
    , mEncodingEfficiency{0.0}
{
}

bool HuffmanEncoder::encode(const EncodingInput& encodingInput)
{
    _reset();

    const bool c_IsValidInput{_buildOccurrenceMap(encodingInput)};

    if (c_IsValidInput)
    {
        _buildTree();
        _retrieveEncodingFromTree();
        _computeEncodingEfficiency();
    }

    return c_IsValidInput;
}

EncodingOutput HuffmanEncoder::getEncodingResult() const
{
    return mEncodingResult;
}

double HuffmanEncoder::getEncodingEfficiency() const
{
    return mEncodingEfficiency;
}

bool HuffmanEncoder::_buildOccurrenceMap(const EncodingInput& encodingInput)
{
    bool isDataValid{true};

    CharOccurrenceMap huffmanOccurrenceMap;
    const ssize_t c_CharOccurrencePairSize{2};

    if (encodingInput.getNrOfRows() >= scMinRequiredCharsCount && c_CharOccurrencePairSize == encodingInput.getNrOfColumns())
    {
        CharSet foundCharacters;

        for (EncodingInput::ConstNIterator it{encodingInput.constNBegin()}; it != encodingInput.constNColumnEnd(0); ++it)
        {
            bool isValidTuple{false};
            std::string currentSymbol{*it};
            char character{scNullCharacter};
            ssize_t occurrence{0};

            // there should be exactly one character on each row in the first column
            if (1u == currentSymbol.size())
            {
                character = currentSymbol[0];

                if (foundCharacters.find(character) == foundCharacters.cend())
                {
                    std::stringstream stream;
                    stream << encodingInput.at(it.getRowNr(), it.getColumnNr() + 1);
                    stream >> occurrence;

                    if (!stream.fail() && occurrence > 0)
                    {
                        huffmanOccurrenceMap.insert(std::pair<size_t, char>{occurrence, character});
                        foundCharacters.insert(character);
                        isValidTuple = true;
                    }
                }
            }

            if (!isValidTuple)
            {
                isDataValid = false;
                break;
            }
        }
    }
    else
    {
        isDataValid = false;
    }

    if (isDataValid)
    {
        mHuffmanOccurrenceMap = std::move(huffmanOccurrenceMap);
    }

    return isDataValid;
}

/* The Huffman encoding is based a binary tree that be built according to following rules:
    - each node has 0 or 2 children (the tree is complete)
    - there are 2 types of nodes:
        - leaf nodes (encoded characters)
        - binding nodes
    - each node contains:
        - a character (symbol) for leaf nodes or a character placeholder for binding nodes (used '\0')
        - a value: parent has the sum of the values of the children; for leaf nodes the value is the occurrence of the character
    - left child has a lower value than the right child
    - the edge from each parent node to its left child is a "0" while the edge to the right child is a "1"; the edge path from the root to each leaf determines the encoding of each character

   As can be seen the tree is different from a binary search tree (BST).
   Also it is not balanced.
*/
void HuffmanEncoder::_buildTree()
{
    size_t c_CharsCount{mHuffmanOccurrenceMap.size()};

    // There should be at least 2 characters to encode and the tree should be initially empty (reset method to be called before executing this method)
    if (c_CharsCount >= scMinRequiredCharsCount && nullptr == mpRoot)
    {
        // resulting nodes count after connecting first 2 leaf nodes to a non-leaf (binding) node and then each other leaf to the previously obtained binding node
        const size_t c_RequiredNodesCount{2 * c_CharsCount - 1};

        mTreeContainer.resize(c_RequiredNodesCount);
        Node* pCurrentNode{&(*mTreeContainer.begin())}; // use pointers to bind the nodes with each other for building the binary tree

        // set first node as initial root
        CharOccurrenceMap::const_iterator occurrenceMapBeginIter{mHuffmanOccurrenceMap.cbegin()};
        mpRoot = pCurrentNode;
        mpRoot->mCharacter = occurrenceMapBeginIter->second;
        mpRoot->mOccurrence = occurrenceMapBeginIter->first;

        ++pCurrentNode;

        // connect all other nodes in increasing occurrence order and move the root upwards (in order to obtain a smaller code size for higher character occurrence)
        for (CharOccurrenceMap::const_iterator it{++occurrenceMapBeginIter}; it != mHuffmanOccurrenceMap.cend(); ++it)
        {
            Node* pNewNode = pCurrentNode;
            pNewNode->mCharacter = it->second;
            pNewNode->mOccurrence = it->first;
            ++pCurrentNode;

            Node* pNewRoot = pCurrentNode;
            pNewRoot->mOccurrence = mpRoot->mOccurrence + pNewNode->mOccurrence;
            ++pCurrentNode;

            if (pNewNode->mOccurrence >= mpRoot->mOccurrence)
            {
                pNewRoot->mpLeftChild = mpRoot;
                pNewRoot->mpRightChild = pNewNode;
            }
            else
            {
                pNewRoot->mpLeftChild = pNewNode;
                pNewRoot->mpRightChild = mpRoot;
            }

            mpRoot = pNewRoot;
        }
    }
}

void HuffmanEncoder::_retrieveEncodingFromTree()
{
    if (mpRoot != nullptr)
    {
        assert(0u == mEncodingResult.size()); // ensured by reset method

        std::string prefix;

        const Node* pCurrentNode{mpRoot};

        while (pCurrentNode != nullptr)
        {
            Node* pLeftChild{pCurrentNode->mpLeftChild};
            Node* pRightChild{pCurrentNode->mpRightChild};

            if (pLeftChild == nullptr)
            {
                mEncodingResult[pCurrentNode->mCharacter] = prefix;
                break;
            }
            else if (pLeftChild->mpLeftChild == nullptr)
            {
                mEncodingResult[pLeftChild->mCharacter] = prefix + scBinaryDigit0;
                prefix += scBinaryDigit1;
                pCurrentNode = pRightChild;
            }
            else
            {
                mEncodingResult[pRightChild->mCharacter] = prefix + scBinaryDigit1;
                prefix += scBinaryDigit0;
                pCurrentNode = pLeftChild;
            }
        }
    }
    else
    {
        assert(false);
    }
}

/* The efficiency might be negative when the characters are uniformly distributed regarding occurrence (extreme case: they have perfectly equal occurrence).
   The least uniform the chars are distributed the higher is the efficiency.
   The efficiency is measured by how much the number of used bits decreases (or increases: negative) when using Huffman algorithm
   comparing to using a fixed-size encoding.
   Minimum number of bits required (for the given number of symbols) is taken into account for fixed-size encoding (e.g. 3 for 6 symbols: 'a', 'b', 'p', 'p', 'u', 'z').
*/
void HuffmanEncoder::_computeEncodingEfficiency()
{
    size_t c_SymbolsCount{mEncodingResult.size()};

    if (c_SymbolsCount > 0u)
    {
        ssize_t requiredBitsPerSymbolCount{0};
        size_t quotient{c_SymbolsCount};

        // get minimum required number of binary digits for the number of symbols to be encoded
        do
        {
            quotient /= 2;
            ++requiredBitsPerSymbolCount;
        }
        while (quotient > 0u);

        ssize_t totalBitsCountPrefixCoding{0u};
        ssize_t totalOccurrencesCount{0u};

        for (CharOccurrenceMap::const_iterator it{mHuffmanOccurrenceMap.cbegin()}; it != mHuffmanOccurrenceMap.cend(); ++it)
        {
            const ssize_t c_CurrentCharacterOccurrencesCount{it->first};
            const char c_CurrentCharacter{it->second};
            totalBitsCountPrefixCoding += (c_CurrentCharacterOccurrencesCount) * mEncodingResult[c_CurrentCharacter].size();
            totalOccurrencesCount += c_CurrentCharacterOccurrencesCount;
        }

        const ssize_t c_TotalBitsCountFixedLength{totalOccurrencesCount * requiredBitsPerSymbolCount};
        mEncodingEfficiency = static_cast<double>(c_TotalBitsCountFixedLength - totalBitsCountPrefixCoding) / c_TotalBitsCountFixedLength;
    }
}

void HuffmanEncoder::_reset()
{
    // disconnect nodes prior to erasing them from tree
    for (TreeContainer::iterator it{mTreeContainer.begin()}; it != mTreeContainer.end(); ++it)
    {
        it->mpLeftChild = nullptr;
        it->mpRightChild = nullptr;
    }

    mpRoot = nullptr;

    mHuffmanOccurrenceMap.clear();
    mTreeContainer.clear();
    mEncodingResult.clear();

    mEncodingEfficiency = 0.0;
}

HuffmanEncoder::Node::Node()
    : mCharacter{scNullCharacter}
    , mOccurrence{0}
    , mpLeftChild{nullptr}
    , mpRightChild{nullptr}
{
}
