#include "IntList.h"

int IntList_head(const std::vector<int> input)
{
	return input[0];
}

std::vector<int> IntList_tail(const std::vector<int> input)
{
	std::vector<int> result;
	int i;
	for(i=1; i<(int)input.size(); i++)
		result.push_back(input[i]);
	return result;
}

std::vector<int> IntList_app(const int* xs, int xslength, const std::vector<int> input)
{
	std::vector<int> result;
	int i;
	int j;

	for(i=0; i<xslength; i++) 
	{
		result.push_back(xs[i]);
	}

	for(j=0; j<(int)input.size(); j++) 
	{
		result.push_back(input[j]);
	}
	return result;
}
