#ifndef _INTLIST_
#define _INTLIST_

#include <vector>
#include "IntStack.h"

int IntList_head(const std::vector<int> input);
std::vector<int> IntList_tail(const std::vector<int> input);
std::vector<int> IntList_app(const int* xs, int xslength, const std::vector<int> input);

#endif
