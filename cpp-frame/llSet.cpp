#include "llSet.h"
#include <iostream>

using namespace std;

llSet::llSet() {
	first = NULL;
	last = NULL;
	num = 0;
}

llSet::llSet(llSet_element * ele) {
	first = NULL;
	last = NULL;
	num = 0;
	while(ele != NULL) {
		this->addElement(ele->getValue());
		ele = ele->getNext();
	}
}

llSet::~llSet() {
	llSet_element * ele, * tmp;
	ele = first;
	while(ele != NULL) {
		tmp = ele->getNext();
		delete ele;
		ele = tmp;
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
		this->num++;
	} 
	else {
		if(last->getValue() != num) {
			llSet_element * nw;
			nw = new llSet_element(num);
			last->setNext(nw);
			last = nw;
			this->num++;
		}
	}
}

int llSet::size() {
	return this->num;
}

void llSet::print() {
	llSet_element * ele = this->first;
	while(ele != NULL) {
		cout  << ele->getValue() << " -> " ;
		ele = ele->getNext();
	}
	cout << endl;
}









