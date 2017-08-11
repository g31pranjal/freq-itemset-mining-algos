#include "uTidset.h"
#include <iostream>
#include <unordered_map>

using namespace std;

uTidset::uTidset(){
	this->support = 0;
	this->tidAndProbMap = new unordered_map<int, double>();
}

uTidset::~uTidset(){
	delete tidAndProbMap;
}

void uTidset::insert(int tid, double probability){
	if(tidAndProbMap->find(tid) == tidAndProbMap->end()){
		tidAndProbMap->insert(pair<int, double>(tid, probability));
		support += probability;
	}
}

void uTidset::print(){
	for (unordered_map<int, double>::iterator it = tidAndProbMap->begin(); it != tidAndProbMap->end(); it++){
		cout << it->first << " (" << it->second <<"), ";
	}
	cout << endl << "support: " << support << endl;
}

double uTidset::getSupport() {
	return support;
}

unordered_map<int, double> * uTidset::getMap() {
	return tidAndProbMap;
}