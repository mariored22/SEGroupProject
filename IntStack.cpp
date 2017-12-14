#include "IntStack.h"
#include <cstdlib>
#include <cstring>

IntStack::IntStack(const int size) 
{
	stack = new int[size];
	stackLength = size;
	clear();
}

IntStack::~IntStack()
{
	delete stack;
}

IntStack::IntStack(const IntStack& copied)
{
	stack = new int[copied.stackLength];
	stackLength = copied.stackLength;
	memcpy(stack, copied.stack, sizeof(int) * stackLength);
	top = copied.top;
}

IntStack IntStack::operator=(const IntStack& copied)
{
	delete stack;
	stack = new int[copied.stackLength];
	stackLength = copied.stackLength;
	memcpy(stack, copied.stack, sizeof(int) * stackLength);
	top = copied.top;
	return *this;
}

int IntStack::getTop() const
{
	return top;
}

int IntStack::setTop(int top) 
{
	int oldTop = this->top;
	this->top = top;
	return oldTop;
}

void IntStack::clear() 
{
	//for (int i = 0; i <= top; i++)
	//stack[i] = 0;
	top = -1;
}

bool IntStack::isEmpty() const
{
	return top < 0;
}

/**
 * Pushes an element - top is incremented first than the
 * element is assigned. This means top point to the last assigned
 * element - which can be returned with peek().
 */
void IntStack::push(const int i) 
{
	// IO.dump("push:"+i);
	if (++top >= stackLength) 
	{
		expand();
	}
	stack[top] = i;
}

int IntStack::pop() 
{
	int r = stack[top--];
	shrink();
	return r;
}

int IntStack::get(const int i) const
{
	return stack[i];
}

void IntStack::set(const int i, const int val)
{
	stack[i] = val;
}

int IntStack::size() const
{
	return top + 1;
}

/*
int IntStack::stackSize() const
{
	return stackLength;
}
*/

/**
 * dynamic array operation: doubles when full
 */
void IntStack::expand() 
{
	const int l = stackLength;
	int* newstack = new int[l << 1];

	memcpy(newstack, stack, sizeof(int)*l);
	delete stack;
	stack = newstack;
	stackLength = l << 1;
}

/**
 * dynamic array operation: shrinks to 1/2 if more than than 3/4 empty
 */
void IntStack::shrink() 
{
	int l = stackLength;
	if (l <= MINSIZE || top << 2 >= l)
		return;

	l = 1 + (top << 1); // still means shrink to at 1/2 or less of the heap
	if (top < MINSIZE) 
	{
		l = MINSIZE;
	}

	int* newstack = new int[l];
	memcpy(newstack, stack, sizeof(int)*l);
	delete stack;
	stack = newstack;
	stackLength = l;
}
