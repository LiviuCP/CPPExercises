module;

#include <expected>
#include <string>

export module commandargumentsparser;
import matrix;

enum class ErrorType
{
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

export using CommandArgumentsParsingResult = std::expected<ApplicationInput, ErrorType>;

export CommandArgumentsParsingResult parseCommandArguments(int argc, char** argv);
export std::string buildErrorMessage(ErrorType errorType);
