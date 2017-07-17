#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
	
#include "uAlgoFramework.h"
#include "uTransactionDatabase.h"
#include "uTidset.h"
#include "uBitset.h"

using namespace std;

static int INTSIZE = 32;


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

	for( map<int, uTidset *>::iterator i = verticalDB->begin(); i != verticalDB->end(); i++){
		uTidset *tidset = i->second;
		double support = tidset->getSupport();
		int item = i->first;
		if (support >= minSupRelative){
			oneFrequentItems->push_back(item);
			totalInOneFrequent += tidset->getMap()->size();
		}
	}

	if (oneFrequentItems->size() > 0) {
		
		// double avgTidsetSize =  (totalInOneFrequent / oneFrequentItems->size());
		
		sort(oneFrequentItems->begin(), oneFrequentItems->end(), [this] (int arg0, int arg1) {
			return this->verticalDB->at(arg0)->getSupport() < this->verticalDB->at(arg1)->getSupport() ;
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
	
	vector<uTidset * > * equivalenceClassTidsets = new vector<uTidset * >();

	// populate the vector of set ints corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {
		equivalenceClassTidsets->push_back(verticalDB->at(equivalenceClassItems->at(i)));
		verticalDB->at(equivalenceClassItems->at(i)) = NULL; 
	} 

	delete this->database;

	int * prefixArray = new int[1000];
	
	this->processEquivalenceClassEclat(prefixArray, 0, (double)N, equivalenceClassItems, equivalenceClassTidsets);
	
	delete prefixArray;

}


void uAlgoFramework::constructBITSETS(vector<int> * equivalenceClassItems) {
	
	vector<uBitset * > * equivalenceClassBitsets = new vector<uBitset * >();

	// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {
		
		int item = equivalenceClassItems->at(i);
		uBitset * bs = new uBitset(N);
		
		uTidset * tidset = verticalDB->at(item);
		map<int, double> * tidMap = tidset->getMap();
		for(map<int, double>::iterator i = tidMap->begin(); i != tidMap->end(); i++) {
			bs->insert(i->first, i->second);
		}

		equivalenceClassBitsets->push_back(bs);
	}

	delete this->database;

	int * prefixArray = new int[1000];

	this->processEquivalenceClassViper(prefixArray, 0, (double)N, equivalenceClassItems, equivalenceClassBitsets);
	
	delete prefixArray;

}


