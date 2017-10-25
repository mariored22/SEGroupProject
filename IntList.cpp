#include "IntList.h"

namespace iProlog
{

	IntList::IntList(int const head) : head(head), tail(nullptr)
	{
	}

	IntList::IntList(int const X, IntList *const Xs) : head(X), tail(Xs)
	{
	}

	bool IntList::isEmpty(IntList *const Xs)
	{
	  return nullptr == Xs;
	}

	int IntList::head(IntList *const Xs)
	{
	  return Xs->head_Renamed;
	}

	IntList *IntList::tail(IntList *const Xs)
	{
	  return Xs->tail_Renamed;
	}

	IntList *IntList::cons(int const X, IntList *const Xs)
	{
	  return new IntList(X, Xs);
	}

	IntList *IntList::app(std::vector<int> &xs, IntList *const Ys)
	{
	  IntList *Zs = Ys;
		
		int i = xs.size() - 1;
		while(i>=0)
		{
		  Zs = cons(xs[i], Zs);
			i--;
		}
		
		  return Zs;
	}

	IntStack *IntList::toInts(IntList *Xs)
	{
	  IntStack * const is = new IntStack();
	  while (!isEmpty(Xs))
	  {
		is->push(head(Xs));
		Xs = tail(Xs);
	  }
	  return is;
	}

	int IntList::len(IntList *const Xs)
	{
	  return toInts(Xs)->size();
	}

	std::wstring IntList::toString()
	{
//JAVA TO C++ CONVERTER TODO TASK: There is no native C++ equivalent to 'toString':
	  return toInts(this)->toString();
	}
}
