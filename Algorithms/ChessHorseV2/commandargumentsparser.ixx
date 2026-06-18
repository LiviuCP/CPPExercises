module;

#include <string>

#include "commandargumentsparsingresult.h"

export module commandargumentsparser;

export CommandArgumentsParsingResult parseCommandArguments(int argc, char** argv);
export std::string buildErrorMessage(ErrorType errorType);
