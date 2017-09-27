#include "uBitset.h"
#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include <map>

using namespace std;

uBitset::uBitset(int N, int _precision) {
	this->precision = _precision;
	this->support = 0;
	this->num = 0;
//	this->bitBucket = (boost::dynamic_bitset<> **)malloc(precision*sizeof(boost::dynamic_bitset<> *));
	unsigned long precision_long = unsigned(long(precision));
	this->bitBucket = new boost::dynamic_bitset<> *[precision_long]();
	for(int i=0;i<precision;i++) {
		this->bitBucket[i] = new boost::dynamic_bitset<>(N);
	}
	eligible = new boost::dynamic_bitset<>(N);
}

uBitset::~uBitset() {
	delete this->eligible;
	for(int i=0;i<precision;i++) {
//		delete this->bitBucket[i];
		this->bitBucket[i]->clear();
		delete this->bitBucket[i];
	}
	//free(this->bitBucket);
	delete[] this->bitBucket;
}

uBitset* uBitset::getDeepCopy() {
	uBitset* copy = new uBitset(this->eligible->size(), this->precision);
	boost::dynamic_bitset<> * eligibleCopy = this->getEligible();
//	boost::dynamic_bitset<> ** bitBucketCopy = this->getBitBucket();

	for(int i = eligibleCopy->find_first(); i >= 0; i = eligibleCopy->find_next(i)){
		 if(eligibleCopy->test(i)){
			 //cout << "tid: " << i << ", probability: " << this->getProbability(i)<<endl;
			 copy->insert(i, this->getProbability(i));
		 }
	}
	eligibleCopy = 0;

	return copy;
}

void uBitset::insert(int tid, double probability) {
	double fraction = 0;
	double factor = 1.0;
	if(!this->eligible->test(tid)) {
		this->num++;
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
	return this->support;
}

int uBitset::size() {
	return this->num;
}

boost::dynamic_bitset<> ** uBitset::getBitBucket() {
	return this->bitBucket;
}

boost::dynamic_bitset<> * uBitset::getEligible() {
	return this->eligible;
}

double uBitset::getProbability(int tid) {

	unsigned long tid_long = unsigned(long(tid));
	if(!this->eligible->test(tid_long)){
		return 0.0;
	}

	double fraction = 0.0;
	double factor = 1.0;
	for(int i=0;i<precision;i++) {
		factor /= 2.0;
		fraction += ((int)(this->bitBucket[i]->test(tid_long))) * factor;
	}
	return fraction;
}















