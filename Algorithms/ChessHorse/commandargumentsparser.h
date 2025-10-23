#ifndef COMMANDARGUMENTSPARSER_H
#define COMMANDARGUMENTSPARSER_H

#include "matrix.h"

enum class ResultType
{
    SUCCESS = 0,
    INVALID_ARGUMENTS_COUNT,
    INVALID_STRING,
    INVALID_TABLE_SIZE,
    INVALID_START_POSITION
};

struct ApplicationInput
{
    matrix_size_t m_TableLength;
    matrix_size_t m_TableWidth;
    matrix_size_t m_StartPositionX;
    matrix_size_t m_StartPositionY;
};

using Result = std::pair<ResultType, std::optional<ApplicationInput>>;

Result parseCommandArguments(int argc, char** argv);

#endif // COMMANDARGUMENTSPARSER_H
