#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cctype>
#include "Toks.h"

std::string nextToken;

#define IF     "if"
#define AND    "and"
#define DOT    "."
#define HOLDS  "holds"
#define LISTS  "lists" // todo
#define IS     "is" // todo

bool Toks::getWord(std::istream& is, std::string& sval)
{		
	if (nextToken!="")
	{
		sval = nextToken;
		nextToken = "";
	}
	else
	{
		char c;
		bool finished = false;
		sval = "";

		do
		{
			if (is.read(&c, 1))
			{
				if (!isspace(c))
				{
					if (c=='_' || isalnum(c))
					{
						sval += c;
					}
					else if (c=='.' || (c>='!' && c<='/') || (c>=':' && c<='@') || (c>='[' && c<='`') || (c>='{' && c<='~'))
					{
						if (sval.size()>0)
							nextToken = c;
						else
						{
							sval = c;
							nextToken = "";
						}
						finished = true;
					}
				}
				else
				{
					if (sval.size()>0)
						finished = true;
				}
			}
			else
				finished = true;

		}while(!finished);
	}

	if (sval.size()>0)
	{
		char c  = sval[0];

		if (c=='_' || isupper(c))
		{
			sval = "v:" + sval;		
		}		
		else if (!(c=='\n' || c=='.' || (c>='!' && c<='/') || (c>=':' && c<='@') || (c>='[' && c<='`') || (c>='{' && c<='~')))
		{
			int n = -1;
			std::stringstream ss(sval);
			if (ss >> n)
			{
				if (abs(n)< (1 << 28))
				{
					sval =  "n:" + sval;
				}
				else
				{
					sval = "c:" + sval;
				}
			}
			else
			{
				sval = "c:" + sval;
			}
		}
	}

	return (sval.size()>0 || nextToken.size()>0);
}

void Toks::toSentences(std::istream& toks, std::vector< std::vector< std::vector<std::string> > >& Wsss)
{
	std::vector< std::vector<std::string> > Wss;
   	std::vector<std::string> Ws;

	std::string t = "";

	std::string sval;

	toks >> std::noskipws;
	while(getWord(toks, t))
	{
		if (t==DOT)
		{
		        Wss.push_back(Ws);
			Wsss.push_back(Wss);
			Wss.clear();
			Ws.clear();
		}
		else if (t == "c:" IF) 
		{
			Wss.push_back(Ws);
			Ws.clear();
		}
		else if (t == "c:" AND)
		{
			Wss.push_back(Ws);
			Ws.clear();
		} 
		else if (t == "c:" HOLDS)
		{

			std::string w = Ws[0];
			Ws[0] = "h:" + w.substr(2);
			
		} 
		else if (t == "c:" LISTS)
		{
			std::string w = Ws[0];
			Ws[0] = "l:" + w.substr(2);			
		}
		else if (t == "c:" IS) 
		{
			std::string w = Ws[0];
			Ws[0] = "f:" + w.substr(2);
		}
		else
		{
			Ws.push_back(t);
		}
	}	
}

// Vano101: there is left to handle comments
void Toks::toSentences(const char* s, const bool fromFile, std::vector< std::vector< std::vector<std::string> > >& Wsss)
{	
	if (fromFile)
	{
		std::ifstream toks(s);
		toSentences(toks, Wsss);		
	}
	else
	{
		std::stringstream toks(s);
		toSentences(toks, Wsss);
	}		
}	
