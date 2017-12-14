
Conversation opened. 1 unread message.


Skip to content
Using Gmail with screen readers
Search



Gmail
COMPOSE
Labels
Inbox (43)
Starred
Important
Sent Mail
Drafts (65)
Categories
Notes
Personal
Travel
More labels 
Hangouts

 
 
 
  More 
1 of 1,916  
 
Print all In new window
Final files 
Inbox
x 

PRUTHVI TEJA ANUMANDLA
Attachments9:16 PM (8 minutes ago)

to chandrasekhar, akhil, me, pooja 
27 Attachments 
 
	
Click here to Reply, Reply to all, or Forward
8.25 GB (55%) of 15 GB used
Manage
Terms - Privacy
Last account activity: 47 minutes ago
Details

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstring>
#include "Engine.h"
#include "IntStack.h"
#include "Toks.h"
#include "Main.h"


#define ENGINE_MINSIZE  (1 << 15) // power of 2

Engine::Engine(const char* fname)
{
	imaps_null = true;
	makeHeap();
	dload(fname);

	cls = toNums(clauses);

    query = init();
	
    index(clauses, vmaps);
}

Engine::~Engine()
{
	delete heap;
	if (query)
		delete query;
}

std::vector<int> Engine::toNums(std::vector<Clause>& clauses)
{
	std::vector<int> result;
	for(size_t i=0; i<clauses.size(); i++)
	{
		result.push_back((int)i);
	}
	return result;
}


/**
 * tags an integer value while fliping it into a negative
 * number to ensure that untagged cells are always negative and the tagged
 * ones are always positive - a simple way to ensure we do not mix them up
 * at runtime
 */
int Engine::tag(const int t, const int w) 
{
	return -((w << 3) + t);
}

/**
 * removes tag after flipping sign
 */
int Engine::detag(const int w) 
{
    return -w >> 3;
}

/**
 * extracts the tag of a cell
 */
int Engine::tagOf(const int w) 
{
	return -w & 7;
}

/**
* places an identifier in the symbol table
*/
int Engine::addSym(const std::string& sym) 
{
	std::map<std::string, int>::iterator I = syms.find(sym);
    if (I==syms.end()) 
	{
		int i = (int)syms.size();		
		syms[sym] = i;
		slist.push_back(sym);
		return i;
    }
	else
		return I->second;;
}

/**
 * returns the symbol associated to an integer index
 * in the symbol table
 */

// Vano101: it's slower version that Java version
std::string Engine::getSym(int w) 
{
	if (w < 0 || w >= (int)slist.size())
	{
		std::stringstream ss;
		ss << "BADSYMREF=" << w;
		return ss.str();
	}
	return slist[w];
}

void Engine::makeHeap() 
{
	makeHeap(ENGINE_MINSIZE);
}

void Engine::makeHeap(const int size) 
{
	heap = new int[size];
	heapLength = size;
    clear();
}

int Engine::getTop() const
{
	return top;
}

int Engine::setTop(const int top) 
{
	int oldTop = this->top;
	this->top = top;
	return oldTop;
}

void Engine::clear() 
{	
	top = -1;
}


/**
 * Pushes an element - top is incremented frirst than the
 * element is assigned. This means top point to the last assigned
 * element - which can be returned with peek().
 */
void Engine::push(const int i) 
{
	heap[++top] = i;
}

int Engine::size() const
{
	return top + 1;
}

/**
 * dynamic array operation: doubles when full
 */
void Engine::expand() 
{
    int l = heapLength;
    int* newstack = new int[l << 1];

	memcpy(newstack, heap, sizeof(int)*l);
	delete heap;
    heap = newstack;	
}

void Engine::ensureSize(const int more) 
{
    if (1 + top + more >= heapLength) 
	{
		expand();
    }
}


/**
 * expands a "Xs lists .." statements to "Xs holds" statements
 */
