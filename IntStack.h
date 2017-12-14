#ifndef _INTSTACK_
#define _INTSTACK_

/**
 Dynamic Stack for int data.
 */

class IntStack {
private:
	int *stack;
	int stackLength;
	int top;
	static const int SIZE = 16; // power of 2
	static const int MINSIZE = 1 << 15; // power of 2
public:	
	IntStack(const int size=SIZE);
	~IntStack();
	IntStack(const IntStack& copied);
	IntStack operator=(const IntStack& copied);
    int getTop() const;
    int setTop(const int top);
	void clear();
	bool isEmpty() const;
    void push(const int i);
	int pop();
	int get(const int i) const;
    void set(const int i, const int val);
    int size() const;
	void expand();
	void shrink();  

	//int size() const;
	//int get(int index) const;
};

#endif