void uAlgoFramework::processEquivalenceClassEclat(int * prefix, int prefixLength, double prefixSupport, vector<int> * equivalenceClassItems, vector<uTidset * > * equivalenceClassTidsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> ECLAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

	cout << "Prefix : " ;
	for(int i=0;i<prefixLength;i++)
		cout << prefix[i] << ", " ;
	cout << endl;
	
	rec[1]++;

	int length = prefixLength+1;
	
	if(equivalenceClassItems->size() == 1) {
		
		int item = equivalenceClassItems->at(0);
		uTidset * tidset = equivalenceClassTidsets->at(0);
		double support = tidset->getSupport();
		this->save(prefix, prefixLength, item, support);

		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassTidsets->size();d++) 
			delete equivalenceClassTidsets->at(d);
		delete equivalenceClassTidsets;

		return;
	}

	if(equivalenceClassItems->size() == 2) {
		
		int itemI = equivalenceClassItems->at(0);
		uTidset * tidsetI = equivalenceClassTidsets->at(0);
		double supportI = tidsetI->getSupport();
		this->save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		uTidset * tidsetJ = equivalenceClassTidsets->at(1);
		double supportJ = tidsetJ->getSupport();
		this->save(prefix, prefixLength, itemJ, supportJ);
		
		uTidset * tidsetIJ = this->performINTERSECTION(tidsetI, tidsetJ);
		double supportIJ = tidsetIJ->getSupport();
		
		if(supportIJ >= minSupRelative) {
			int newPrefixLength = prefixLength+1;
			prefix[prefixLength] = itemI;
			this->save(prefix, newPrefixLength, itemJ, supportIJ);
		}

		delete tidsetIJ;
		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassTidsets->size();d++) 
			delete equivalenceClassTidsets->at(d);
		delete equivalenceClassTidsets;

		return;
	}


	for(int i=0; i< equivalenceClassItems->size(); i++) {
		
	// 	int ETotal = 0;
	// 	int DTotal = 0;

		int suffixI = equivalenceClassItems->at(i);
		uTidset * tidsetI = equivalenceClassTidsets->at(i);
		double supportI = tidsetI->getSupport();
		this->save(prefix, prefixLength, suffixI, supportI);
		
		vector<int> * equivalenceClassISuffixItems = new vector<int>();
		vector<uTidset * > * equivalenceClassITidsets = new vector<uTidset * >();

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {
			
			int suffixJ = equivalenceClassItems->at(j);
			uTidset * tidsetJ = equivalenceClassTidsets->at(j);
			double supportJ = tidsetJ->getSupport();
			
			cout << "i : " << suffixI << ", j : " << suffixJ << endl;

			uTidset * tidsetIJ = this->performINTERSECTION(tidsetI, tidsetJ);
			double supportIJ = tidsetIJ->getSupport();
	
			cout << "support ij : " << supportIJ << endl;

			if(supportIJ >= minSupRelative) {
	// 			ETotal += supportIJ;
	// 			DTotal += (supportI - supportIJ);
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
				
				this->processEquivalenceClassEclat(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);

			}
	// 		else {

	// 			// double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
	// 			// double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
	// 			// int ECLATthreshold = (int)(N*(1.0/INTSIZE) );
	// 			// int ECLATstart = 0;
	// 			// int DECLATthreshold  = supportI - ECLATthreshold;
	// 			// int DECLATstart  = supportI;

	// 			//  Estore = INTSIZE*(Etotal + supportI)
	// 			int Estore = INTSIZE*(ETotal + supportI);
	// 			//  Dstore = INTSIZE*(m*supportI - Etotal + (N-supportI) )
	// 			int Dstore = INTSIZE*(equivalenceClassISuffixItems->size()*supportI + N - ETotal - supportI);
	// 			//  Vstore = N*(m+1)*BITSIZE
	// 			int Vstore = (double)N*(equivalenceClassISuffixItems->size()+1.0)*BITSIZE;

	// 			// if(DECLATthreshold <= ECLATthreshold) {

	// 			// 	double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
			
	// 			// 	if(EAvg > PointOfDiff){
						
	// 			// 		// cout << "DECLAT" << endl;
						
	// 			// 		vector<set<int> * > * equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
	// 			// 		set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);

	// 			// 		for(int i=0;i<equivalenceClassITidsets->size();i++)
	// 			// 			delete equivalenceClassITidsets->at(i);
	// 			// 		delete equivalenceClassITidsets;

	// 			// 		this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);	
	// 			// 	}
	// 			// 	else {

	// 			// 		// cout << "ECLAT" << endl;
				
	// 			// 		set<int> * tidsetIClone = new set<int>(tidsetI->begin(), tidsetI->end());
	// 			// 		this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
	// 			// 	}
	// 			// }
	// 			// else {

	// 				cout << Estore << ", " << Vstore << ", " << Dstore << endl;

	// 				if(Estore <= Dstore && Estore <= Vstore){
						
	// 					// cout << "ECLAT" << endl;
					
	// 					set<int> * tidsetIClone = new set<int>(tidsetI->begin(), tidsetI->end());
	// 					this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
	// 				}
	// 				else if(Vstore <= Estore && Vstore <= Dstore){
						
	// 					// cout << "VIPER" << endl;
						
	// 					boost::dynamic_bitset<> * prefixBitset = formPrefixBitsetFromPrefixTidset(tidsetI);
	// 					vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets = convertTIDSETStoBITSETS(equivalenceClassITidsets);

	// 					for(int i=0;i<equivalenceClassITidsets->size();i++)
	// 						delete equivalenceClassITidsets->at(i);
	// 					delete equivalenceClassITidsets;

	// 					this->processEquivalenceClassViper(prefixBitset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
	// 				}
	// 				else{
						
	// 					// System.out.println("DECLAT");
					
	// 					vector<set<int> * > * equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
	// 					set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);
						
	// 					for(int i=0;i<equivalenceClassITidsets->size();i++)
	// 						delete equivalenceClassITidsets->at(i);
	// 					delete equivalenceClassITidsets;

	// 					processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
	// 				}
	// 			// }
	// 		}			
		}
		else {
			delete equivalenceClassISuffixItems;
			delete equivalenceClassITidsets;
		}
	}

	delete equivalenceClassItems;
	for(int d=0;d<equivalenceClassTidsets->size();d++) 
		delete equivalenceClassTidsets->at(d);
	delete equivalenceClassTidsets;

}

