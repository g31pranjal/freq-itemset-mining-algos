#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
	
#include "uAlgoFramework.h"
#include "uTransactionDatabase.h"
#include "tidAndProb.h"

using namespace std;

static int INTSIZE = 32;

double calculateSupportInUTidset(set<tidAndProb *> * tidset) {
	double val = 0;
	for (set<tidAndProb *>::iterator i = tidset->begin(); i!= tidset->end(); i++){
		val += (*i)->getProbability();
	}
	return val;
}

uAlgoFramework::uAlgoFramework(){
	this->algo = 0;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;
}

uAlgoFramework::uAlgoFramework(int algo){
	this->algo = algo;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;	
}

void uAlgoFramework::runAlgo(char * outputFile, uTransactionDatabase * database, double minsupp){

	this->database = database;
	this->N = database->getN();
	this->M = database->getM();
	this->minSupRelative = minsupp * (double)N;
	this->verticalDB = database->getVerticalDatabase();
	this->writer.open(outputFile);

	itemsetCount = 0;
	double totalInOneFrequent = 0l;

	cout << "\t\tminsupp :: " << this->minSupRelative << endl;

	vector<int> * oneFrequentItems = new vector<int>();

	for( map<int, set<tidAndProb *> *>::iterator i = verticalDB->begin(); i != verticalDB->end(); i++){
		set<tidAndProb *> *tidset = i->second;
		double support = calculateSupportInUTidset(tidset);
		cout << "\tsupport : " << support << endl;
		int item = i->first;
		if (support >= minSupRelative){
			oneFrequentItems->push_back(item);
			totalInOneFrequent += tidset->size();
		}
	}

	cout << oneFrequentItems->size() << endl;
	cout << totalInOneFrequent << endl;

	if (oneFrequentItems->size() > 0) {
		
		// double avgTidsetSize =  (totalInOneFrequent / oneFrequentItems->size());
		
		sort(oneFrequentItems->begin(), oneFrequentItems->end(), [this] (int arg0, int arg1) {
			return calculateSupportInUTidset(this->verticalDB->at(arg0)) < calculateSupportInUTidset(this->verticalDB->at(arg1) );
		});
	}


}

























