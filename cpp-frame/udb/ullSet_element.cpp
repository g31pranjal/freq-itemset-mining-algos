#include "ullSet.h"
#include <iostream>

using namespace std;

ullSet_element::ullSet_element(pair<int, double> val) {
	this->val = val;
	this->next = NULL;
}

ullSet_element* ullSet_element::getDeepCopy() {
	pair<int, double> newPair = make_pair(this->val.first, this->val.second);
//	newPair->first = this->val.first;
//	newPair->second = this->val.first;
	ullSet_element* newEle = new ullSet_element(newPair);

	return newEle;
}

pair<int, double> ullSet_element::getValue() {
	return this->val;
}

ullSet_element * ullSet_element::getNext() {
	return this->next;
}

void ullSet_element::setNext(ullSet_element * nw) {
	this->next = nw;
}

