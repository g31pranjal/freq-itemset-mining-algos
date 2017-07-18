#include "uBitset.h"
#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <map>

using namespace std;

uBitset::uBitset(int N) {
	this->support = 0;
	this->bitBucket = (boost::dynamic_bitset<> **)malloc(precision*sizeof(boost::dynamic_bitset<> *));
	for(int i=0;i<precision;i++) {
		this->bitBucket[i] = new boost::dynamic_bitset<>(N);
	}
	eligible = new boost::dynamic_bitset<>(N);
}

uBitset::~uBitset() {
	delete this->eligible;
	for(int i=0;i<precision;i++) {
		delete this->bitBucket[i];
	}
	delete this->bitBucket;
}

void uBitset::insert(int tid, double probability) {
	double fraction = 0;
	double factor = 1.0;
	if(!this->eligible->test(tid)) {
		this->eligible->set(tid);
		for(int i=0;i<precision;i++) {
			probability *= 2.0;
			factor /= 2.0;
			if(probability > 1.0) {
				this->bitBucket[i]->set(tid);
				fraction += factor;
				probability -= 1.0;
			}
		}
		this->support += fraction;
	}
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

double uBitset::getProbability(int tid) {
	
	if(!this->eligible->test(tid)){
		cout << "returning " << endl;
		return 0.0;
	}

	double fraction = 0.0;
	double factor = 1.0;
	for(int i=0;i<precision;i++) {
		factor /= 2.0;
		fraction += ((int)(this->bitBucket[i]->test(tid))) * factor;
	}
}















