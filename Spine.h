#ifndef _SPINE_
#define _SPINE_

/**
 * runtime representation of an immutable list of goals
 * together with top of heap and trail pointers
 * and current clause tried out by head goal
 * as well as registers associated to it
 *
 * note that parts of this immutable lists
 * are shared among alternative branches
 */

#include <cstddef>
#include <vector>
#include "IntList.h"

// number of index args
#define MAXIND			3

class Spine
{	
public:
	static int created;
	static int destroyed;

	Spine(const int* gs0, const int gslength, const int base, std::vector<int>& gs, const int ttop, const int k, std::vector<int>* cs);
	Spine(const int hd, const int ttop);
	~Spine();

	int hd; // head of the clause to which this corresponds
	int base; // top of the heap when this was created

	std::vector<int> gs;
	/*IntList* gs;*/ // goals - with the top one ready to unfold
	int ttop; // top of the trail when this was created

	int k;

	int xs[MAXIND];
	bool xsNull;
	
	int cs(int index) const;
	int csLength() const;

        void print();
private:
	std::vector<int>* _cs; // array of  clauses known to be unifiable with top goal in gs	
};


#endif
