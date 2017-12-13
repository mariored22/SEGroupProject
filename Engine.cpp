#include "Engine.h"
#pragma warning(disable: 4996)

string Object::tostring()
{
	return "Object::tostring()";
}

	ObjectArray::ObjectArray(vector<Object *> arr)
	{
		this->arr = arr;
		instanceOf = OBJECT_ARRAY;
	}

	string ObjectArray::tostring()
	{
		string s = arr[0]->tostring();
		s += "(";
		if (arr.size() > 1) s += arr[1]->tostring();
		for (int i = 2;i < arr.size();i++)
		{
			s += ", " + arr[i]->tostring();
		}

		s += ")";

		return s; //"OBJECT_ARRAY";
	}


	ObjectInteger::ObjectInteger(int x)
	{
		this->x = x;
		instanceOf = OBJECT_INTEGER;
	}

	string ObjectInteger::tostring()
	{
		char tmp[50];
		sprintf(tmp, "%d", x);
		string s(tmp);
		return s;
	}

	ObjectString::ObjectString(string x)
	{
		this->x = x;
		instanceOf = OBJECT_STRING;
	}

	string ObjectString::tostring()
	{
		return x;
	}


	Engine::Engine( string fname) {

   /// syms = new map<string, int>();
	//syms_list = new list<string>;
    //slist = new vector<string>();
	//spines = new ObStack<Spine>();

    makeHeap();


    //trail = new IntStack();
    //ustack = new IntStack();

    clauses = dload(fname);

    cls = toNums(*clauses);

    query = init();

    vmaps = vcreate(3);
    imaps = index(clauses, vmaps);


//	vector<Clause *> clauses, vector<IntMap *> vmaps
  }

    int Engine::tag( int t,  int w) {
    return -((w << 3) + t);
  }

     int Engine::detag( int w) {
    return -w >> 3;
  }

 int Engine::tagOf( int w) {
    return -w & 7;
  }


    int Engine::addSym( string sym) {
		int I;
		if (syms.find(sym) == syms.end()){
				//    int I = syms.get(sym);
				  //  if (NULL == I) {
			I = syms.size();
			syms[sym] = I;
			syms_list.push_back(sym);
			slist.push_back(sym);
     //  int i = syms.size();
    //  I = new Integer(i);
     // syms.put(sym, I);
    //  slist.add(sym);
    }
		else
		{
			I = syms[sym];
		}
		return I; //I.intValue();
  }

  /**
   * returns the symbol associated to an integer index
   * in the symbol table
   */
    string Engine::getSym( int w) {
    if (w < 0 || w >= slist.size())
      return "BADSYMREF=" + w;
	return slist[w]; //.get(w);
  }

    void Engine::makeHeap() {
    makeHeap(MINSIZE);
  }

    void Engine::makeHeap( int size) {
		heap.resize(size); // = new int[size];
    clear();
  }

    int Engine::getTop() {
    return top;
  }

    int Engine::setTop( int top) {
    return this->top = top;
  }

    void Engine::clear() {
    //for (int i = 0; i <= top; i++)
    //heap[i] = 0;
    top = -1;
  }

  /**
   * Pushes an element - top is incremented frirst than the
   * element is assigned. This means top point to the last assigned
   * element - which can be returned with peek().
   */
    void Engine::push( int i) {
    heap[++top] = i;
  }

   int Engine::size() {
    return top + 1;
  }

  /**
   * dynamic array operation: doubles when full
   */
    void Engine::expand() {
     int l = heap.size();
	 vector<int> newstack(l << 1);
//     int[] newstack = new int[l << 1];

	 for (int i = 0;i <l ;i++)
		 newstack[i] = heap[i];

   // System.arraycopy(heap, 0, newstack, 0, l);
    heap = newstack;
  }

   void Engine::ensureSize( int more) {
    if (1 + top + more >= heap.size()) {
      expand();
    }
  }

  /**
  * expands a "Xs lists .." statements to "Xs holds" statements
  */

     vector<vector<string> > * Engine::maybeExpand( vector<string> Ws) {
		string W = Ws[0];
    if (W.length() < 2 ||  W.substr(0, 2)!= "l:")
      return NULL;

     int l = Ws.size();
	 vector<vector<string> > * Rss = new vector<vector<string> >();
     string V = W.substr(2);
    for (int i = 1; i < l; i++) {
		vector<string> Rs(4);
//       string[] Rs = new string[4];

		string Vi = V;

		if (i != 1)
		{
			char tmp[64];
			sprintf(tmp, "%s__%d", V.c_str(), i - 1);
			Vi = tmp;
		}

		char tmp[64];
		sprintf(tmp, "%s__%d", V.c_str(), i);
		string Vii = tmp;

     ////  string Vi = 1 == i ? V : V + "__" + (i - 1);
     //  string Vii = V + "__" + i;
      Rs[0] = "h:" + Vi;
      Rs[1] = "c:list";
	  Rs[2] = Ws[i]; // get(i);
      Rs[3] = i == l - 1 ? "c:nil" : "v:" + Vii;
      Rss->push_back(Rs);
    }
    return Rss;

  }

  /**
   * expands, if needed, "lists" statements in sequence of statements
   */
     vector<vector<string> > * Engine::mapExpand( vector<vector<string> *>* Wss) {
		vector<vector<string> > *Rss = new vector<vector<string> >();; // = new ArrayList<string[]>();
    for ( vector<string>* Ws : *Wss) {

       vector<vector<string> > * Hss = maybeExpand(*Ws);

      if (NULL == Hss) {
		  vector<string> ws(Ws->size()); // = new string[Ws.size()];
        for (int i = 0; i < ws.size(); i++) {
			ws[i] = (*Ws)[i]; //.get(i);
        }
        Rss->push_back(ws);
      } else {
        for ( vector<string> X : *Hss) {
          Rss->push_back(X);
        }
		delete Hss;
      }
    }
    return Rss;
  }

  /**
   * loads a program from a .nl file of
   * "natural language" equivalents of Prolog/HiLog statements
   */
  vector<Clause *> * Engine::dload( string s) {
     bool fromFile = true;
	 vector<vector<vector<string > *> *> Wsss = Toks::toSentences(s, fromFile);
//     ArrayList<ArrayList<ArrayList<string>>> Wsss = Toks.toSentences(s, fromFile);

	 vector<Clause *> *Cs = new vector<Clause *>();

//     ArrayList<Clause> Cs = new ArrayList<Clause>();

    for ( vector<vector<string> *>* Wss : Wsss) {
      // clause starts here

		map<string, IntStack*> refs; // = new map<string, IntStack>();
		list<string> refs_list;

//       LinkedHashMap<string, IntStack> refs = new LinkedHashMap<string, IntStack>();
		IntStack cs;// = new IntStack();
		IntStack gs;// = new IntStack();

       vector<vector<string > > * Rss = mapExpand(Wss);
      int k = 0;
      for ( vector<string> ws : *Rss) {

        // head or body element starts here

         int l = ws.size();
        gs.push(tag(R, k++));
        cs.push(tag(A, l));

        for (string w : ws) {

          // head or body subterm starts here

			if (w == "c:list")
				w = w;

          if (1 == w.length()) {
            w = "c:" + w;
          }

           string L = w.substr(2);

          switch (w[0]){ // .charAt(0)) {
            case 'c':
              cs.push(encode(C, L));
              k++;
            break;
            case 'n':
              cs.push(encode(N, L));
              k++;
            break;
            case 'v': {
				IntStack * Is = refs[L];//.get(L);
              if (NULL == Is) {
                Is = new IntStack();
				refs[L] = Is; //.put(L, Is);
				refs_list.push_back(L);
              }
              Is->push(k);

              cs.push(tag(BAD, k)); // just in case we miss this
              k++;
            }
            break;
            case 'h': {
				IntStack *Is = refs[L]; //get(L);
              if (NULL == Is) {
                Is = new IntStack();
				refs[L] = Is; //.put(L, Is);
				refs_list.push_back(L);
              }
              Is->push(k - 1);
              cs.set(k - 1, tag(A, l - 1));
              gs.pop();
            }
            break;
            default:
				printf("FORGOTTEN=%s\n", w.c_str());
//              Main.pp("FORGOTTEN=" + w);
          } // end subterm
        } // end element
      } // end clause

	  delete Rss;

      // linker
       //Iterator<IntStack> K = refs.values().iterator();

	  list<string>::iterator k_it = refs_list.begin();

      while (k_it!=refs_list.end()) {

		  IntStack *Is = refs[*k_it];
		  k_it++;

        // IntStack Is = K.next();

        // finding the A among refs
        int leader = -1;
        for ( int j : Is->toArray()) {
          if (A == tagOf(cs.get(j))) {
            leader = j;

            break;
          }
        }
        if (-1 == leader) {
          // for vars, first V others U
          leader = Is->get(0);
          for ( int i : Is->toArray()) {
            if (i == leader) {
              cs.set(i, tag(V, i));
            } else {
              cs.set(i, tag(U, leader));
            }

          }
        } else {
          for ( int i : Is->toArray()) {
            if (i == leader) {
              continue;
            }
            cs.set(i, tag(R, leader));
          }
        }
      }

       int neck = 1 == gs.size() ? cs.size() : detag(gs.get(1));
       vector<int> tgs = gs.toArray();

       Clause *C = putClause(cs.toArray(), tgs, neck);

      Cs->push_back(C);

    } // end clause set

     int ccount = Cs->size();
	 vector<Clause *> * cls = new vector<Clause *>(ccount);
//     Clause[] cls = new Clause[ccount];
    for (int i = 0; i < ccount; i++) {
		(*cls)[i] = (*Cs)[i]; //.get(i);
    }
    return cls;
  }

    vector<int> * Engine::toNums( vector<Clause *> clauses) {
     int l = clauses.size();
	 vector<int> * cls = new vector<int>(l);
     //int[] cls = new int[l];
    for (int i = 0; i < l; i++) {
      (*cls)[i] = i;
    }
    return cls;
  }

  /*
   * encodes string constants into symbols while leaving
   * other data types untouched
   */
    int Engine::encode( int t,  string s) {
    int w;

	bool aNumber = true;

	for (char c : s)
	{
		if (!isdigit(c))
		{
			aNumber = false;
			if (C == t) w = addSym(s);
			else
				return tag(BAD, 666);
			break;
		}
	}
	if(aNumber)
	w = atoi(s.c_str()); //Integer.parseInt(s);
	return tag(t, w);

	/*
	try {
		w = atoi(s.c_str()); //Integer.parseInt(s);
    } catch ( Exception e) {
      if (C == t) {
        w = addSym(s);
      } else
        //pp("bad in encode=" + t + ":" + s);
        return tag(BAD, 666);
    }
    return tag(t, w);
	*/
  }

  /**
   * true if cell x is a variable
   * assumes that variables are tagged with 0 or 1
   */
     bool Engine::isVAR( int x) {
    // int t = tagOf(x);
    //return V == t || U == t;
    return tagOf(x) < 2;
  }

  /**
   * returns the heap cell another cell points to
   */
   int Engine::getRef( int x) {
    return heap[detag(x)];
  }

  /*
   * sets a heap cell to point to another one
   */
    void Engine::setRef( int w,  int r) {
    heap[detag(w)] = r;
  }

  /**
   * removes binding for variable cells
   * above savedTop
   */
   void Engine::unwindTrail( int savedTop) {
    while (savedTop < trail.getTop()) {
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
    int Engine::deref(int x) {
    while (isVAR(x)) {
       int r = getRef(x);
      if (r == x) {
        break;
      }
      x = r;
    }
    return x;
  }

  /**
   * raw display of a term - to be overridden
   */
  string Engine::showTerm( int x) {
    return showTerm(exportTerm(x));
  }

  /**
   * raw display of a externalized term
   */
  string Engine::showTerm( Object *O) {
	  return O->tostring();
	  /*
    if (O->instanceOf == OBJECT_ARRAY) // instanceof Object[])
      return Arrays.deepTostring((Object[]) O);
    return O.tostring();
	*/
  }

  /**
   * prints out content of the trail
   */
  void Engine::ppTrail() {
    for (int i = 0; i <= trail.getTop(); i++) {
       int t = trail.get(i);
	   printf("trail[%d]=%s:%s\n", showCell(t).c_str(), showTerm(t).c_str());
//      Main.pp("trail[" + i + "]=" + showCell(t) + ":" + showTerm(t));
    }
  }

  /**
   * builds an array of embedded arrays from a heap cell
   * representing a term for interaction with an external function
   * including a displayer
   */
  Object *Engine::exportTerm(int x) {
    x = deref(x);

     int t = tagOf(x);
     int w = detag(x);

    Object *res = NULL;
    switch (t) {
      case C:
        res = new ObjectString(getSym(w));
      break;
      case N:
        res = new ObjectInteger(w);
      break;
      case V:
        //case U:
	  {
		  char tmp[64];
		  sprintf(tmp, "V%d", w);
		  res = new ObjectString(string(tmp));

	  }
      break;
      case R: {

         int a = heap[w];
        if (A != tagOf(a))
          return new ObjectString("*** should be A, found=" + showCell(a));
         int n = detag(a);

		 vector<Object *> arr(n);

       //  Object[] arr = new Object[n];
         int k = w + 1;
        for (int i = 0; i < n; i++) {
           int j = k + i;
          arr[i] = exportTerm(heap[j]);
        }
		res = new ObjectArray(arr);
		//res = arr;
      }
      break;
      default:
        res = new ObjectString("*BAD TERM*" + showCell(x));
    }
    return res;
  }

  /**
   * extracts an integer array pointing to
   * the skeleton of a clause: a cell
   * pointing to its head followed by cells pointing to its body's
   * goals
   */
   vector<int> * Engine::getSpine( vector<int> cs) {
     int a = cs[1];
     int w = detag(a);
	 vector<int>* rs = new vector<int>(w - 1);
//     int[] rs = new int[w - 1];
    for (int i = 0; i < w - 1; i++) {
       int x = cs[3 + i];
       int t = tagOf(x);
      if (R != t) {
        printf("*** getSpine: unexpected tag=%d\n", t);
        return NULL;
      }
      (*rs)[i] = detag(x);
    }
    return rs;
  }

  /**
   * raw display of a cell as tag : value
   */
   string Engine::showCell( int w) {
     int t = tagOf(w);
     int vl = detag(w);

	 char tmp[20];
	 sprintf(tmp, "%d", vl);
	 string val(tmp);


    string s = "";
    switch (t) {
      case V:
        s = "v:" + val;
      break;
      case U:
        s = "u:" + val;
      break;
      case N:
        s = "n:" + val;
      break;
      case C:
        s = "c:" + getSym(vl);
      break;
      case R:
        s = "r:" + val;
      break;
      case A:
        s = "a:" + val;
      break;
      default:
        s = "*BAD*=" + w;
    }
    return s;
  }

  /**
   * a displayer for cells
   */

  string Engine::showCells( int base,  int len) {
	  string buf = "";//new stringBuffer();
    for (int k = 0; k < len; k++) {
       int instr = heap[base + k];

	   char tmp[64];
	   sprintf(tmp, "[%d]", base+k);
	   buf += tmp;
	   buf += showCell(instr);
	   buf += " ";

     // buf.append("[" + (base + k) + "]");
     // buf.append(showCell(instr));
     // buf.append(" ");
    }
	return buf; //.tostring();
  }

  string Engine::showCells( vector<int> cs) {
	  string buff = "";
   //  stringBuffer buf = new stringBuffer();
    for (int k = 0; k < cs.size(); k++) {
		char tmp[64];
		sprintf(tmp, "[%d]", k);
		buff += tmp;
		buff +=  showCell(cs[k]);
		buff += " ";
     // buf.append("[" + k + "]");
    //  buf.append(showCell(cs[k]));
    //  buf.append(" ");
    }
    return buff;
  }

  /**
  * to be overridden as a printer of a spine
  */
  void Engine::ppc( Spine C) {
    // override
  }

  /**
   * to be overridden as a printer for current goals
   * in a spine
   */
  void Engine::ppGoals( IntList gs) {
    // override
  }

  /**
   * to be overriden as a printer for spines
   */
  void Engine::ppSpines() {
    // override
  }

  /**
   * unification algorithm for cells X1 and X2 on ustack that also takes care
   * to trail bindigs below a given heap address "base"
   */
    bool Engine::unify( int base) {
    while (!ustack.isEmpty()) {
       int x1 = deref(ustack.pop());
       int x2 = deref(ustack.pop());
      if (x1 != x2) {
         int t1 = tagOf(x1);
         int t2 = tagOf(x2);
         int w1 = detag(x1);
         int w2 = detag(x2);

        if (isVAR(x1)) { /* unb. var. v1 */
          if (isVAR(x2) && w2 > w1) { /* unb. var. v2 */
            heap[w2] = x1;
            if (w2 <= base) {
              trail.push(x2);
            }
          } else { // x2 nonvar or older
            heap[w1] = x2;
            if (w1 <= base) {
              trail.push(x1);
            }
          }
        } else if (isVAR(x2)) { /* x1 is NONVAR */
          heap[w2] = x1;
          if (w2 <= base) {
            trail.push(x2);
          }
        } else if (R == t1 && R == t2) { // both should be R
          if (!unify_args(w1, w2))
            return false;
        } else
          return false;
      }
    }
    return true;
  }

    bool Engine::unify_args( int w1,  int w2) {
     int v1 = heap[w1];
     int v2 = heap[w2];
    // both should be A
     int n1 = detag(v1);
     int n2 = detag(v2);
    if (n1 != n2)
      return false;
     int b1 = 1 + w1;
     int b2 = 1 + w2;
    for (int i = n1 - 1; i >= 0; i--) {
       int i1 = b1 + i;
       int i2 = b2 + i;
       int u1 = heap[i1];
       int u2 = heap[i2];
      if (u1 == u2) {
        continue;
      }
      ustack.push(u2);
      ustack.push(u1);
    }
    return true;
  }

  /**
   * places a clause built by the Toks reader on the heap
   */
  Clause * Engine::putClause( vector<int> cs, vector<int> gs, int neck) {
     int base = size();
     int b = tag(V, base);
     int len = cs.size();
    pushCells(b, 0, len, cs);
    for (int i = 0; i < gs.size(); i++) {
      gs[i] = relocate(b, gs[i]);
    }
	vector <int> * xs = getIndexables(gs[0]);
    return new Clause(len, gs, base, neck, *xs);
  }

  /**
   * relocates a variable or array reference cell by b
   * assumes var/ref codes V,U,R are 0,1,2
   */
    int Engine::relocate( int b,  int cell) {
    return tagOf(cell) < 3 ? cell + b : cell;
  }

  /**
   * pushes slice[from,to] of array cs of cells to heap
   */
    void Engine::pushCells( int b,  int from,  int to,  int base) {
    ensureSize(to - from);
    for (int i = from; i < to; i++) {
      push(relocate(b, heap[base + i]));
    }
  }

  /**
   * pushes slice[from,to] of array cs of cells to heap
   */
    void Engine::pushCells( int b,  int from,  int to,  vector<int> cs) {
    ensureSize(to - from);
    for (int i = from; i < to; i++) {
      push(relocate(b, cs[i]));
    }
  }

  /**
   * copies and relocates head of clause at offset from heap to heap
   */
    int Engine::pushHead( int b,  Clause *C) {
    pushCells(b, 0, C->neck, C->base);
     int head = C->hgs[0];
    return relocate(b, head);
  }

  /**
   * copies and relocates body of clause at offset from heap to heap
   * while also placing head as the first element of array gs that
   * when returned contains references to the toplevel spine of the clause
   */
    vector<int> Engine::pushBody( int b,  int head,  Clause * C) {
    pushCells(b, C->neck, C->len, C->base);
     int l = C->hgs.size();
	 vector<int> gs(l);
//     int[] gs = new int[l];
    gs[0] = head;
    for (int k = 1; k < l; k++) {
       int cell = C->hgs[k];
      gs[k] = relocate(b, cell);
    }
    return gs;
  }

  /**
   * makes, if needed, registers associated to top goal of a Spine
   * these registers will be reused when matching with candidate clauses
   * note that xs contains dereferenced cells - this is done once for
   * each goal's toplevel subterms
   */
    void Engine::makeIndexArgs( Spine *G,  int goal) {
    if (NULL!= G->xs)
      return;

     int p = 1 + detag(goal);
     int n = min(3, detag(getRef(goal)));

	 vector<int> *xs = new vector<int>(3); //[] xs = new int[MAXIND];

    for (int i = 0; i < n; i++) {
       int cell = deref(heap[p + i]);
      (*xs)[i] = cell2index(cell);
    }

	//if (G->xs != NULL)
	//	G = G;
    G->xs = xs;

    if (NULL == imaps)
      return;
	vector<int> *cs = IMap<int>::get(imaps, vmaps, xs);
    G->cs = cs;
  }

    vector<int> * Engine::getIndexables( int ref) {
     int p = 1 + detag(ref);
     int n = detag(getRef(ref));
	 vector<int> * xs = new vector<int>(3);
//     int[] xs = new int[MAXIND];
    for (int i = 0; i < 3 && i < n; i++) {
       int cell = deref(heap[p + i]);
      (*xs)[i] = cell2index(cell);
    }
    return xs;
  }

    int Engine::cell2index( int cell) {
    int x = 0;
     int t = tagOf(cell);
    switch (t) {
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
   * tests if the head of a clause, not yet copied to the heap
   * for execution could possibly match the current goal, an
   * abstraction of which has been place in xs
   */
    bool Engine::match( vector<int>* xs,  Clause *C0) {
    for (int i = 0; i < 3; i++) {
       int x = (*xs)[i];
       int y = C0->xs[i];
      if (0 == x || 0 == y) {
        continue;
      }
      if (x != y)
        return false;
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
    Spine * Engine::unfold( Spine * G) {

     int ttop = trail.getTop();
     int htop = getTop();
     int base = htop + 1;

     int goal = IntList::head(G->gs);

    makeIndexArgs(G, goal);

     int last = G->cs->size();
    for (int k = G->k; k < last; k++) {
		Clause *C0 = (*clauses)[(*G->cs)[k]];

      if (!match(G->xs, C0))
        continue;

       int base0 = base - C0->base;
       int b = tag(V, base0);
       int head = pushHead(b, C0);

      ustack.clear(); // set up unification stack

      ustack.push(head);
      ustack.push(goal);

      if (!unify(base)) {
        unwindTrail(ttop);
        setTop(htop);
        continue;
      }
	  vector <int> gs = pushBody(b, head, C0);
       IntList *newgs = IntList::tail(IntList::app(gs, IntList::tail(G->gs)));
      G->k = k + 1;
      if (!IntList::isEmpty(newgs))
        return new Spine(gs, base, IntList::tail(G->gs), ttop, 0, *cls);
      else
        return answer(ttop);
    } // end for
    return NULL;
  }

  /**
   * extracts a query - by convention of the form
   * goal(Vars):-body to be executed by the engine
   */
  Clause * Engine::getQuery() {
    return (*clauses)[clauses->size() - 1];
  }

  /**
   * returns the initial spine built from the
   * query from which execution starts
   */
  Spine * Engine::init() {
     int base = size();

     Clause *G = getQuery();
     Spine *Q = new Spine(G->hgs, base, NULL, trail.getTop(), 0, *cls);
    spines.push(Q);
    return Q;
  }

  /**
   * returns an answer as a Spine while recording in it
   * the top of the trail to allow the caller to retrieve
   * more answers by forcing backtracking
   */
    Spine * Engine::answer( int ttop) {
    return new Spine(spines.get(0)->hd, ttop);
  }

  /**
   * detects availability of alternative clauses for the
   * top goal of this spine
   */
    bool Engine::hasClauses( Spine* S) {
    return S->k < S->cs->size();
  }

  /**
   * true when there are no more goals left to solve
   */
    bool Engine::hasGoals( Spine *S) {
    return !IntList::isEmpty(S->gs);
  }

  /**
   * removes this spines for the spine stack and
   * resets trail and heap to where they where at its
   * creating time - while undoing variable binding
   * up to that point
   */
    void Engine::popSpine() {
		Spine *pop = spines.peek();
		//delete pop;
     Spine *G = spines.pop();
    unwindTrail(G->ttop);
    setTop(G->base - 1);
	delete pop;
  }

	int cc = 0;

  /**
   * main interpreter loop: starts from a spine and works
   * though a stream of answers, returned to the caller one
   * at a time, until the spines stack is empty - when it
   * returns null
   */
   Spine * Engine::yield() {
    while (!spines.isEmpty()) {
       Spine *G = spines.peek();
	   
	   cc++;
	   if((cc%1000)==0)
	   printf("%d: %d %d %d %d\n", cc,G->base, G->hd, G->k, G->ttop);

      if (!hasClauses(G)) {
        popSpine(); // no clauses left
        continue;
      }
       Spine *C = unfold(G);
      if (NULL == C) {
        popSpine(); // no matches
        continue;
      }
      if (hasGoals(C)) {
        spines.push(C);
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
  Object * Engine::ask() {
    query = yield();
    if (NULL == query)
      return NULL;
	Spine * ans = answer(query->ttop);
     int res = ans->hd;
	 delete ans;
     Object *R = exportTerm(res);
    unwindTrail(query->ttop);
    return R;
  }

 

  /**
   * initiator and consumer of the stream of answers
   * generated by this engine
   */
  void Engine::run() {
    long ctr = 0L;
    for (;; ctr++) {
		if ((ctr % 1000) == 0)printf("%d\n", ctr);
       Object *A = ask();
      if (NULL == A) {
        break;
      }
	  if (ctr < 5) printf("[%d] *** ANSWER=%s\n", ctr, showTerm(A).c_str());
	//  else if(ctr==100)break;
		  //Prog.println("[" + ctr + "] " + "*** ANSWER=" + showTerm(A));
    }
    if(ctr>5) printf("...\n");
	printf("TOTAL ANSWERS=%d\n", ctr);
//    Prog.println("TOTAL ANSWERS=" + ctr);
  }

  // indexing extensions - ony active if START_INDEX clauses or more

    vector<IntMap *> * Engine::vcreate( int l) {
	   vector<IntMap *> * vss = new vector<IntMap *>(l); //IntMap[l];
    for (int i = 0; i < l; i++) {
      (*vss)[i] = new IntMap();
    }
    return vss;
  }
	
    void Engine::put( vector<IMap<int> *>  *imaps,  vector<IntMap*> * vss,  vector<int> keys,  int val) {
    for (int i = 0; i < imaps->size(); i++) {
       int key = keys[i];
      if (key != 0) {
        IMap<int>::put(imaps, i, key, val);
      } else {
        (*vss)[i]->add(val);
      }
    }
  }

   vector<IMap<int> *> * Engine::index( vector<Clause *> *clauses, vector<IntMap *> * vmaps) { //Clause[] clauses,  IntMap[] vmaps) {
    if (clauses->size() < START_INDEX)
      return NULL;

     vector<IMap<int> *> * imaps = IMap<int>::create(vmaps->size());
    for (int i = 0; i < clauses->size(); i++) {
       Clause *c = (*clauses)[i];

      put(imaps, vmaps, c->xs, i + 1); // $$$ UGLY INC

    }
	printf("INDEX\n");

	// TODO... print

	/**
    Main.pp("INDEX");
    Main.pp(IMap.show(imaps));
    Main.pp(Arrays.tostring(vmaps));
    Main.pp("");
	*/
    return imaps;
  }



   string Engine::showClause(Clause *s) {

	   char tmp[128];

	   string buf = "";
	   int l = s->hgs.size();

	   sprintf(tmp, "---base:[%d] neck: %d----\n", s->base, s->neck);
	   buf += tmp;
	   buf += showCells(s->base, s->len);
	   buf += "\n";
	   buf += showCell(s->hgs[0]);
	  // buf.append("---base:[" + s.base + "] neck: " + s.neck + "-----\n");
	  // buf.append(showCells(s.base, s.len)); // TODO
	  // buf.append("\n");
	 //  buf.append(showCell(s.hgs[0]));

	   buf += " :- [";

	  // buf.append(" :- [");
	   for (int i = 1; i < l; i++) {

		   int e = s->hgs[i];
		   buf += showCell(e);
		   if (i < l - 1) {
			   buf += ", ";
//			   buf.append(", ");
		   }
	   }

	   buf += "]\n";

	//   buf.append("]\n");
	   buf += showTerm(s->hgs[0]);

	  // buf.append(showTerm(s.hgs[0]));
	   if (l > 1) {
		  // buf.append(" :- \n");
		   buf += " :- \n";
		   for (int i = 1; i < l; i++) {
			   int e = s->hgs[i];
			   buf += " ";
			   buf += showTerm(e);
			   buf += "\n";
			  // buf.append("  ");
			  // buf.append(showTerm(e));
			  // buf.append("\n");
		   }
	   }
	   else {
		   buf.append("\n");
	   }
	   return buf; //.toString();
   }


   void Engine::ppCode() {
	   
	   printf("\nSYMS:\n");
	   //pp(syms);
	   printf("{");
	   bool first = true;
	   for (string x : syms_list)
	   {
		   if (first)
		   {
			   first = false;
		   }
		   else
		   {
			   printf(", ");
		   }
		   printf("%s=%d", x.c_str(), syms[x]);
	   }
	   printf("}\n");
	   // {eq=0, sel=1, list=2, perm=3, nil=4, app=5, nrev=6, input=7, goal=8}
	   printf("\nCLAUSES:\n\n");

	   for (int i = 0; i < clauses->size(); i++) {

		   Clause *C = (*clauses)[i];
		   printf("[%d]:%s\n", i, showClause(C).c_str());
	   }
	   printf("\n");	   
   }