uTidset * uAlgoFramework::performINTERSECTION(uTidset * tidsetI, uTidset * tidsetJ) {
	
	uTidset * tidsetIJ = new uTidset();
	map<int, double> * IMap = tidsetI->getMap();
	map<int, double> * JMap = tidsetJ->getMap();

	if( JMap->size() < IMap->size() ){
		for(map<int, double>::iterator i = JMap->begin();i != JMap->end();i++) 
			if(IMap->find(i->first) != IMap->end())
				tidsetIJ->insert( i->first ,(i->second)*(IMap->at(i->first)) );
	}
	else {
		for(map<int, double>::iterator i = IMap->begin();i != IMap->end();i++) 
			if(JMap->find(i->first) != JMap->end())
				tidsetIJ->insert( i->first ,(i->second)*(JMap->at(i->first)));
	}

	return tidsetIJ;
}


void uAlgoFramework::processEquivalenceClassViper(int * prefix, int prefixLength, double prefixSupport, vector<int> * equivalenceClassItems, vector<uBitset * > * equivalenceClassBitsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> VIPER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

	cout << "Prefix : " ;
	for(int i=0;i<prefixLength;i++)
		cout << prefix[i] << ", ";
	cout << endl;
	
	rec[2]++;
	
	int length = prefixLength+1;
	
	if(equivalenceClassItems->size() == 1) {
		
		int item = equivalenceClassItems->at(0);
		uBitset * bitset = equivalenceClassBitsets->at(0);
		save(prefix, prefixLength, item, bitset->getSupport());

		delete equivalenceClassItems;
		// for(int d=0;d<equivalenceClassBitsets->size();d++) 
		// 	delete equivalenceClassBitsets->at(d);
		// delete equivalenceClassBitsets;

		return;
	}

	if(equivalenceClassItems->size() == 2) {
		
		int itemI = equivalenceClassItems->at(0);
		uBitset * bitsetI = equivalenceClassBitsets->at(0);
		double supportI = bitsetI->getSupport();
		save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		uBitset * bitsetJ = equivalenceClassBitsets->at(1);
		double supportJ = bitsetJ->getSupport();
		save(prefix, prefixLength, itemJ, supportJ);
		
		uBitset * bitsetIJ = this->performAND(bitsetI, bitsetJ);
		double supportIJ = bitsetIJ->getSupport();
		
		if(supportIJ >= minSupRelative) {
			int newPrefixLength = prefixLength+1;
			prefix[prefixLength] = itemI;
			save(prefix, newPrefixLength, itemJ, supportIJ);
		}

		delete bitsetIJ;
		delete equivalenceClassItems;
		// for(int d=0;d<equivalenceClassBitsets->size();d++) 
		// 	delete equivalenceClassBitsets->at(d);
		// delete equivalenceClassBitsets;

		return;
	}

	
	for(int i=0; i< equivalenceClassItems->size(); i++) {
		
		// int ETotal = 0;
		// int DTotal = 0;

		int suffixI = equivalenceClassItems->at(i);
		uBitset * bitsetI = equivalenceClassBitsets->at(i);
		double supportI = bitsetI->getSupport();
		this->save(prefix, prefixLength, suffixI, supportI);
		
		vector<int> * equivalenceClassISuffixItems= new vector<int>();
		vector<uBitset * > * equivalenceClassIBitsets = new vector<uBitset * >();

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {
			
			int suffixJ = equivalenceClassItems->at(j);
			uBitset * bitsetJ = equivalenceClassBitsets->at(j);
			int supportJ = bitsetJ->getSupport();
			
			uBitset * bitsetIJ = this->performAND(bitsetI, bitsetJ);
			double supportIJ = bitsetIJ->getSupport();

			if(supportIJ >= minSupRelative) {
				// ETotal += supportIJ;
				// DTotal += (supportI - supportIJ);
				equivalenceClassISuffixItems->push_back(suffixJ);
				equivalenceClassIBitsets->push_back(bitsetIJ);
			}
			else {
				delete bitsetIJ;
			}
		}
		
		if(equivalenceClassISuffixItems->size() > 0) {

			prefix[prefixLength] = suffixI;
			int newPrefixLength = prefixLength+1;
			
			if(algo == 2) {

				this->processEquivalenceClassViper(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);

			}
			// else {

			// 	// double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
			// 	// double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
			// 	// int ECLATthreshold = (int)N*(1.0/INTSIZE);
			// 	// int ECLATstart = 0;
			// 	// int DECLATthreshold  = supportI - ECLATthreshold;
			// 	// int DECLATstart  = supportI;

			// 	int Estore = INTSIZE*(ETotal + supportI);
			// 	int Dstore = INTSIZE*(equivalenceClassISuffixItems->size()*supportI + N - ETotal - supportI);
			// 	int Vstore = (double)N*(equivalenceClassISuffixItems->size()+1.0)*BITSIZE;

			// 	// if(DECLATthreshold <= ECLATthreshold) {
	
			// 	// 	double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
	
			// 	// 	if(EAvg > PointOfDiff){
						
			// 	// 		// cout << "DECLAT" << endl;
					
			// 	// 		vector<set<int> * > * equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
			// 	// 		set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
					
			// 	// 		for(int i=0;i<equivalenceClassIBitsets->size();i++)
			// 	// 			delete equivalenceClassIBitsets->at(i);
			// 	// 		delete equivalenceClassIBitsets;

			// 	// 		this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);

			// 	// 	}
			// 	// 	else {
						
			// 	// 		// cout << "ECLAT" << endl;
					
			// 	// 		vector<set<int> * > * equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
			// 	// 		set<int> * prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
					
			// 	// 		for(int i=0;i<equivalenceClassIBitsets->size();i++)
			// 	// 			delete equivalenceClassIBitsets->at(i);
			// 	// 		delete equivalenceClassIBitsets;

			// 	// 		this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
			// 	// 	}
			// 	// }
			// 	// else {
				
			// 		cout << Estore << ", " << Vstore << ", " << Dstore << endl;

			// 		if(Estore <= Dstore && Estore <= Vstore){
						
			// 			// cout << "ECLAT" << endl;
						
			// 			vector<set<int> * > * equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
			// 			set<int> * prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
					
			// 			for(int i=0;i<equivalenceClassIBitsets->size();i++)
			// 				delete equivalenceClassIBitsets->at(i);
			// 			delete equivalenceClassIBitsets;

			// 			this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
			// 		}
			// 		else if(Vstore <= Estore && Vstore <= Dstore){
						
			// 			// cout << "VIPER" << endl;
	
			// 			boost::dynamic_bitset<> * bitsetIClone = new boost::dynamic_bitset<>(*bitsetI);
			// 			this->processEquivalenceClassViper(bitsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
			// 		}
			// 		else{
						
			// 			// cout << "DECLAT" << endl;
						
			// 			vector<set<int> * > * equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
			// 			set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
					
			// 			for(int i=0;i<equivalenceClassIBitsets->size();i++)
			// 				delete equivalenceClassIBitsets->at(i);
			// 			delete equivalenceClassIBitsets;

			// 			this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
						
			// 		}
			// 	// }
			// }
		}
		else {
			delete equivalenceClassISuffixItems;
			delete equivalenceClassIBitsets;
		}
	}

	// delete prefixBitset;
	delete equivalenceClassItems;
	// for(int d=0;d<equivalenceClassBitsets->size();d++) 
	// 	delete equivalenceClassBitsets->at(d);
	// delete equivalenceClassBitsets;

	return;

}

