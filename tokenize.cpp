#include "tokenize.hpp"

token StringFormat(token Unformatted)
{
	for (size_t i = 0; i < Unformatted.size(); i++)
	{
		if (Unformatted.at(i) == '(')
		{
			Unformatted.insert((i+1), " ");
			Unformatted.insert(i, " ");
			i++;
		}
		else if (Unformatted.at(i) == ')')
		{
			Unformatted.insert((i+1), " ");
			Unformatted.insert(i, " ");
			i++;
		}
		else if (Unformatted.at(i) == ';')
		{
			Unformatted.insert(i, " ");
			for (i; i < Unformatted.size(); i++)
			{
				Unformatted.erase(i);
			}
		}
		else if (Unformatted.at(i) == '\n')
		{
			Unformatted.erase(i);
		}
	}
	return Unformatted;
}


std::vector<token> StringSplit(token formatted)
{
	std::istringstream TokenStream(formatted);
	token storage;
	std::vector<token> ParsedData;
	while (TokenStream >> storage)
	{
		ParsedData.push_back(storage);
	}
	return ParsedData;
}