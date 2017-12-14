#ifndef _ENGINE_
#define _ENGINE_ 

#include <map>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include "Clause.h"
#include "IntStack.h"
#include "IntMap.h"
#include "IMap.h"
#include "Spine.h"

 
// switches off indexing for less then START_INDEX clauses e.g. <20
#define START_INDEX		20
  

/**
 * Implements execution mechanism
 */
class Engine 
{
private:
	

	int addSym(const std::string& sym);
	

	void ensureSize(const int more);
	void expand();

	void makeHeap(); 
	void makeHeap(const int size);
	int getTop() const; 
	int setTop(const int top);
	void clear(); 
	void push(const int i);
	int size() const;

	int encode(const int t, const std::string& s);

	static void mapExpand(std::vector< std::vector<std::string> >& Wss, std::vector< std::vector<std::string> >& Rss);
	static bool maybeExpand(std::vector<std::string>& Ws, std::vector< std::vector<std::string> >& Rss);

	static int relocate(const int b, const int cell);
	void pushCells(const int b, const int from, const int to, const int base);
	void pushCells(const int b, const int from, const int to, const int* cs);
	int pushHead(const int b, const Clause& C);

	int* getIndexables(const int ref, int& xsLength);
	int cell2index(const int cell);
	static bool isVAR(const int x);
	int getRef(const int x);
	void setRef(const int w, const int r);
	void unwindTrail(const int savedTop);

	Spine* init();
	Spine* answer(int ttop);
	Clause& getQuery();

	static void put(IMap<int> imaps[], IntMap vss[], int keys[], int val);
	bool match(const int* xs, const Clause& C0);
	bool unify(const int base);
	bool unify_args(const int w1, const int w2);
protected:
	static int tag(const int t, const int w);
	static int detag(const int w);
	static int tagOf(const int w);

	std::string getSym(int w);

	/**
	 * trimmed down clauses ready to be quickly relocated to the heap
	 */
	std::vector<Clause> clauses;	

	std::vector<int> cls;	
	/** symbol table made of map + reverse map from ints to syms */

	std::map<std::string, int> syms;
	std::vector<std::string> slist;

	/** runtime areas:
	 *
	 * the heap contains code for and clauses their their copies
	 * created during execution
	 *
	 * the trail is an undo list for variable bindings
	 * that facilitates retrying failed goals with alternative
	 * matching clauses
	 *
	 * the unification stack ustack helps handling term unification non-recursively
	 *
	 * the spines stack contains abstractions of clauses and goals and performs the
	 * functions of  both a choice-point stack and goal stack
	 *
	 * imaps: contains indexes for up toMAXIND>0 arg positions (0 for pred symbol itself)
	 *
	 * vmaps: contains clause numbers for which vars occur in indexed arg positions
	 */

	int* heap;
	int heapLength;

	int top;
	  	
	IntStack trail;
	IntStack ustack;

	Spine* query;

	std::list<Spine*> spines;
	IntMap vmaps[MAXIND];
	IMap<int> imaps[MAXIND];	 
	bool imaps_null;

	/**
	 * tags of our heap cells - that can also be seen as
	 * instruction codes in a compiled implementation
	 */
	static const int V = 0;
	static const int U = 1;
	static const int R = 2;

	static const int C = 3;
	static const int N = 4;
	static const int A = 5;

	// G - ground?
	static const int BAD = 7;

	void dload(const char* s);
	
	Clause putClause(const IntStack& cs, const IntStack& gs, const int neck);
	int deref(int x);

	void pushBody(const int b, const int head, Clause& C, int*& gs, int& gsLength);
	void makeIndexArgs(Spine* G, const int goal);

	virtual std::string exportTerm(int x, bool& isArray); 
	std::string showTerm(const int x); 

	std::string showCell(const int w);	
	std::string showCells(const int base, const int len);

	static std::vector<int> toNums(std::vector<Clause>& clauses);

	void index(std::vector<Clause>& clauses, IntMap vmaps[]);
	std::string ask(bool& isArray, bool& isNull);
	Spine* yield(); 

	void popSpine();
	bool hasGoals(Spine* S);
	bool hasClauses(Spine* S);
	Spine* unfold(Spine* G);
public:
	void run();
	Engine(const char* fname);
	virtual ~Engine();
};

#endif