bool Engine::maybeExpand(std::vector<std::string>& Ws, std::vector< std::vector<std::string> >& Rss)
{
	std::string W = Ws[0];
    if (W.length() < 2 || (W.substr(0, 2)!="l:"))
		return false;

    int l = (int)Ws.size();    
	std::string V = W.substr(2);

	for (int i = 1; i < l; i++) 
	{		
		std::stringstream ss1;
		ss1 << (i-1);
		std::stringstream ss2;
		ss2 << i;

		std::vector<std::string> Rs;
		std::string Vi = 1 == i ? V : V + "__" + ss1.str();
		std::string Vii = V + "__" + ss2.str();

		Rs.push_back("h:" + Vi);
		Rs.push_back("c:list");
		Rs.push_back(Ws[i]);
		Rs.push_back(i == l - 1 ? "c:nil" : "v:" + Vii);
		Rss.push_back(Rs);
	}
	return true;
}

/**
 * expands, if needed, "lists" statements in sequence of statements
 */
void Engine::mapExpand(std::vector< std::vector<std::string> >& Wss, std::vector< std::vector<std::string> >& Rss)
{
	for (std::vector< std::vector<std::string> >::iterator Ws = Wss.begin(); Ws != Wss.end(); Ws++)
	{
		std::vector< std::vector<std::string> > Hss;
		  
		if (!maybeExpand(*Ws, Hss)) 
		{			
			Rss.push_back(*Ws);
		}
		else 
		{		
			for (std::vector< std::vector<std::string> >::iterator X = Hss.begin(); X!=Hss.end(); X++)
			{
				Rss.push_back(*X);		
			}
		}
    }    
}

void Engine::dload(const char* s)
{
	const bool fromFile = true;
	std::vector< std::vector< std::vector<std::string> > > Wsss;
	Toks::toSentences(s, fromFile, Wsss);

	for (std::vector< std::vector< std::vector<std::string> > >::iterator Wss = Wsss.begin(); Wss!=Wsss.end(); Wss++)
	{
		std::map<std::string, IntStack> refs;
		IntStack cs;
		IntStack gs;

		std::vector< std::vector<std::string> > Rss;
		mapExpand(*Wss, Rss);
		int k = 0;

		for (std::vector< std::vector<std::string> >::iterator ws = Rss.begin(); ws != Rss.end(); ws++)
		{
			// head or body element starts here
			int l = (int)ws->size();
			gs.push(tag(R, k++));
			cs.push(tag(A, l));

			for (std::vector<std::string>::iterator ww = ws->begin(); ww != ws->end(); ww++)
			{
				// head or body subterm starts here

				std::string w= *ww;
				if (1 == w.size()) 
				{
					w = "c:" + w;
				}

				std::string L = w.substr(2);

				switch (w[0]) 
				{
					case 'c':
						cs.push(encode(C, L));
						k++;
					break;
					case 'n':
						cs.push(encode(N, L));
						k++;
					break;
					case 'v': 
						{
							refs[L].push(k);
							cs.push(tag(BAD, k)); // just in case we miss this
							k++;
						}
					break;
					case 'h':
						{
							refs[L].push(k - 1);
							cs.set(k - 1, tag(A, l - 1));
							gs.pop();
						}
						break;
					default:
						pp("FORGOTTEN=" + w);
				  } // end subterm
			} // end element
		}// end clause

		// linker
		for(std::map<std::string, IntStack>::iterator kk=refs.begin(); kk!=refs.end(); kk++)
		{
			IntStack& Is = kk->second;

			// finding the A among refs
			int leader = -1;
			for (int ii=0; ii<Is.size(); ii++) 
			{
				int j = Is.get(ii);
				if (A == tagOf(cs.get(j))) 
				{
					leader = j;
					break;
				}
			}
			if (-1 == leader) 
			{
				// for vars, first V others U
				leader = Is.get(0);
				for (int ii=0; ii<Is.size(); ii++) 
				{
					int i = Is.get(ii);
					if (i == leader) 
					{
						cs.set(i, tag(V, i));
					}
					else
					{
						cs.set(i, tag(U, leader));
					}
				}
			} 
			else 
			{
				for (int ii=0; ii<Is.size(); ii++) 
				{
					int i = Is.get(ii);
					if (i == leader) 
					{
						continue;
					}
					cs.set(i, tag(R, leader));
				}
			}
		}

		int neck = 1 == gs.size() ? cs.size() : detag(gs.get(1));				
		clauses.push_back(putClause(cs, gs, neck));
	}
}


/*
 * encodes string constants into symbols while leaving
 * other data types untouched
 */
