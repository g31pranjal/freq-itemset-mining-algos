#ifndef ULLSET_ELEMENT_H
#define ULLSET_ELEMENT_H

#include <utility>  

using namespace std;

class ullSet_element {
	private :
		pair<int, double> val;
		ullSet_element * next;
	public :
		ullSet_element(pair<int, double> val);
		ullSet_element* getDeepCopy();
		pair<int,double> getValue();
		ullSet_element * getNext();
		void setNext(ullSet_element * nw);
};

#endif

#ifndef ULLSET_H
#define ULLSET_H

class ullSet {
	private :
		ullSet_element * first;
		ullSet_element * last;
		int num;
		double supp;
	public :
		ullSet();
		ullSet(ullSet_element * first);
		~ullSet();
		ullSet* getDeepCopy();
		int size();
		double support();
		ullSet_element * getFirst();
		ullSet_element * getLast();
		void setFirst(ullSet_element * nw);
		void setLast(ullSet_element * nw);
		void addElement(pair<int, double> val);
		void print();
};


#endif

