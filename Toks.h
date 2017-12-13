#ifndef __TOKS_H__
#define __TOKS_H__

#include <string>
#include <vector>
#include <cstdio>
#include <cctype>

#pragma warning(disable: 4996)

using namespace std;

/**
 * Reads chars from char streams using the current default encoding
 */
class Toks{

public:

	static Toks * makeToks(string s, bool fromFile);
	Toks(FILE * f);

  FILE * f;

  string * getWord();
  static vector<vector<vector<string>* > *> toSentences(string s, bool fromFile);

};

#endif
