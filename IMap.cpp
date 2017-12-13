#include "IMap.h"

#pragma warning(disable: 4996)

template <class K>
  IMap<K>::IMap() {
	//  map_; // = new map<K, IntMap*>();
  }

  template <class K>
  void IMap<K>::clear() {
    map_.clear();
  }

  template <class K>
  bool IMap<K>::put( K key,  int val) {
	   IntMap *vals = map_[key];//.get(key);
    if (NULL == vals) {
      vals = new IntMap();
	  map_[key] = vals;
    }

    return vals->add(val);
  }

  template <class K>
  IntMap * IMap<K>::get( K key) {
	  IntMap *s = map_[key]; //get(key);
    if (NULL == s) {
      s = new IntMap();
    }
    return s;
  }

  template<class K>
  void IMap<K>::remove_key(map<K, IntMap*> &mp, vector<K> mp_lst, K k)
  {
	  auto element = mp.find(k);
	  mp.erase(element);

	  for (int i = 0; i < mp_lst.size(); i++)
	  {
		  if (mp_lst[i] == k)
		  {
			  mp_lst.erase(mp_lst.begin() + i);
			  return;
		  }
	  }
  }


  template <class K>
  bool IMap<K>::remove( K key,  int val) {
     IntMap *vals = get(key);
     bool ok = vals->del(val);
    if (vals->isEmpty()) {
		remove_key(map_, map_list, key);
    }
    return ok;
  }

  template<class K>
  bool IMap<K>::remove(K key) {
	  bool ret = map_.find(key) != map_.end();
	  remove_key(map_, map_list, key);
	  return ret;
	  //return 0 != mp.remove(key);
  }

  template <class K>
  int IMap<K>::size() {

	   int s = 0;
	   for (pair<K, IntMap*> p : map_)
	   {
		   s += p.second->size();
	   }
	   return s;


/*
     Iterator<K> I = map.keySet().iterator();
    int s = 0;
    while (I.hasNext()) {
       K key = I.next();
       IntMap vals = get(key);
      s += vals.size();
    }
    return s;*/
  }

   
  template <class K>
  set<K> IMap<K>::keySet() {
	   set<K> s;
	   for (pair<K, IntMap *> it : map_)
	   {
		   s.insert(it.first);
	   }
	   return s; //map.keySet();
  }

   /*
   Iterator<K> keyIterator() {
    return keySet().iterator();
  }*/

  template <class K>
  string IMap<K>::tostring() {
	  /*
	  string s;
	  bool first = true;
	  for (pair<K, IntMap*> it : map_)
	  {
		  if (first)
			  first = false;
		  else
		  {
			  int k = it.first; // the key is an int
			  char ks[12];
			  sprintf(ks, "%d", k);
			  s += k;
		  }

	  }
	  return s; //map.tostring()
	  */
	  return "IMAP String";
  }

  // specialization for array of int maps

  
  template <class K>
  vector<IMap<int>*>* IMap<K>::create( int l) {
     IMap<int> * first = new IMap<int>();
	 vector<IMap<int>*> * imaps = new vector<IMap<int>*>(l); 
    (*imaps)[0] = first;
    for (int i = 1; i < l; i++) {
      (*imaps)[i] = new IMap<int>();
    }
    return imaps;
  }
  

  template <class K>
  bool IMap<K>::put( vector<IMap<int>*> * imaps,  int pos,  int key,  int val) {
    return (*imaps)[pos]->put(key, val);
  }

  template <class K>
  vector<int>* IMap<K>::get( vector<IMap<int>*> * iMaps, vector<IntMap *> *vmaps, vector<int> * keys) {
     int l = iMaps->size();

	 vector<IntMap *> ms;
	 vector<IntMap *> vms;
    // ArrayList<IntMap> ms = new ArrayList<IntMap>();
     //ArrayList<IntMap> vms = new ArrayList<IntMap>();

    for (int i = 0; i < l; i++) {
       int key = (*keys)[i];
      if (0 == key) {
        continue;
      }
       IntMap *m = (*iMaps)[i]->get(key);
      ms.push_back(m);
      vms.push_back((*vmaps)[i]);
    }

	vector<IntMap *> ims(ms.size());
	vector<IntMap *> vims(vms.size());
   //  IntMap[] ims = new IntMap[ms.size()];
   //  IntMap[] vims = new IntMap[vms.size()];

    for (int i = 0; i < ims.size(); i++) {
		IntMap *im = ms[i];
      ims[i] = im;
	  IntMap *vim = vms[i];
      vims[i] = vim;
    }

     IntStack *cs = IntMap::intersect(ims, vims); // $$$ add vmaps here
	 vector<int> *is = new vector<int>;
	 for (int i = 0; i < cs->size(); i++)
	 {
		 is->push_back(cs->get(i));//(*cs)[i]);
	 }
	 /*
	 for (int x : *cs)
	 {
		 is->push_back(x);
	 }*/
     //int[] is = cs.toArray();
    for (int i = 0; i < is->size(); i++) {
      (*is)[i] = (*is)[i] - 1;
    }
	sort(is->begin(), is->end());
  //  java.util.Arrays.sort(is);
    return is;
  }

  template <class K>
  string IMap<K>::show( vector<IMap<int>*> * imaps) {
	   return "IMAP SHOW IMAPS";
    //return Arrays.tostring(imaps);
  }

  template <class K>
  string IMap<K>::show( vector<int> is) {
	   string s = "{";
	   bool first = true;
	   for (int x : is)
	   {
		   if (first) first = false;
		   else
		   {
			   s += ", ";
		   }
		   char tmp[11];
		   sprintf(tmp, "%d", x);
		   s += tmp;
	   }
	   s += "}";
	   return s;
   // return Arrays.tostring(is);
  }

  template class IMap<int>;
