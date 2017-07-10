#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
	
#include "uAlgoFramework.h"
#include "UTransactionDatabase.h"
#include "TidAndProb.h"

using namespace std;

static int INTSIZE = 32;

double calculateSup(set<TidAndProb *> * tidset){
	double val = 0;
	for (set<TidAndProb *>::iterator i = tidset->begin(); i!= tidset->end(); i++){
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

void uAlgoFramework::runAlgo(char * outputFile, UTransactionDatabase * database, double minsupp){

	this->database = database;
	this->N = database->getN();
	this->M = database->getM();
	this->minSupRelative = minsupp * (double)N;
	this->verticalDB = database->getVerticalDatabase();
	this->writer.open(outputFile);

	itemsetCount = 0;
	double totalInOneFrequent = 0l;

	
	//cout << fixed;
	cout <<"\nMinimum relative support is :" << minSupRelative << "\n" << endl;
	//cout.unsetf(ios_base::floatfield);
	

	vector<int> * oneFrequentItems = new vector<int>();

	for( map<int, set<TidAndProb *> *>::iterator i = verticalDB->begin(); i != verticalDB->end(); i++){
		set<TidAndProb *> *tidset = i->second;
		double expectedSup = calculateSup(tidset);
		int item = i->first;
		cout <<"item: #"<< item << " Expected support: " << expectedSup <<endl;
		if (expectedSup >= minSupRelative){
			oneFrequentItems->push_back(item);
			totalInOneFrequent += expectedSup;
		}
	}

	cout << totalInOneFrequent << endl;
}
