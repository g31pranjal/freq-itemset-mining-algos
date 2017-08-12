#ifndef LLSET_ELEMENT_H
#define LLSET_ELEMENT_H

using namespace std;

class llSet_element {
	private :
		int val;
		llSet_element * next;
	public :
		llSet_element(int num);
		int getValue();
		llSet_element * getNext();
		void setNext(llSet_element * nw);
};

#endif

#ifndef LLSET_H
#define LLSET_H

class llSet {
	private :
		llSet_element * first;
		llSet_element * last;
		int num;
	public :
		llSet();
		llSet(llSet_element * first);
		~llSet();
		int size();
		llSet_element * getFirst();
		llSet_element * getLast();
		void setFirst(llSet_element * nw);
		void setLast(llSet_element * nw);
		void addElement(int num);
		void print();
};


#endif

