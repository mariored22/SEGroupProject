#ifndef _INTMAP_
#define _INTMAP_

#define FREE_KEY 0
#define NO_VALUE 0

#include <stdint.h>
#include "IntStack.h"

/**
 * derived from code at https://github.com/mikvor/hashmapTest
 */
class IntMap
{
private:
	/** Keys and values */
	int* m_data;
	int dataLength;

	/** Do we have 'free' key in the map? */
	bool m_hasFreeKey;
	/** Value of 'free' key */
	int m_freeValue;

	/** Fill factor, must be between (0 and 1) */
	float m_fillFactor;
	/** We will resize a map once it reaches this size */
	int m_threshold;
	/** Current map size */
	int m_size;

	/** Mask to calculate the original position */
	int m_mask;
	int m_mask2;
			
public:
	IntMap(int size = (1<<2), float fillFactor = 0.75f);
	~IntMap();
	int get(const int key);
	bool contains(const int key);
	bool addKey(const int key);
	bool deleteKey(const int key);
	bool isEmpty();

	static void intersect0(IntMap* m, IntMap* maps, IntMap* vmaps, int mapsLength, IntStack* r);
	static IntStack* intersect(IntMap* maps, IntMap* vmaps, int mapsLength);
	int put(const int key, const int value);
	int remove(const int key);
	int size();
private:
	int shiftKeys(int pos);
	void rehash(const int newCapacity);
	static int64_t nextPowerOfTwo(int64_t x);
	static int arraySize(const int expected, const float f);
	static int phiMix(const int x);
};

#endif
