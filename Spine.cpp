#include "Spine.h"


/**
 * runtime representation of an immutable list of goals
 * together with top of heap and trail pointers
 * and current clause tried out by head goal
 * as well as registers associated to it
 *
 * note that parts of this immutable lists
 * are shared among alternative branches
 */


Spine::Spine( vector<int> gs0,  int base,  IntList *gs,  int ttop,  int k, vector <int> cs) {
    hd = gs0[0];
    this->base = base;
    this->gs = IntList::tail(IntList::app(gs0, gs)); // prepends the goals of clause with head hs
    this->ttop = ttop;
    this->k = k;
	this->cs = new vector<int>();
	for (int x : cs)
		this->cs->push_back(x);
  }


Spine::Spine( int hd,  int ttop) {
    this->hd = hd;
    base = 0;
    gs = NULL;
    this->ttop = ttop;

    k = -1;
    cs = NULL;
  }
