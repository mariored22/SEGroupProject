#include "Toks.h"


  Toks * Toks::makeToks(string s, bool fromFile) {
		FILE * f;
      if (fromFile) {
		  f = fopen(s.c_str(), "r");
      } else {
		  f = stdin;
      }
      Toks *T = new Toks(f);
      return T;

  }

  Toks::Toks(FILE * f) {

	  this->f = f;
  }


  string * Toks::getWord() {
    string *t = NULL;

	char c;
	fscanf(f, "%c", &c);
	while (!feof(f) && isspace(c))
	{
		fscanf(f, "%c", &c);
	}

	if (feof(f))
		return NULL;

	if (c == '.')
	{
		t = new string(".");
	}
	else if (isupper(c) || c=='_')
	{
		t = new string("v:");
		while (!feof(f) && (isalnum(c) || c=='_'))
		{
			*t += c;
			fscanf(f, "%c", &c);			
		}
	}
	else if (islower(c))
	{
		t = new string("c:");
		while (!feof(f) && isalnum(c))
		{
			*t += c;
			fscanf(f, "%c", &c);
		}
	}
	else if (isdigit(c))
	{
		t = new string("n:");
		while (!feof(f) && isdigit(c))
		{
			*t += c;
			fscanf(f, "%c", &c);
		}
	}
    return t;
  }

  vector<vector<vector<string>* > *> Toks::toSentences(string s, bool fromFile) {
	  vector<vector<vector<string>* > *> Wsss; // = new ArrayList<ArrayList<ArrayList<String>>>();
	  vector<vector<string> *>* Wss = new vector<vector<string> *>();
	  vector<string>* Ws = new vector<string>();//<String>();
     Toks * toks = makeToks(s, fromFile);
    string *t = NULL;
    while (NULL != (t = toks->getWord())) {

      if (*t == ".") {
        Wss->push_back(Ws);
        Wsss.push_back(Wss);
		Wss = new vector<vector<string>* >();
		Ws = new vector<string>(); //ArrayList<String>();
      } else if (*t == "c:if") { //("c:" + IF).equals(t)) {

		  Wss->push_back(Ws); //.add(Ws);

		  Ws = new vector<string>(); //ArrayList<String>();
      } else if (*t == ("c:and")) {
		  Wss->push_back(Ws); //add(Ws);

		  Ws = new vector<string>();//ArrayList<String>();
      } else if (*t=="c:holds") { //.equals(t)) {
			  string w = (*Ws)[0]; //.get(0);
			  (*Ws)[0] = "h:" + w.substr(2);//
      //  Ws.set(0, "h:" + w.substring(2));
      } else if (*t==("c:lists")) { //.equals(t)) {
		  string w = (*Ws)[0];//.get(0);
		  (*Ws)[0] = "l:" + w.substr(2);
      //  Ws.set(0, "l:" + w.substring(2));
      } else if (*t == ("c:is")){ //.equals(t)) {
		  string w = (*Ws)[0]; // .get(0);
		  (*Ws)[0] = "f:" + w.substr(2);
     //   Ws.set(0, "f:" + w.substring(2));
      } else {
		  Ws->push_back(*t);
       // Ws.add(t);
      }
    }

	delete toks;
    return Wsss;
  }


/*
int main() {
		vector<vector<vector<string>* > *> result = Toks::toSentences("perms.pl.nl", true);

		for (vector<vector<string>*>* wss : result)
		{

		}

		return 0;
		// Main.pp(toSentences("prog.nl", true));
}
*/
