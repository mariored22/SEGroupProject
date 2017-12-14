#ifndef _CLAUSE_
#define _CLAUSE_

/**
 * representation of a clause
 */
class Clause 
{
	public:
		static int created;
		static int destroyed;

		int len;
		int* hgs;
		int hgsLength;
		int base;
		int neck;
		int* xs;
		int xsLength;

	public:
		Clause(int len, int* hgs, int hgsLenght, int base, int neck, int* xs, int xsLength);
		Clause(const Clause& copied);
		Clause operator=(const Clause& copied);
		~Clause();
};

#endif
