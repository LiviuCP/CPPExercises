#include "concreteparsers.h"
#include "parserfactory.h"

Parser* ParserFactory::createParser(ParserType parserType, char* filePath, IAggregator* pIAggregator)
{
    Parser* pParser{nullptr};

    switch (parserType)
    {
    case ParserType::DIGITS:
        pParser = new DigitsParser{filePath, pIAggregator};
        break;
    case ParserType::LOWERCASE:
        pParser = new LowerCaseParser{filePath, pIAggregator};
        break;
    case ParserType::UPPERCASE:
        pParser = new UpperCaseParser{filePath, pIAggregator};
        break;
    case ParserType::LOWER_UPPER_CASE:
        pParser = new LowerUpperCaseParser{filePath, pIAggregator};
        break;
    case ParserType::ALPHA_AND_DIGITS:
        pParser = new AlphaAndDigitsParser{filePath, pIAggregator};
        break;
    default:
        break;
    }

    return pParser;
}
