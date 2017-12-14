#ifndef _TOKS_
#define _TOKS_

#include <vector>
#include <string>

class Toks
{
		static bool getWord(std::istream& is, std::string& sval);
		static void toSentences(std::istream& toks, std::vector< std::vector< std::vector<std::string> > >& Wsss);
	public:		
		static void toSentences(const char* s, const bool fromFile, std::vector< std::vector< std::vector<std::string> > >& Wsss);
};

#endif
