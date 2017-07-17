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

		// int Estore = INTSIZE*(totalInOneFrequent + N);
		// int Dstore = INTSIZE*(oneFrequentItems->size()*N + N - totalInOneFrequent - N);
		// int Vstore = (double)N*(oneFrequentItems->size()+1.0)*BITSIZE;

		// cout << Estore << ", " << Vstore << ", " << Dstore << endl;
		// cout << oneFrequentItems->size() << ", " << N << endl;

		if(algo == 0) {
			// if(Estore <= Dstore && Estore <= Vstore ) {
			// 	// cout << "ECLAT" << endl;
			// 	// this->constructTIDSETS(oneFrequentItems);
			// 	this->constructTIDSETS(oneFrequentItems);
			// }
			// else if(Vstore <= Estore && Vstore <= Dstore){
			// 	// cout << "VIPER" << endl;
			// 	this->constructBITSETS(oneFrequentItems);
			// }
			// else{
			// 	// cout << "DECLAT" << endl;
			// 	this->constructDIFFSETS(oneFrequentItems);
			// }
		}
		else {
			if(algo == 1) {
				// cout << "ECLAT" << endl;
				this->constructTIDSETS(oneFrequentItems);
			}
			else if(algo == 2){
				// cout << "VIPER" << endl;
				this->constructBITSETS(oneFrequentItems);
			}
		}
	}

	writer.close();
	
	this->printStats();

}

void uAlgoFramework::constructTIDSETS(vector<int> * equivalenceClassItems) {
	
	vector<set<tidAndProb * > * > * equivalenceClassTidsets = new vector<set<tidAndProb * > * >();

	// populate the vector of set ints corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {
		equivalenceClassTidsets->push_back(verticalDB->at(equivalenceClassItems->at(i)));
		verticalDB->at(equivalenceClassItems->at(i)) = NULL; 
	} 

	delete this->database;

	int * prefixArray = new int[1000];
	
	this->processEquivalenceClassEclat(prefixArray, 0, N, equivalenceClassItems, equivalenceClassTidsets);
	
	delete prefixArray;

}


void uAlgoFramework::constructBITSETS(vector<int> * equivalenceClassItems) {

}


