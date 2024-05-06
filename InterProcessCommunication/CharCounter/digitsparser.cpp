#include "digitsparser.h"

DigitsParser::DigitsParser(char* name)
    : Parser(name)
{
}

bool DigitsParser::okchar(char c)
{
    return c >= 48 && c <= 57;
}
