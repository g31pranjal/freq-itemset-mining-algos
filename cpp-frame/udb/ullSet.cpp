#include "ullSet.h"
#include <iostream>

using namespace std;

ullSet::ullSet() {
	this->first = NULL;
	this->last = NULL;
	this->num = 0;
	this->supp = 0.0;
}

ullSet::ullSet(ullSet_element * ele) {
	this->first = NULL;
	this->last = NULL;
	this->num = 0;
	this->supp = 0.0;
	while(ele != NULL) {
		this->addElement(ele->getValue());
		ele = ele->getNext();
	}
}

ullSet::~ullSet() {
	ullSet_element * ele, * tmp;
	ele = first;
	while(ele != NULL) {
		tmp = ele->getNext();
		delete ele;
		ele = tmp;
	}
}

ullSet_element * ullSet::getFirst() {
	return this->first;
}

ullSet_element * ullSet::getLast() {
	return this->last;
}

void ullSet::setFirst(ullSet_element * nw) {
	this->first = nw;
}

void ullSet::setLast(ullSet_element * nw) {
	this->last = nw;
}

void ullSet::addElement(pair<int, double> val) {
	
	if(first == NULL && last == NULL) {
		ullSet_element * nw;
		nw = new ullSet_element(val);
		this->first = this->last = nw;
		this->num++;
		this->supp += val.second;
	} 
	else {
		if(last->getValue().first != val.first) {
			ullSet_element * nw;
			nw = new ullSet_element(val);
			last->setNext(nw);
			last = nw;
			this->num++;
			this->supp += val.second;
		}
	}
}

int ullSet::size() {
	return this->num;
}

double ullSet::support() {
	return this->supp;
}

void ullSet::print() {
	ullSet_element * ele = this->first;
	while(ele != NULL) {
		cout << "(" << ele->getValue().first << "," << ele->getValue().second << ") -> " ;
		ele = ele->getNext();
	}
	cout << endl;
}

