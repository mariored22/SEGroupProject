#ifndef _PROG_
#define _PROG_

#include "Engine.h"

class Prog : public Engine
{
protected:
	std::string showClause(const Clause& s);

public:
	Prog(const char* fname);
	virtual ~Prog();
	void ppCode();
	
	virtual std::string exportTerm(int x, bool& isArray);
	std::string st0(int x);
};

#endif
