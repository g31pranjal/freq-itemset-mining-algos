#include "llSets.h"

using namespace std;

llSets::llSets() {
	first = NULL;
	last = NULL;
	num = 0;
}

llSets::addElement(int num) {
	
	if(first == NULL && last == NULL) {
		llSet * nw;
		nw = new llSet(num);
		first = last = nw;
		num++;
	} 
	else {
		if(last->val != num) {
			llSet * nw;
			nw = new llSet(num);
			last->next = nw;
			last = nw;
			num++;
		}
	}
	
}