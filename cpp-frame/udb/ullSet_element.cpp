#include "ullSet.h"
#include <iostream>

using namespace std;

ullSet_element::ullSet_element(pair<int, double> val) {
	this->val = val;
	this->next = NULL;
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