int Engine::encode(const int t, const std::string& s)
{
	int w;
	std::stringstream ss(s);

	if (!(ss>>w))
	{
		if (C == t) 
		{
			w = addSym(s);
		}
		else
		{
			//pp("bad in encode=" + t + ":" + s);
			return tag(BAD, 666);
		}
	}

	return tag(t, w);
}

/**
 * true if cell x is a variable
 * assumes that variables are tagged with 0 or 1
 */
bool Engine::isVAR(const int x) 
{
    //final int t = tagOf(x);
    //return V == t || U == t;
    return tagOf(x) < 2;
}

/**
 * returns the heap cell another cell points to
 */
int Engine::getRef(const int x) 
{
    return heap[detag(x)];
}

/**
 * sets a heap cell to point to another one
 */
void Engine::setRef(const int w, const int r) 
{
    heap[detag(w)] = r;
}

/**
 * removes binding for variable cells
 * above savedTop
 */
void Engine::unwindTrail(const int savedTop) 
{
    while (savedTop < trail.getTop()) 
	{
		int href = trail.pop();
		// assert href is var

		setRef(href, href);
    }
}

/**
 * scans reference chains starting from a variable
 * until it points to an unbound root variable or some
 * non-variable cell
 */
int Engine::deref(int x) 
{
    while (isVAR(x)) 
	{
		int r = getRef(x);
		if (r == x) {
			break;
		}
		x = r;
    }
    return x;
}
 
/**
 * places a clause built by the Toks reader on the heap
 */
Clause Engine::putClause(const IntStack& cs, const IntStack& gs, const int neck) 
{
    int base = size();
    int b = tag(V, base);
    int len = cs.size();
	int* csArray = new int[cs.size()];
	for(int i=0; i<cs.size(); i++)	
		csArray[i] = cs.get(i);
    pushCells(b, 0, len, csArray);
	delete csArray;

	int* gsArray = new int[gs.size()];
	for(int i=0; i<gs.size(); i++)	
		gsArray[i] = gs.get(i);

	for(int i=0; i<gs.size(); i++)	
		gsArray[i] = relocate(b, gsArray[i]);

    int xsLength = 0;   
    int* xs = getIndexables(gsArray[0], xsLength);
		
    Clause c(len, gsArray, gs.size(), base, neck, xs, xsLength);
	delete gsArray;
	delete xs;
	return c;
}

/**
 * relocates a variable or array reference cell by b
 * assumes var/ref codes V,U,R are 0,1,2
 */
int Engine::relocate(const int b, const int cell) 
{
    return tagOf(cell) < 3 ? cell + b : cell;
}

/**
 * pushes slice[from,to] of array cs of cells to heap
 */
void Engine::pushCells(const int b, const int from, const int to, const int base) 
{
    ensureSize(to - from);
    for (int i = from; i < to; i++) 
	{
		push(relocate(b, heap[base + i]));
    }
}

/**
 * pushes slice[from,to] of array cs of cells to heap
 */
void Engine::pushCells(const int b, const int from, const int to, const int* cs) 
{
    ensureSize(to - from);
    for (int i = from; i < to; i++) 
	{
		push(relocate(b, cs[i]));
    }
}

/**
 * copies and relocates head of clause at offset from heap to heap
 */
int Engine::pushHead(const int b, const Clause& C) 
{
    pushCells(b, 0, C.neck, C.base);
    int head = C.hgs[0];
    return relocate(b, head);
}

/**
 * copies and relocates body of clause at offset from heap to heap
 * while also placing head as the first element of array gs that
 * when returned contains references to the toplevel spine of the clause
 */
void Engine::pushBody(const int b, const int head, Clause& C, int*& gs, int& gsLength)
{
    pushCells(b, C.neck, C.len, C.base);
    int l = C.hgsLength;
	gs = new int[l];
	gsLength = l;
    gs[0] = head;
    for (int k = 1; k < l; k++) 
	{
		int cell = C.hgs[k];
		gs[k] = relocate(b, cell);
    }    
}

/**
 * makes, if needed, registers associated to top goal of a Spine
 * these registers will be reused when matching with candidate clauses
 * note that xs contains dereferenced cells - this is done once for
 * each goal's toplevel subterms
 */
