#include <iostream>
#include <string>
#include <cstdlib>
#include "Spine.h"

int Spine::created= 0;
int Spine::destroyed = 0;

Spine::Spine(const int* gs0, const int gslength, const int base, std::vector<int>& gs, const int ttop, const int k, std::vector<int>* cs)
{
	hd = gs0[0];
	this->base = base;
	this->gs = IntList_tail(IntList_app(gs0, gslength, gs)); // prepends the goals of clause with head hs
	this->ttop = ttop;
	this->k = k;
	this->_cs = cs;
	this->xsNull = true;

	created++;
}

Spine::Spine(const int hd, const int ttop)
{
	this->hd = hd;
	base = 0;
	//gs = NULL;
	this->ttop = ttop;

	k = -1;
	_cs = NULL;
	xsNull = true;

	created++;
}

Spine::~Spine()
{	
	//IntList::deleteList(gs);	
	destroyed++;
}

int Spine::cs(int index) const
{
	return _cs->at(index);
}

int Spine::csLength() const
{
	return (int)_cs->size();
}
