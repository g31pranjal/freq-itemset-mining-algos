#include "llSet.h"
#include <iostream>

using namespace std;

llSet_element::llSet_element(int num) {
	this->val = num;
	this->next = NULL;
}

int llSet_element::getValue() {
	return this->val;
}

llSet_element * llSet_element::getNext() {
	return this->next;
}

void llSet_element::setNext(llSet_element * nw) {
	this->next = nw;
}