void Engine::makeIndexArgs(Spine* G, const int goal) 
{
	if (!G->xsNull)
		return;

	int p = 1 + detag(goal);
	int n = detag(getRef(goal)); 
	if (n>MAXIND)
		n = MAXIND;	
    

	for (int i = 0; i < n; i++) 
	{
		int cell = deref(heap[p + i]);
		G->xs[i] = cell2index(cell);                
	}
	G->xsNull = false;
	
	if (imaps_null)
		return;

	// LATER! 
	//int[] cs = IMap::get(imaps, vmaps, xs);
	//G->cs = cs;
}

int* Engine::getIndexables(const int ref, int& xsLength) 
{
	int p = 1 + detag(ref);
	int n = detag(getRef(ref));
	int* xs = new int[MAXIND];
	for (int i = 0; i < MAXIND; i++)	
	{
        	xs[i] = 0;
	}

	xsLength = MAXIND;

    	for (int i = 0; i < MAXIND && i < n; i++) 
	{
		int cell = deref(heap[p + i]);
		xs[i] = cell2index(cell);

	}
	return xs;
}

int Engine::cell2index(const int cell) 
{
	int x = 0;
	int t = tagOf(cell);
	switch (t) 
	{
		case R:
		        x = getRef(cell);
		break;
		case C:
		case N:
				x = cell;
		break;
		// 0 otherwise - assert: tagging with R,C,N <>0
	}
	return x;
 }



/**
 * raw display of a term - to be overridden
 */
std::string Engine::showTerm(const int x) 
{
	bool isArray = false;
	return exportTerm(x, isArray);
}

/**
 * builds an array of embedded arrays from a heap cell
 * representing a term for interaction with an external function
 * including a displayer
 */
std::string Engine::exportTerm(int x, bool& isArray) 
{
	std::stringstream res;

    x = deref(x);

    int t = tagOf(x);
    int w = detag(x);
    
    switch (t) 
	{
		case C:
			res << getSym(w);
			isArray = false;
		break;
		case N:
			res << w;
			isArray = false;
		break;
		case V:
			//case U:
			res << "V" << w;
			isArray = false;
		break;
		case R: 
			{
				bool internalIsArray = false;

				int a = heap[w];
				if (A != tagOf(a))
				{
					return "*** should be A, found=" + showCell(a);
				}
				int n = detag(a);
				//final Object[] arr = new Object[n];
				int k = w + 1;


				res << "[";
				for (int i = 0; i < n; i++) 
				{
					if (i>0)
						res << ",";

					int j = k + i;
					res << exportTerm(heap[j], internalIsArray);
				}
				res << "]";				
				isArray = true;
			}			      
		break;
		default:
			res << "*BAD TERM*" << showCell(x);
			isArray = false;
		break;
    }

    return res.str();
}




/**
 * raw display of a cell as tag : value
 */
std::string Engine::showCell(const int w) 
{
    int t = tagOf(w);
    int val = detag(w);
	std::stringstream ss;
    switch (t) 
	{
		case V:
			ss << "v:" << val;
		break;
		case U:
			ss << "u:" << val;
		break;
		case N:
			ss << "n:" << val;
			break;
		case C:
			ss << "c:" << getSym(val);
		break;
		case R:
			ss << "r:" << val;
		break;
		case A:
			ss << "a:" << val;
		break;
		default:
			ss << "*BAD*=" << w;
		break;
    }

    return ss.str();
}

/**
 * a displayer for cells
 */

std::string Engine::showCells(const int base, const int len) 
{
	std::stringstream buf;
    for (int k = 0; k < len; k++) 
	{
		int instr = heap[base + k];

		buf << "[" << (base + k) << "]";
		
		buf << showCell(instr);
		buf << " ";
    }
    return buf.str();
}

  /*
  String showCells(final int[] cs) {
    final StringBuffer buf = new StringBuffer();
    for (int k = 0; k < cs.length; k++) {
      buf.append("[" + k + "]");
      buf.append(showCell(cs[k]));
      buf.append(" ");
    }
    return buf.toString();
  }
  */

/**
 * unification algorithm for cells X1 and X2 on ustack that also takes care
 * to trail bindigs below a given heap address "base"
 */
