#ifndef USERINPUT_H
#define USERINPUT_H

#include "matrixutils.h"

struct UserInput
{
    matrix_opt_size_t m_TableLength;
    matrix_opt_size_t m_TableWidth;
    matrix_opt_size_t m_StartPositionX;
    matrix_opt_size_t m_StartPositionY;

    UserInput(const std::string& tableLengthStr,
              const std::string& tableWidthStr,
              const std::string& startPositionXStr,
              const std::string& startPositionYStr);

    bool isValid() const;

private:
    static void parseInputParameter(const std::string& inputParameter, matrix_opt_size_t& field);
};

#endif // USERINPUT_H
