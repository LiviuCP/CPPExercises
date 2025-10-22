#include <algorithm>
#include <cctype>

#include "userinput.h"

UserInput::UserInput(const std::string& tableLengthStr,
                     const std::string& tableWidthStr,
                     const std::string& startPositionXStr,
                     const std::string& startPositionYStr)
{
    parseInputParameter(tableLengthStr, m_TableLength);
    parseInputParameter(tableWidthStr, m_TableWidth);
    parseInputParameter(startPositionXStr, m_StartPositionX);
    parseInputParameter(startPositionYStr, m_StartPositionY);
}

bool UserInput::isValid() const
{
    bool isValidInput = false;

    do
    {
        if (!m_TableLength.has_value() || !m_TableWidth.has_value() || !m_StartPositionX.has_value() || !m_StartPositionY.has_value())
        {
            break;
        }

        if (0 == m_TableLength || 0 == m_TableWidth)
        {
            break;
        }

        if (0 == m_StartPositionX || m_StartPositionX > m_TableLength)
        {
            break;
        }

        if (0 == m_StartPositionY || m_StartPositionY > m_TableWidth)
        {
            break;
        }

        isValidInput = true;
    }
    while(false);

    return isValidInput;
}

void UserInput::parseInputParameter(const std::string& inputParameter, matrix_opt_size_t& field)
{
    field.reset();

    if (!inputParameter.empty())
    {
        std::string inputCopy{inputParameter};
        Utilities::trimWhiteSpace(inputCopy);

        if (std::all_of(inputCopy.cbegin(), inputCopy.cend(), [](char ch){return std::isdigit(ch);}))
        {
            field = std::stoi(inputCopy);
        }
    }
}
