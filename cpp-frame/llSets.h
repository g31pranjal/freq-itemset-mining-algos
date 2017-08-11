#ifndef LLSET_ELEMENT_H
#define LLSET_ELEMENT_H

using namespace std;

class llSet_element {
	private :
		int val;
		llset_element * next;
	public :
		llSet_element(int num);
}

class llSet {
	private :
		llset_element * first;
		llset_element * last;
		int num;
	public :
		llSets();
		~llSets();
		addElement(int num);
}


#endif