void algoFramework::processEquivalenceClassEclat(int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<set<tidAndProb * > * > * equivalenceClassTidsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> ECLAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

	cout << "Prefix : " ;
	for(int i=0;i<prefixLength;i++)
		cout << prefix[i] << ", " ;
	cout << endl;
	
	rec[1]++;

	int length = prefixLength+1;
	
	if(equivalenceClassItems->size() == 1) {
		
		int item = equivalenceClassItems->at(0);
		set<int> * tidset = equivalenceClassTidsets->at(0);
		int support = calculateSupportInUTidset(tidset);
		this->save(prefix, prefixLength, item, support);

		// delete prefixTidset;
		// delete equivalenceClassItems;
		// for(int d=0;d<equivalenceClassTidsets->size();d++) 
		// 	delete equivalenceClassTidsets->at(d);
		// delete equivalenceClassTidsets;

		return;
	}

	if(equivalenceClassItems->size() == 2) {
		
		int itemI = equivalenceClassItems->at(0);
		set<tidAndProb * > * tidsetI = equivalenceClassTidsets->at(0);
		int supportI = calculateSupportInUTidset(tidsetI);
		this->save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		set<tidAndProb * > * tidsetJ = equivalenceClassTidsets->at(1);
		int supportJ = calculateSupportInUTidset(tidsetJ);
		this->save(prefix, prefixLength, itemJ, supportJ);
		
		// clear

		set<int> * tidsetIJ = this->performINTERSECTION(tidsetI, tidsetJ);
		int supportIJ = tidsetIJ->size();
		
		if(supportIJ >= minSupRelative) {
			int newPrefixLength = prefixLength+1;
			prefix[prefixLength] = itemI;
			this->save(prefix, newPrefixLength, itemJ, supportIJ);
		}

		delete tidsetIJ;
		delete prefixTidset;
		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassTidsets->size();d++) 
			delete equivalenceClassTidsets->at(d);
		delete equivalenceClassTidsets;

		return;
	}


	for(int i=0; i< equivalenceClassItems->size(); i++) {
		
		int ETotal = 0;
		int DTotal = 0;

		int suffixI = equivalenceClassItems->at(i);
		set<int> * tidsetI = equivalenceClassTidsets->at(i);
		int supportI = tidsetI->size();
		this->save(prefix, prefixLength, suffixI, supportI);
		
		vector<int> * equivalenceClassISuffixItems = new vector<int>();
		vector<set<int> * > * equivalenceClassITidsets = new vector<set<int> * >();

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {
			
			int suffixJ = equivalenceClassItems->at(j);
			set<int> * tidsetJ = equivalenceClassTidsets->at(j);
			int supportJ = tidsetJ->size();
			
			set<int> * tidsetIJ = this->performINTERSECTION(tidsetI, tidsetJ);
			int supportIJ = tidsetIJ->size();
			
			if(supportIJ >= minSupRelative) {
				ETotal += supportIJ;
				DTotal += (supportI - supportIJ);
				equivalenceClassISuffixItems->push_back(suffixJ);
				equivalenceClassITidsets->push_back(tidsetIJ);
			}
			else {
				delete tidsetIJ;
			}
		}
		
		if(equivalenceClassISuffixItems->size() > 0) {

			prefix[prefixLength] = suffixI;
			int newPrefixLength = prefixLength+1;
			
			if(algo == 1) {
				
				set<int> * tidsetIClone = new set<int>(tidsetI->begin(), tidsetI->end());
				this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);

			}
			else {

				// double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
				// double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
				// int ECLATthreshold = (int)(N*(1.0/INTSIZE) );
				// int ECLATstart = 0;
				// int DECLATthreshold  = supportI - ECLATthreshold;
				// int DECLATstart  = supportI;

				//  Estore = INTSIZE*(Etotal + supportI)
				int Estore = INTSIZE*(ETotal + supportI);
				//  Dstore = INTSIZE*(m*supportI - Etotal + (N-supportI) )
				int Dstore = INTSIZE*(equivalenceClassISuffixItems->size()*supportI + N - ETotal - supportI);
				//  Vstore = N*(m+1)*BITSIZE
				int Vstore = (double)N*(equivalenceClassISuffixItems->size()+1.0)*BITSIZE;

				// if(DECLATthreshold <= ECLATthreshold) {

				// 	double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
			
				// 	if(EAvg > PointOfDiff){
						
				// 		// cout << "DECLAT" << endl;
						
				// 		vector<set<int> * > * equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
				// 		set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);

				// 		for(int i=0;i<equivalenceClassITidsets->size();i++)
				// 			delete equivalenceClassITidsets->at(i);
				// 		delete equivalenceClassITidsets;

				// 		this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);	
				// 	}
				// 	else {

				// 		// cout << "ECLAT" << endl;
				
				// 		set<int> * tidsetIClone = new set<int>(tidsetI->begin(), tidsetI->end());
				// 		this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				// 	}
				// }
				// else {

					cout << Estore << ", " << Vstore << ", " << Dstore << endl;

					if(Estore <= Dstore && Estore <= Vstore){
						
						// cout << "ECLAT" << endl;
					
						set<int> * tidsetIClone = new set<int>(tidsetI->begin(), tidsetI->end());
						this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
					else if(Vstore <= Estore && Vstore <= Dstore){
						
						// cout << "VIPER" << endl;
						
						boost::dynamic_bitset<> * prefixBitset = formPrefixBitsetFromPrefixTidset(tidsetI);
						vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets = convertTIDSETStoBITSETS(equivalenceClassITidsets);

						for(int i=0;i<equivalenceClassITidsets->size();i++)
							delete equivalenceClassITidsets->at(i);
						delete equivalenceClassITidsets;

						this->processEquivalenceClassViper(prefixBitset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
					}
					else{
						
						// System.out.println("DECLAT");
					
						vector<set<int> * > * equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
						set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);
						
						for(int i=0;i<equivalenceClassITidsets->size();i++)
							delete equivalenceClassITidsets->at(i);
						delete equivalenceClassITidsets;

						processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
				// }
			}			
		}
		else {
			delete equivalenceClassISuffixItems;
			delete equivalenceClassITidsets;
		}
	}

	delete prefixTidset;
	delete equivalenceClassItems;
	for(int d=0;d<equivalenceClassTidsets->size();d++) 
		delete equivalenceClassTidsets->at(d);
	delete equivalenceClassTidsets;

}

set<tidAndProb * > * uAlgoFramework::performINTERSECTION(set<tidAndProb * > * tidsetI, set<tidAndProb * > * tidsetJ) {
	
	set<tidAndProb * > * tidsetIJ = new set<tidAndProb * >();
	if( calculateSupportInUTidset(tidsetJ) < calculateSupportInUTidset(tidsetI)){
		for(set<tidAndProb * >::iterator i = tidsetJ->begin();i != tidsetJ->end();i++) 
			
			if(tidsetI->find(*i) != tidsetI->end() )
				tidsetIJ->insert(*i);
			
	}
	else {
		for(set<int>::iterator i = tidsetI->begin();i != tidsetI->end();i++) 
			if(tidsetJ->find(*i) != tidsetJ->end() )
				tidsetIJ->insert(*i);
	}

	return tidsetIJ;
}









void uAlgoFramework::printStats() {
	cout << "\n\n===================================================" << endl;
	cout << " Transactions count from database : " << N << endl;
	cout << " Frequent itemset count : " << itemsetCount << endl;
	cout << " Usage : ECLAT "  << rec[1] << " VIPER " << rec[2] << " DECLAT " << rec[3] << endl;
	cout << "===================================================" << endl;
}

void uAlgoFramework::save(int * prefix, int prefixLength, int suffixItem, int support) {
	
	itemsetCount++;
	string s = "";

	for(int i=0; i < prefixLength; i++) 
		s = s + (to_string(prefix[i]) + " ") ;
	s = s + (to_string(suffixItem) + " #SUP: " + to_string(support) +  '\n' );

	writer << s;
}

