uBitset * uAlgoFramework::performAND(uBitset * bitsetI, uBitset * bitsetJ) {
	
	uBitset * bitsetIJ = new uBitset(this->N);
	// map<int, double> * IMap = tidsetI->getMap();
	// map<int, double> * JMap = tidsetJ->getMap();

	// if( JMap->size() < IMap->size() ){
	// 	for(map<int, double>::iterator i = JMap->begin();i != JMap->end();i++) 
	// 		if(IMap->find(i->first) != IMap->end())
	// 			tidsetIJ->insert( i->first ,(i->second)*(IMap->at(i->first)) );
	// }
	// else {
	// 	for(map<int, double>::iterator i = IMap->begin();i != IMap->end();i++) 
	// 		if(JMap->find(i->first) != JMap->end())
	// 			tidsetIJ->insert( i->first ,(i->second)*(JMap->at(i->first)));
	// }

	return bitsetIJ;
}






void uAlgoFramework::printStats() {
	cout << "\n\n===================================================" << endl;
	cout << " Transactions count from database : " << N << endl;
	cout << " Frequent itemset count : " << itemsetCount << endl;
	cout << " Usage : ECLAT "  << rec[1] << " VIPER " << rec[2] << " DECLAT " << rec[3] << endl;
	cout << "===================================================" << endl;
}

void uAlgoFramework::save(int * prefix, int prefixLength, int suffixItem, double support) {
	
	itemsetCount++;
	string s = "";

	for(int i=0; i < prefixLength; i++) 
		s = s + (to_string(prefix[i]) + " ") ;
	s = s + (to_string(suffixItem) + " #SUP: " + to_string(support) +  '\n' );

	writer << s;
}

























