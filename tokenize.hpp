#ifndef TOKENIZE_H
#define TOKENIZE_H
#include <sstream>
#include "string.h"
#include <vector>

typedef std::string token;

//Formats the 
token StringFormat(token Unformatted);

//stream splitter. Splits stream into white space separated items.
//stores tokens into a vector.
std::vector<token> StringSplit(token formatted);

//include sstream
// std::stringstream name (string to convert)
#endif // !TOKENIZE_H
