#include "Clause.h"
#include <cstring>

int Clause::created = 0;
int Clause::destroyed = 0;

Clause::Clause(int len, int* hgs, int hgsLenght, int base, int neck, int* xs, int xsLength)
{	
	this->hgsLength = hgsLenght;
	this->hgs = new int[hgsLenght];
	memcpy(this->hgs, hgs, sizeof(int)*(this->hgsLength));	
	this->base = base;
	this->neck = neck;
	this->xsLength = xsLength;
	this->xs = new int[xsLength];	
	memcpy(this->xs, xs, sizeof(int)*(this->xsLength));
	this->len = len;	

	created++;
}

Clause::Clause(const Clause& copied)
{	
	hgs = new int[copied.hgsLength];
	hgsLength = copied.hgsLength;
	memcpy(hgs, copied.hgs, sizeof(int)*copied.hgsLength);	
	base = copied.base;
	neck = copied.neck;
	xs = new int[copied.xsLength];
	xsLength = copied.xsLength;
	memcpy(xs, copied.xs, sizeof(int)*copied.xsLength);
	len = copied.len;

	created++;
}

Clause Clause::operator=(const Clause& copied)
{
	delete hgs;
	delete xs;

	
	hgsLength = copied.hgsLength;
	hgs = new int[copied.hgsLength];
	memcpy(hgs, copied.hgs, sizeof(int)*copied.hgsLength);	
	base = copied.base;
	neck = copied.neck;
	xs = new int[copied.xsLength];
	xsLength = copied.xsLength;
	memcpy(xs, copied.xs, sizeof(int)*copied.xsLength);	
	len = copied.len;
	return *this;
}

Clause::~Clause()
{
	delete hgs;
	delete xs;

	destroyed++;
}

/*
int main()
{
	Clause h(10,{4,6,7},9,{3,4,5}6,)	
	cout<<h.base;
}
*/