bool Engine::unify(const int base) 
{
    while (!ustack.isEmpty()) 
	{
		int x1 = deref(ustack.pop());
		int x2 = deref(ustack.pop());
		if (x1 != x2) 
		{
			int t1 = tagOf(x1);
			int t2 = tagOf(x2);
			int w1 = detag(x1);
			int w2 = detag(x2);

			if (isVAR(x1)) 
			{ /* unb. var. v1 */
				if (isVAR(x2) && w2 > w1) 
				{ /* unb. var. v2 */
					heap[w2] = x1;
					if (w2 <= base) 
					{
						trail.push(x2);
					}
				} 
				else 
				{ // x2 nonvar or older
					heap[w1] = x2;
					if (w1 <= base) 
					{
						trail.push(x1);
					}
				}
			} 
			else if (isVAR(x2)) 
			{ 
				/* x1 is NONVAR */
				heap[w2] = x1;
				if (w2 <= base) 
				{
					trail.push(x2);
				}
			} 
			else if (R == t1 && R == t2) 
			{ 
				// both should be R
				if (!unify_args(w1, w2))
					return false;
			} else
				return false;
		}
    }
    return true;
}

bool Engine::unify_args(const int w1, const int w2) 
{
    int v1 = heap[w1];
    int v2 = heap[w2];
    // both should be A
    int n1 = detag(v1);
    int n2 = detag(v2);
    if (n1 != n2)
		return false;
    int b1 = 1 + w1;
    int b2 = 1 + w2;
    for (int i = n1 - 1; i >= 0; i--) 
	{      
		int i1 = b1 + i;
		int i2 = b2 + i;
		int u1 = heap[i1];
		int u2 = heap[i2];
		if (u1 == u2) 
		{
			continue;
		}
		ustack.push(u2);
		ustack.push(u1);
    }
    return true;
}

/**
 * tests if the head of a clause, not yet copied to the heap
 * for execution could possibly match the current goal, an
 * abstraction of which has been place in xs
 */
bool Engine::match(const int* xs, const Clause& C0) 
{
    for (int i = 0; i < MAXIND; i++) 
	{
		int x = xs[i];
		int y = C0.xs[i];
		if (0 == x || 0 == y) 
		{
			continue;
		}

		if (x != y) 
		{		
			return false;
		}
    }
    return true;
}

/**
 * transforms a spine containing references to choice point and
 * immutable list of goals into a new spine, by reducing the
 * first goal in the list with a clause that successfully
 * unifies with it - in which case places the goals of the
 * clause at the top of the new list of goals, in reverse order
 */
Spine* Engine::unfold(Spine* G) 
{
	int ttop = trail.getTop();
	int htop = getTop();
	int base = htop + 1;
    
	int goal = IntList_head(G->gs);

	makeIndexArgs(G, goal);
    
	int last = G->csLength();
        
	for (int k = G->k; k < last; k++) 
	{      
		Clause& C0 = clauses[G->cs(k)];

		if (!match(G->xs, C0)) {
			continue;
		}

		int base0 = base - C0.base;
		int b = tag(V, base0);
		int head = pushHead(b, C0);

		ustack.clear(); // set up unification stack

		ustack.push(head);
		ustack.push(goal);

		if (!unify(base)) 
		{
			unwindTrail(ttop);
			setTop(htop);
			continue;
		}

		int* gs = NULL;
		int gsLength = 0;
		pushBody(b, head, C0, gs, gsLength);
		std::vector<int> newgs = IntList_tail(IntList_app(gs, gsLength, IntList_tail(G->gs)));
		G->k = k + 1;                
		if (newgs.size()>0)
		{
			std::vector<int> v = IntList_tail(G->gs);
			Spine* spine = new Spine(gs, gsLength, base, v, ttop, 0, &cls);
			//IntList::deleteList(newgs);
			delete gs;
			return spine;
		}
		else {
			delete gs;
			//IntList::deleteList(newgs);
			return answer(ttop);
		}
	} // end for
	return NULL;
}

/**
 * extracts a query - by convention of the form
 * goal(Vars):-body to be executed by the engine
 */
Clause& Engine::getQuery() 
{
	return clauses[clauses.size() - 1];
}

