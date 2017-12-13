#ifndef __CLAUSE_H__
#define __CLAUSE_H__
/**
 * representation of a clause
 */

#include <vector>

using namespace std;

class Clause {
public:

	Clause(int len, vector<int> hgs, int base, int neck, vector<int> xs);

  int len;
  vector<int> hgs;
  int base;
  int neck;
  vector<int> xs;
};

#endif
