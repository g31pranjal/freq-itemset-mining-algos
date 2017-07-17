#include "uBitset.h"
#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <map>

using namespace std;


uBitset::uBitset(int N){
	this->support = 0;
	for(int i=0;i<precision;i++) {
		this->bitBucket[i] = new boost::dynamic_bitset<>(N);
	}
	eligible = new boost::dynamic_bitset<>(N);
}

uBitset::~uBitset(){
	delete this->eligible;
	for(int i=0;i<precision;i++) {
		delete this->bitBucket[i];
	}
}

void uBitset::insert(int tid, double probability){
	// if(tidAndProbMap->find(tid) == tidAndProbMap->end()){
	// 	tidAndProbMap->insert(pair<int, double>(tid, probability));
	// 	support += probability;
	// }
}

// void uBitset::print(){
// 	// for (map<int, double>::iterator it = tidAndProbMap->begin(); it != tidAndProbMap->end(); it++){
// 	// 	cout << it->first << " (" << it->second <<"), ";
// 	// }
// 	// cout << endl << "support: " << support << endl;
// }

double uBitset::getSupport() {
	return support;
}

boost::dynamic_bitset<> ** uBitset::getBitBucket() {
	return bitBucket;
}

boost::dynamic_bitset<> * uBitset::getEligible() {
	return eligible;
}















