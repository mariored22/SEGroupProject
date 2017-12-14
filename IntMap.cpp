#include <cmath>
#include <algorithm>
#include <cassert>
#include "IntMap.h"

#define INT_PHI 0x9E3779B9

IntMap::IntMap(int size, float fillFactor) 
{
	// "FillFactor must be in (0, 1)"
	assert(fillFactor > 0 && fillFactor < 1);

	// "Size must be positive!"
	assert(size>0);

	int capacity = arraySize(size, fillFactor);
    m_mask = capacity - 1;
    m_mask2 = capacity * 2 - 1;
    m_fillFactor = fillFactor;

    m_data = new int[capacity * 2];
    m_threshold = (int) (capacity * fillFactor);
}

IntMap::~IntMap()
{
	delete m_data;
}

int IntMap::get(const int key)
{
    int ptr = (phiMix(key) & m_mask) << 1;

	if (key == FREE_KEY)
		return m_hasFreeKey ? m_freeValue : NO_VALUE;

    int k = m_data[ptr];

    if (k == FREE_KEY)
		return NO_VALUE; //end of chain already
    if (k == key) //we check FREE prior to this call
		return m_data[ptr + 1];

    while (true) 
	{
		ptr = (ptr + 2) & m_mask2; //that's next index
		k = m_data[ptr];
		if (k == FREE_KEY)
			return NO_VALUE;
		if (k == key)
			return m_data[ptr + 1];
    }
}

// for use as IntSet - Paul Tarau
bool IntMap::contains(const int key) 
{
	return NO_VALUE != get(key);
}

bool IntMap::addKey(const int key) 
{
	return NO_VALUE != put(key, 666);
}

bool IntMap::deleteKey(const int key) 
{
    return NO_VALUE != remove(key);
}

bool IntMap::isEmpty() 
{
    return 0 == m_size;
}

void IntMap::intersect0(IntMap* m, IntMap* maps, IntMap* vmaps, int mapsLength, IntStack* r) 
{
    int* data = m->m_data;
    for (int k = 0; k < m->dataLength; k += 2) 
	{
		bool found = true;
		int key = data[k];
		if (FREE_KEY == key)
		{
			continue;
		}

		for (int i = 1; i < mapsLength; i++) 
		{
			IntMap& map = maps[i];
			int val = map.get(key);

			if (NO_VALUE == val) 
			{
				IntMap& vmap = vmaps[i];
				int vval = vmap.get(key);
				if (NO_VALUE == vval) 
				{
					found = false;
					break;
				}
			}
		}
		if (found) 
		{
			r->push(key);
		}
	}
}

IntStack* IntMap::intersect(IntMap* maps, IntMap* vmaps, int mapsLength) 
{
    IntStack* r = new IntStack();

    intersect0(&(maps[0]), maps, vmaps, mapsLength, r);
    intersect0(&(vmaps[0]), maps, vmaps, mapsLength, r);

    return r;
}

// end changes

int IntMap::put(const int key, const int value) 
{
    if (key == FREE_KEY) 
	{
		int ret = m_freeValue;
		if (!m_hasFreeKey) 
		{
			++m_size;
		}
		m_hasFreeKey = true;
		m_freeValue = value;
		return ret;
    }

    int ptr = (phiMix(key) & m_mask) << 1;
    int k = m_data[ptr];
    if (k == FREE_KEY) //end of chain already
    {
		m_data[ptr] = key;
		m_data[ptr + 1] = value;
		if (m_size >= m_threshold) 
		{
			rehash(dataLength * 2); //size is set inside
		} 
		else 
		{
			++m_size;
		}
		return NO_VALUE;
	}
	else if (k == key) //we check FREE prior to this call
    {
		int ret = m_data[ptr + 1];
		m_data[ptr + 1] = value;
		return ret;
    }

    while (true) 
	{
		ptr = (ptr + 2) & m_mask2; //that's next index calculation
		k = m_data[ptr];
		if (k == FREE_KEY) 
		{
			m_data[ptr] = key;
			m_data[ptr + 1] = value;
			if (m_size >= m_threshold) 
			{
			  rehash(dataLength * 2); //size is set inside
			}
			else 
			{
			  ++m_size;
			}
			return NO_VALUE;
		} 
		else if (k == key) 
		{
			int ret = m_data[ptr + 1];
			m_data[ptr + 1] = value;
			return ret;
		}
	}
}

