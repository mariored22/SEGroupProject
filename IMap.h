#ifndef __IMAP_H__
#define __IMAP_H__

#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "IntMap.h"
#include "IntStack.h"

using namespace std;

template <class K>
class IMap {
private:
	map<K, IntMap*> map_;
	vector < K>  map_list;

public:

	IMap();
	void clear();
	bool put(K key, int val);
	IntMap *get(K key);
	void remove_key(map<K, IntMap*> &mp, vector<K> mp_lst, K k);
	bool remove(K key, int val);
	bool remove(K key);
	int size();
	set<K> keySet();
	string tostring();
	static vector<IMap<int>*>* create(int l);
	static bool put(vector<IMap<int>*> * imaps, int pos, int key, int val);
	static vector<int>* get(vector<IMap<int>*> * iMaps, vector<IntMap *> *vmaps, vector<int> * keys);
	static string show(vector<IMap<int>*> * imaps);
	static string show(vector<int> is);


};

#endif
