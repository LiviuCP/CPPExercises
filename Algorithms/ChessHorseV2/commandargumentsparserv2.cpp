module;

#include <cctype>

#include "commandargumentsparsingresult.h"
#include "matrixutils.h"

module commandargumentsparser;

// required input data received by application
struct ApplicationArguments
{
    std::string m_TableLengthStr;
    std::string m_TableWidthStr;
    std::string m_StartPositionXStr;
    std::string m_StartPositionYStr;
};

matrix_opt_size_t convertArgumentToInt(const std::string& argument)
{
    matrix_opt_size_t result;

    std::string inputCopy{argument};
    Utilities::trimWhiteSpace(inputCopy);

    if (!inputCopy.empty() &&
        std::all_of(inputCopy.cbegin(), inputCopy.cend(), [](char ch) { return std::isdigit(ch); }))
    {
        result = static_cast<matrix_size_t>(std::stoi(inputCopy));
    }

    return result;
}

CommandArgumentsParsingResult convertArgumentsToApplicationInput(const ApplicationArguments& applicationArguments)
{
    assert(!applicationArguments.m_TableLengthStr.empty() && !applicationArguments.m_TableWidthStr.empty() &&
           !applicationArguments.m_StartPositionXStr.empty() && !applicationArguments.m_StartPositionYStr.empty());

    const matrix_opt_size_t c_TableLength{convertArgumentToInt(applicationArguments.m_TableLengthStr)};
    const matrix_opt_size_t c_TableWidth{convertArgumentToInt(applicationArguments.m_TableWidthStr)};
    const matrix_opt_size_t c_StartPositionX{convertArgumentToInt(applicationArguments.m_StartPositionXStr)};
    const matrix_opt_size_t c_StartPositionY{convertArgumentToInt(applicationArguments.m_StartPositionYStr)};

    std::optional<ApplicationInput> applicationInput;

    if (c_TableLength.has_value() && c_TableWidth.has_value() && c_StartPositionX.has_value() &&
        c_StartPositionY.has_value())
    {
        applicationInput = {*c_TableLength, *c_TableWidth, *c_StartPositionX, *c_StartPositionY};
    }

    std::optional<ErrorType> errorType;

    do
    {
        if (!applicationInput.has_value())
        {
            errorType = ErrorType::INVALID_STRING;
            break;
        }

        if (0 == applicationInput->m_TableLength || 0 == applicationInput->m_TableWidth)
        {
            errorType = ErrorType::INVALID_TABLE_SIZE;
            break;
        }

        if (0 == applicationInput->m_StartPositionX ||
            applicationInput->m_StartPositionX > applicationInput->m_TableLength ||
            0 == applicationInput->m_StartPositionY ||
            applicationInput->m_StartPositionY > applicationInput->m_TableWidth)
        {
            errorType = ErrorType::INVALID_START_POSITION;
        }
    } while (false);

    return !errorType.has_value() ? std::expected<ApplicationInput, ErrorType>{*applicationInput}
                                  : std::unexpected<ErrorType>{*errorType};
}

CommandArgumentsParsingResult parseCommandArguments(int argc, char** argv)
{
    static constexpr int c_MinRequiredArgumentsCount{5}; // application file path included
    CommandArgumentsParsingResult result{std::unexpected<ErrorType>{ErrorType::INVALID_ARGUMENTS_COUNT}};

    if (argc >= c_MinRequiredArgumentsCount)
    {
        int validArgumentsCount{0};

        if (argv)
        {
            for (size_t index{0}; index < c_MinRequiredArgumentsCount; ++index)
            {
                if (!argv[index])
                {
                    break;
                }

                ++validArgumentsCount;
            }
        }

        assert(validArgumentsCount == c_MinRequiredArgumentsCount);

        if (validArgumentsCount == c_MinRequiredArgumentsCount)
        {
            const ApplicationArguments c_ApplicationArguments{{argv[1]}, {argv[2]}, {argv[3]}, {argv[4]}};
            result = convertArgumentsToApplicationInput(c_ApplicationArguments);
        }
    }

    return result;
}

std::string buildErrorMessage(ErrorType errorType)
{
    static const std::map<ErrorType, std::string> c_ErrorMessages{
        {ErrorType::INVALID_ARGUMENTS_COUNT,
         "Insufficient arguments provided, should be at least 5 (including application file path).\n"},
        {ErrorType::INVALID_STRING, "The input contains invalid (non-numeric) characters.\n"},
        {ErrorType::INVALID_TABLE_SIZE, "The table size is invalid (both dimensions should be greater than 0).\n"},
        {ErrorType::INVALID_START_POSITION,
         "The starting position is invalid (should be between 1 and rows/columns count).\n"},
    };

    return c_ErrorMessages.at(errorType);
}
