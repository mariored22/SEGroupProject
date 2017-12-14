#ifndef _IMAP_
#define _IMAP_

#include <map>
#include "IntMap.h"

template<typename K>

class IMap 
{
private:
	std::map<K, IntMap> _map;

public:
	/*void clear();
	bool put(const K key, const int val);
	IntMap& get(const K key);
	bool remove(const K key, const int val);
	bool remove(const K key);
	int size();*/
	
	void clear()
	{
		_map.clear();
	}
	
	bool put(const K key, const int val)
	{
		return !!(_map[key].get(val));
	}

	IntMap& get(const K key)
	{
		return _map[key];
	}
	
	bool remove(const K key, const int val)
	{	
		IntMap& vals = get(key);
		bool ok = vals.deleteKey(key);
		if (vals.isEmpty()) 
		{
			_map.erase(_map.find(key));
		}
		return ok;
	}
	
	bool remove(const K key)
	{
		if (_map.find(key) != _map.end()) 
		{
			_map.erase(_map.find(key));
			return true;
		}
		return false;
	}

	int size()
	{
		return _map.size();
	}	
};

#endif