/**
 * returns the initial spine built from the
 * query from which execution starts
 */
Spine* Engine::init() 
{
	int base = size();

	Clause& G = getQuery();
	std::vector<int> empty;
	Spine* Q = new Spine(G.hgs, G.hgsLength, base, empty, trail.getTop(), 0, &cls);
	spines.push_back(Q);
	return Q;
}

/**
* returns an answer as a Spine while recording in it
* the top of the trail to allow the caller to retrieve
* more answers by forcing backtracking
*/
Spine* Engine::answer(int ttop) 
{
	int hd = spines.front()->hd; // SHOULD BE spines.get(0).hd
	return new Spine(hd, ttop);
}

/**
 * detects availability of alternative clauses for the
 * top goal of this spine
 */
bool Engine::hasClauses(Spine* S) 
{	
    return S->k < S->csLength();
}

/**
 * true when there are no more goals left to solve
 */
bool Engine::hasGoals(Spine* S) 
{	
	return (S->gs.size()>0);
}

/**
 * removes this spines for the spine stack and
 * resets trail and heap to where they where at its
 * creating time - while undoing variable binding
 * up to that point
 */
void Engine::popSpine() 
{    
	Spine* G = spines.back();
	spines.pop_back();
    unwindTrail(G->ttop);
    setTop(G->base - 1);
	delete G;
}

/**
 * main interpreter loop: starts from a spine and works
 * though a stream of answers, returned to the caller one
 * at a time, until the spines stack is empty - when it
 * returns null
 */
Spine* Engine::yield() 
{
	Spine* G = NULL;        
	while (!spines.empty()) 
	{
		G = spines.back();
		if (!hasClauses(G)) 
		{
			popSpine(); // no clauses left
			//delete G;
			continue;
		}

		Spine* C = unfold(G);
		if (!C) 
		{
			popSpine(); // no matches
			//delete G;			
			continue;
		}
		if (hasGoals(C)) 
		{
			spines.push_back(C);			
			// delete G;
			continue;
		}
		return C; // answer
	}    
	return NULL;
}

/**
 * retrieves an answers and ensure the engine can be resumed
 * by unwinding the trail of the query Spine
 * returns an external "human readable" representation of the answer
 */
std::string Engine::ask(bool& isArray, bool& isNull) 
{
	query = yield();
	if (!query)
	{
		isNull = true;
		return "";
	}
	Spine* s = answer(query->ttop);
	int res = s->hd;

	//final Object R = exportTerm(res);	
	std::string rr = exportTerm(res, isArray);
	isNull = false;
		
	unwindTrail(query->ttop);
    
	delete s;
	return rr;
}

/**
 * initiator and consumer of the stream of answers
 * generated by this engine
 */
void Engine::run() 
{
	long ctr = 0L;
	for (;; ctr++) 
	{
		bool isArray = false;
		bool isNull = false;
		std::string A = ask(isArray, isNull);
		if (isNull) 
		{
			break;
		}

		if (ctr<5) 
		{
			std::stringstream ss;
			ss << "[" << ctr << "] " << "*** ANSWER=" << A;
			pp(ss.str());
		}
	}

	if(ctr>5) 
		pp("...");

	std::stringstream ss;
	ss << "TOTAL ANSWERS=" << ctr;
	pp(ss.str());
}

void Engine::put(IMap<int> imaps[], IntMap vss[], int keys[], int val)
{
	for (int i = 0; i < MAXIND; i++) 
	{
		int key = keys[i];

		if (key != 0) 
		{
			imaps[i].put(key, val);
		}
		else 
		{
			vss[i].addKey(val);
		}
    }
}

void Engine::index(std::vector<Clause>& clauses, IntMap vmaps[])
{
	if (clauses.size() < START_INDEX)
		return;

	imaps_null = false;

	for (int i = 0; i < (int)(clauses.size()); i++) 
	{
		Clause& c = clauses[i];
		put(imaps, vmaps, c.xs, i + 1); // $$$ UGLY INC
	}

	pp("INDEX");
	//pp(IMap.show(imaps));
	//pp(Arrays.toString(vmaps));
	pp("");
}
Engine.cpp
Open with Anyfile Notepad
Displaying Engine.cpp.