int IntMap::remove(const int key) 
{
    if (key == FREE_KEY) 
	{
		if (!m_hasFreeKey)
			return NO_VALUE;
		m_hasFreeKey = false;
		--m_size;
		return m_freeValue; //value is not cleaned
	}

    int ptr = (phiMix(key) & m_mask) << 1;
    int k = m_data[ptr];
    if (k == key) //we check FREE prior to this call
    {
		int res = m_data[ptr + 1];
		shiftKeys(ptr);
		--m_size;
		return res;
    } 
	else if (k == FREE_KEY)
		return NO_VALUE; //end of chain already

    while (true) 
	{
		ptr = (ptr + 2) & m_mask2; //that's next index calculation
		k = m_data[ptr];
		if (k == key) 
		{
			int res = m_data[ptr + 1];
			shiftKeys(ptr);
			--m_size;
			return res;
		}
		else if (k == FREE_KEY)
			return NO_VALUE;
    }
}

int IntMap::shiftKeys(int pos) 
{
    // Shift entries with the same hash.
    int last, slot;
    int k;
    int* data = m_data;

    while (true) 
	{
		pos = ((last = pos) + 2) & m_mask2;
		while (true) 
		{
			if ((k = data[pos]) == FREE_KEY) 
			{
				data[last] = FREE_KEY;
				return last;
			}
			slot = (phiMix(k) & m_mask) << 1; //calculate the starting slot for the current key
			if (last <= pos ? last >= slot || slot > pos : last >= slot && slot > pos) 
			{
				break;
			}
			pos = (pos + 2) & m_mask2; //go to the next entry
		}
		data[last] = k;
		data[last + 1] = data[pos + 1];
	}
}


int IntMap::size() 
{
    return m_size;
}

void IntMap::rehash(const int newCapacity) 
{
    m_threshold = (int) (newCapacity / 2 * m_fillFactor);
    m_mask = newCapacity / 2 - 1;
    m_mask2 = newCapacity - 1;

    int oldCapacity = dataLength;
    int* oldData = m_data;

    m_data = new int[newCapacity];
    m_size = m_hasFreeKey ? 1 : 0;

    for (int i = 0; i < oldCapacity; i += 2) 
	{
		int oldKey = oldData[i];
		if (oldKey != FREE_KEY) 
		{
			put(oldKey, oldData[i + 1]);
		}
    }
	delete oldData;
}

  /** Taken from FastUtil implementation */

  /** Return the least power of two greater than or equal to the specified value.
   *
   * <p>Note that this function will return 1 when the argument is 0.
   *
   * @param x a long integer smaller than or equal to 2<sup>62</sup>.
   * @return the least power of two greater than or equal to the specified value.
   */
int64_t IntMap::nextPowerOfTwo(int64_t x) 
{
	if (x == 0)
		return 1;
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;

	return (x | x >> 32) + 1;
}

/** Returns the least power of two smaller than or equal to 2<sup>30</sup>
 * and larger than or equal to <code>Math.ceil( expected / f )</code>.
 *
 * @param expected the expected number of elements in a hash table.
 * @param f the load factor.
 * @return the minimum possible size for a backing array.
 * @throws IllegalArgumentException if the necessary size is larger than 2<sup>30</sup>.
 */
int IntMap::arraySize(const int expected, const float f) 
{
	int64_t s = std::max<int64_t>(2, nextPowerOfTwo((int64_t)ceil((float)expected / f)));

	// ("Too large (" + expected + " expected elements with load factor " + f + ")");
	assert(s<=(1 << 30));    		
	return (int) s;
}

//taken from FastUtil
int IntMap::phiMix(const int x) 
{
	int h = x * INT_PHI;
	return h ^ h >> 16;
}
