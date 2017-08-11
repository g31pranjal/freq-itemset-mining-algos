#include "llSet.h"
#include <iostream>

using namespace std;

llSet::llSet() {
	first = NULL;
	last = NULL;
	num = 0;
}

llSet::~llSet() {
	llSet_element * ele;
	ele = first;
	while(ele != NULL) {
		ele = ele->getNext();
		delete ele;
	}
}

llSet_element * llSet::getFirst() {
	return this->first;
}

llSet_element * llSet::getLast() {
	return this->last;
}

void llSet::setFirst(llSet_element * nw) {
	this->first = nw;
}

void llSet::setLast(llSet_element * nw) {
	this->last = nw;
}

void llSet::addElement(int num) {
	
	if(first == NULL && last == NULL) {
		llSet_element * nw;
		nw = new llSet_element(num);
		first = last = nw;
		num++;
	} 
	else {
		if(last->getValue() != num) {
			llSet_element * nw;
			nw = new llSet_element(num);
			last->setNext(nw);
			last = nw;
			num++;
		}
	}
}