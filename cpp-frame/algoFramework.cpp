#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
	
#include "algoFramework.h"
#include "transactionDatabase.h"

using namespace std;

// current ratio is, 16*8 | reduce this to get good results !

static double INTSIZE = 16.0; 		/* implementation by linked list */
static double BITSIZE = 1.0/8.0;	/* dynamic bitset */

algoFramework::algoFramework() {
	this->algo = 0;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;
	treeEdges[0][0] = treeEdges[0][1] = treeEdges[0][2] = 0;
	treeEdges[1][0] = treeEdges[1][1] = treeEdges[1][2] = 0;
	treeEdges[2][0] = treeEdges[2][1] = treeEdges[2][2] = 0;
}


algoFramework::algoFramework(int algo) {
	this->algo = algo;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;
	treeEdges[0][0] = treeEdges[0][1] = treeEdges[0][2] = 0;
	treeEdges[1][0] = treeEdges[1][1] = treeEdges[1][2] = 0;
	treeEdges[2][0] = treeEdges[2][1] = treeEdges[2][2] = 0;
}


void algoFramework::runAlgo(char * outputFile, transactionDatabase * database, double minsupp) {

	this->database = database;
	this->N = database->getN();
	this->M = database->getM();
	this->minSupRelative = (int) ceil(minsupp * N);
	this->verticalDB = database->getVerticalDatabase();
	this->writer.open(outputFile);
	
	itemsetCount = 0;
	long totalInOneFrequent = 0l;
	
	vector<int> * oneFrequentItems = new vector<int>();
	
	for( unordered_map<int, llSet * >::iterator i = verticalDB->begin() ; i != verticalDB->end();i++ ) {
		llSet * tidset = i->second;
		int support = tidset->size();
		int item = i->first;
		if(support >= minSupRelative) {
			oneFrequentItems->push_back(item);
			totalInOneFrequent += tidset->size();
		}
	}

	if(oneFrequentItems->size() > 0) {

		// int avgTidsetSize = (int)(totalInOneFrequent / oneFrequentItems->size());

		sort(oneFrequentItems->begin(), oneFrequentItems->end(), [this] (int arg0, int arg1) {
			return this->verticalDB->at(arg0)->size() < this->verticalDB->at(arg1)->size();
		});

		/* 
		 *	logic to decide the algorithm to begin with 
			we can work with some kind of probvectoric model to determine, with some success, the startting algorithm
		*/

		// int ECLATthreshold  = (int)(N*(1.0/INTSIZE));
		// int DECLATthreshold = (int)(N*((INTSIZE - 1.0)/INTSIZE));

		long Estore = INTSIZE*(long)(totalInOneFrequent + N);
		long Dstore = INTSIZE*(oneFrequentItems->size()*N + N - totalInOneFrequent - N);
		long Vstore = (double)N*(long)(oneFrequentItems->size()+1.0)*BITSIZE;

		cout << Estore << ", " << Vstore << ", " << Dstore << endl;
		cout << oneFrequentItems->size() << ", " << N << endl;

		if(algo == 0) {
			if(Estore <= Dstore && Estore <= Vstore ) {
				// cout << "ECLAT" << endl;
				// this->constructTIDSETS(oneFrequentItems);
				this->constructTIDSETS(oneFrequentItems);
			}
			else if(Vstore <= Estore && Vstore <= Dstore){
				// cout << "VIPER" << endl;
				this->constructBITSETS(oneFrequentItems);
			}
			else{
				// cout << "DECLAT" << endl;
				this->constructDIFFSETS(oneFrequentItems);
			}
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
			else if(algo == 3){
				// cout << "DECLAT" << endl;
				this->constructDIFFSETS(oneFrequentItems);
			}	
		}
	}

	writer.close();
	
	this->printStats();
}


void algoFramework::constructTIDSETS(vector<int> * equivalenceClassItems) {
		
	vector<llSet * > * equivalenceClassTidsets = new vector<llSet * >();

	// populate the vector of set ints corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {
		equivalenceClassTidsets->push_back(verticalDB->at(equivalenceClassItems->at(i)));
		verticalDB->at(equivalenceClassItems->at(i)) = NULL; 
	} 

	llSet * enot = new llSet();
	for(int i=0;i<N;i++) 
		enot->addElement(i);

	delete this->database;

	int * prefixArray = new int[1000];
	
	this->processEquivalenceClassEclat(enot ,prefixArray, 0, N, equivalenceClassItems, equivalenceClassTidsets);
	
	delete prefixArray;
}	


void algoFramework::constructBITSETS(vector<int> * equivalenceClassItems) {

	vector<boost::dynamic_bitset<> * > * equivalenceClassBitsets = new vector<boost::dynamic_bitset<> * >();

	// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {
		int item = equivalenceClassItems->at(i);
		boost::dynamic_bitset<> * bs = new boost::dynamic_bitset<> (N);
		llSet * tidset = verticalDB->at(item);
		for(llSet_element * ele = tidset->getFirst();ele != NULL;ele = ele->getNext()) {
			bs->set(ele->getValue());
		}
		equivalenceClassBitsets->push_back(bs);
	}

	boost::dynamic_bitset<> * enot = new boost::dynamic_bitset<>(N);
	for(int i=0;i<N;i++) {
		enot->set(i);
	}

	delete this->database;

	int * prefixArray = new int[1000];

	this->processEquivalenceClassViper(enot, prefixArray, 0, N, equivalenceClassItems, equivalenceClassBitsets);
	
	delete prefixArray;
}


void algoFramework::constructDIFFSETS(vector<int> * equivalenceClassItems) {
	
	vector<llSet * > * equivalenceClassDiffsets = new vector<llSet * >();

	// populate the vector of set ints corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {
		llSet * tidset = verticalDB->at(equivalenceClassItems->at(i));
		llSet * diffset = new llSet();
		llSet_element * ptr = tidset->getFirst();

		for(int i=0;i<N;i++) {
			
			if(ptr != NULL) {
				if(ptr->getValue() > i)
					diffset->addElement(i);
				else {
					ptr = ptr->getNext();
				}
			}
			else 
				diffset->addElement(i);	
		}

		equivalenceClassDiffsets->push_back(diffset); 
	} 
	
	delete this->database;

	int * prefixArray = new int[1000];

	this->processEquivalenceClassDEclat(new llSet(), prefixArray, 0, N, equivalenceClassItems, equivalenceClassDiffsets);

	delete prefixArray;
}


void algoFramework::processEquivalenceClassEclat(llSet * prefixTidset, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<llSet * > * equivalenceClassTidsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> ECLAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

	cout << "Prefix : " ;
	for(int i=0;i<prefixLength;i++)
		cout << prefix[i] << ", " ;
	cout << endl;
	
	rec[1]++;

	int length = prefixLength+1;
	
	if(equivalenceClassItems->size() == 1) {
		
		int item = equivalenceClassItems->at(0);
		llSet * tidset = equivalenceClassTidsets->at(0);
		int support = tidset->size();
		this->save(prefix, prefixLength, item, support);

		delete prefixTidset;
		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassTidsets->size();d++) 
			delete equivalenceClassTidsets->at(d);
		delete equivalenceClassTidsets;

		return;
	}

	if(equivalenceClassItems->size() == 2) {
		
		int itemI = equivalenceClassItems->at(0);
		llSet * tidsetI = equivalenceClassTidsets->at(0);
		int supportI = tidsetI->size();
		this->save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		llSet * tidsetJ = equivalenceClassTidsets->at(1);
		int supportJ = tidsetJ->size();
		this->save(prefix, prefixLength, itemJ, supportJ);
		
		llSet * tidsetIJ = this->performINTERSECTION(tidsetI, tidsetJ);
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
		llSet * tidsetI = equivalenceClassTidsets->at(i);
		int supportI = tidsetI->size();
		this->save(prefix, prefixLength, suffixI, supportI);
		
		vector<int> * equivalenceClassISuffixItems = new vector<int>();
		vector<llSet * > * equivalenceClassITidsets = new vector<llSet * >();

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {
			
			int suffixJ = equivalenceClassItems->at(j);
			llSet * tidsetJ = equivalenceClassTidsets->at(j);
			int supportJ = tidsetJ->size();
			
			llSet * tidsetIJ = this->performINTERSECTION(tidsetI, tidsetJ);
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
				
				treeEdges[0][0]++;				
				llSet * tidsetIClone = new llSet( tidsetI->getFirst() );
				
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
				int Dstore = INTSIZE*(DTotal + N - supportI);
				//  Vstore = N*(m+1)*BITSIZE
				int Vstore = (double)N*(equivalenceClassISuffixItems->size()+1.0)*BITSIZE;

				// if(DECLATthreshold <= ECLATthreshold) {

				// 	double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
			
				// 	if(EAvg > PointOfDiff){
						
				// 		// cout << "DECLAT" << endl;
						
				// 		vector<unordered_set<int> * > * equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
				// 		unordered_set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);

				// 		for(int i=0;i<equivalenceClassITidsets->size();i++)
				// 			delete equivalenceClassITidsets->at(i);
				// 		delete equivalenceClassITidsets;

				// 		this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);	
				// 	}
				// 	else {

				// 		// cout << "ECLAT" << endl;
				
				// 		unordered_set<int> * tidsetIClone = new unordered_set<int>(tidsetI->begin(), tidsetI->end());
				// 		this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				// 	}
				// }
				// else {

				cout << Estore << ", " << Vstore << ", " << Dstore << endl;

				if(Estore <= Dstore && Estore <= Vstore){
					
					// cout << "ECLAT" << endl;
					treeEdges[0][0]++;

					llSet * tidsetIClone = new llSet(tidsetI->getFirst() );

					this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				}
				else if(Vstore <= Estore && Vstore <= Dstore){
					
					// cout << "VIPER" << endl;
					treeEdges[0][1]++;					
				
					boost::dynamic_bitset<> * prefixBitset = formPrefixBitsetFromPrefixTidset(tidsetI);
					vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets = convertTIDSETStoBITSETS(equivalenceClassITidsets);

					for(int i=0;i<equivalenceClassITidsets->size();i++)
						delete equivalenceClassITidsets->at(i);
					delete equivalenceClassITidsets;

					this->processEquivalenceClassViper(prefixBitset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
				}
				else{
					
					// System.out.println("DECLAT");
					treeEdges[0][2]++;

					vector<llSet * > * equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
					llSet * parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);
					
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


llSet * algoFramework::performINTERSECTION(llSet * tidsetI, llSet * tidsetJ) {
	
	llSet * tidsetIJ = new llSet();

	llSet_element * ptrA, * ptrB;
	ptrA = tidsetI->getFirst();
	ptrB = tidsetJ->getFirst();

	while(ptrA != NULL && ptrB != NULL) {
		if(ptrA->getValue() < ptrB->getValue()) {
			ptrA = ptrA->getNext();
		}
		else if(ptrA->getValue() > ptrB->getValue()) {
			ptrB = ptrB->getNext();
		}
		else {
			tidsetIJ->addElement(ptrA->getValue());
			ptrA = ptrA->getNext();
			ptrB = ptrB->getNext();
		}
	}

	return tidsetIJ;
}


vector<llSet * > * algoFramework::convertTIDSETStoDIFFSETS(llSet * prefixTidset, vector<llSet * > * equivalenceClassITidsets) {
	
	vector<llSet * > * equivalenceClassIDiffsets = new vector<llSet * >();
	
	for(int i=0;i<equivalenceClassITidsets->size();i++) {
		llSet * tidset = equivalenceClassITidsets->at(i);
		llSet * diffset = new llSet();
		
		llSet_element * ptr = tidset->getFirst();

		for(llSet_element * ele = prefixTidset->getFirst();ele != NULL;ele = ele->getNext()) {
			int tid = ele->getValue();
			if(ptr != NULL) {
				if(ptr->getValue() > tid)
					diffset->addElement(tid);
				else
					ptr = ptr->getNext();
			}
			else
				diffset->addElement(tid);
		}

		// for( unordered_set<int>::iterator j = prefixTidset->begin(); j != prefixTidset->end() ; j++ ) {
		// 	int tid = *j;
		// 	if( tidset->find(tid) == tidset->end())
		// 		diffset->insert(tid);
		// }

		equivalenceClassIDiffsets->push_back(diffset);
	}

	return equivalenceClassIDiffsets;
}


llSet * algoFramework::formParentDiffsUnionFromPrefixTidset(llSet * prefixTidset) {

	llSet * parentDiffsUnion = new llSet();
	llSet_element * ptr;
	ptr = prefixTidset->getFirst();

	for(int i=0;i<N;i++) {
		
		if(ptr != NULL) {
			if(ptr->getValue() > i)
				parentDiffsUnion->addElement(i);
			else
				ptr = ptr->getNext();
		}
		else
			parentDiffsUnion->addElement(i);

	}

	return parentDiffsUnion;
}


vector<boost::dynamic_bitset<> *> * algoFramework::convertTIDSETStoBITSETS(vector<llSet * > * equivalenceClassITidsets) {
	
	vector<boost::dynamic_bitset<> *> * equivalenceClassIBitsets = new vector<boost::dynamic_bitset<> * >();

	for(int i=0;i<equivalenceClassITidsets->size();i++) {
		llSet * tidset = equivalenceClassITidsets->at(i);
		boost::dynamic_bitset<> * bs = new boost::dynamic_bitset<>(N);
		for( llSet_element * ele = tidset->getFirst(); ele != NULL; ele = ele->getNext() ) {
			bs->set(ele->getValue());
		}
		equivalenceClassIBitsets->push_back(bs);
	}

	return equivalenceClassIBitsets;
}


boost::dynamic_bitset<> * algoFramework::formPrefixBitsetFromPrefixTidset(llSet * prefixTidset) {
	
	boost::dynamic_bitset<> * prefixBitset = new boost::dynamic_bitset<>(N);

	for( llSet_element * ele = prefixTidset->getFirst(); ele != NULL ; ele = ele->getNext() ) {
		prefixBitset->set(ele->getValue());
	}

	return prefixBitset;
}


void algoFramework::processEquivalenceClassViper(boost::dynamic_bitset<> * prefixBitset, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<boost::dynamic_bitset<> * > * equivalenceClassBitsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> VIPER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

	cout << "Prefix : " ;
	for(int i=0;i<prefixLength;i++)
		cout << prefix[i] << ", ";
	cout << endl;
	
	rec[2]++;
	
	int length = prefixLength+1;
	
	if(equivalenceClassItems->size() == 1) {
		
		int item = equivalenceClassItems->at(0);
		boost::dynamic_bitset<> * bitset = equivalenceClassBitsets->at(0);
		save(prefix, prefixLength, item, bitset->count());

		delete prefixBitset;
		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassBitsets->size();d++) 
			delete equivalenceClassBitsets->at(d);
		delete equivalenceClassBitsets;

		return;
	}

	if(equivalenceClassItems->size() == 2) {
		
		int itemI = equivalenceClassItems->at(0);
		boost::dynamic_bitset<> * bitsetI = equivalenceClassBitsets->at(0);
		int supportI = bitsetI->count();
		save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		boost::dynamic_bitset<> * bitsetJ = equivalenceClassBitsets->at(1);
		int supportJ = bitsetJ->count();
		save(prefix, prefixLength, itemJ, supportJ);
		
		boost::dynamic_bitset<> * bitsetIJ = this->performAND(bitsetI, bitsetJ);
		int supportIJ = bitsetIJ->count();
		
		if(supportIJ >= minSupRelative) {
			int newPrefixLength = prefixLength+1;
			prefix[prefixLength] = itemI;
			save(prefix, newPrefixLength, itemJ, supportIJ);
		}

		delete bitsetIJ;
		delete prefixBitset;
		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassBitsets->size();d++) 
			delete equivalenceClassBitsets->at(d);
		delete equivalenceClassBitsets;

		return;
	}

	
	for(int i=0; i< equivalenceClassItems->size(); i++) {
		
		int ETotal = 0;
		int DTotal = 0;

		int suffixI = equivalenceClassItems->at(i);
		boost::dynamic_bitset<> * bitsetI = equivalenceClassBitsets->at(i);
		int supportI = bitsetI->count();
		this->save(prefix, prefixLength, suffixI, supportI);
		
		vector<int> * equivalenceClassISuffixItems= new vector<int>();
		vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets = new vector<boost::dynamic_bitset<> * >();

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {
			
			int suffixJ = equivalenceClassItems->at(j);
			boost::dynamic_bitset<> * bitsetJ = equivalenceClassBitsets->at(j);
			int supportJ = bitsetJ->count();
			
			boost::dynamic_bitset<> * bitsetIJ = this->performAND(bitsetI, bitsetJ);
			int supportIJ = bitsetIJ->count();

			if(supportIJ >= minSupRelative) {
				ETotal += supportIJ;
				DTotal += (supportI - supportIJ);
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

				treeEdges[1][1]++;

				boost::dynamic_bitset<> * bitsetIClone = new boost::dynamic_bitset<>(*bitsetI);
				this->processEquivalenceClassViper(bitsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);

			}
			else {

				// double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
				// double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
				// int ECLATthreshold = (int)N*(1.0/INTSIZE);
				// int ECLATstart = 0;
				// int DECLATthreshold  = supportI - ECLATthreshold;
				// int DECLATstart  = supportI;

				int Estore = INTSIZE*(ETotal + supportI);
				int Dstore = INTSIZE*(DTotal + N - supportI);
				int Vstore = (double)N*(equivalenceClassISuffixItems->size()+1.0)*BITSIZE;

				// if(DECLATthreshold <= ECLATthreshold) {
	
				// 	double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
	
				// 	if(EAvg > PointOfDiff){
						
				// 		// cout << "DECLAT" << endl;
					
				// 		vector<unordered_set<int> * > * equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
				// 		unordered_set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
					
				// 		for(int i=0;i<equivalenceClassIBitsets->size();i++)
				// 			delete equivalenceClassIBitsets->at(i);
				// 		delete equivalenceClassIBitsets;

				// 		this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);

				// 	}
				// 	else {
						
				// 		// cout << "ECLAT" << endl;
					
				// 		vector<unordered_set<int> * > * equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
				// 		unordered_set<int> * prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
					
				// 		for(int i=0;i<equivalenceClassIBitsets->size();i++)
				// 			delete equivalenceClassIBitsets->at(i);
				// 		delete equivalenceClassIBitsets;

				// 		this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				// 	}
				// }
				// else {
				
				cout << Estore << ", " << Vstore << ", " << Dstore << endl;

				if(Estore <= Dstore && Estore <= Vstore){
					
					// cout << "ECLAT" << endl;
					treeEdges[1][0]++;
					
					vector<llSet * > * equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
					llSet * prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
				
					for(int i=0;i<equivalenceClassIBitsets->size();i++)
						delete equivalenceClassIBitsets->at(i);
					delete equivalenceClassIBitsets;

					this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				}
				else if(Vstore <= Estore && Vstore <= Dstore){
					
					// cout << "VIPER" << endl;
					treeEdges[1][1]++;

					boost::dynamic_bitset<> * bitsetIClone = new boost::dynamic_bitset<>(*bitsetI);
					this->processEquivalenceClassViper(bitsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
				}
				else{
					
					// cout << "DECLAT" << endl;
					treeEdges[1][2]++;

					vector<llSet * > * equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
					llSet * parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
				
					for(int i=0;i<equivalenceClassIBitsets->size();i++)
						delete equivalenceClassIBitsets->at(i);
					delete equivalenceClassIBitsets;

					this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					
				}
				// }
			}
		}
		else {
			delete equivalenceClassISuffixItems;
			delete equivalenceClassIBitsets;
		}
	}

	delete prefixBitset;
	delete equivalenceClassItems;
	for(int d=0;d<equivalenceClassBitsets->size();d++) 
		delete equivalenceClassBitsets->at(d);
	delete equivalenceClassBitsets;


}


boost::dynamic_bitset<> * algoFramework::performAND(boost::dynamic_bitset<> * bitsetI, boost::dynamic_bitset<> * bitsetJ) {
	
	boost::dynamic_bitset<> * bitsetIJ = new boost::dynamic_bitset<>( (*bitsetI)&(*bitsetJ) );
	return bitsetIJ;
}


vector<llSet * > * algoFramework::convertBITSETStoTIDSETS(vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets) {
	
	vector<llSet * > * equivalenceClassITidsets = new vector<llSet * >();

	for(int i=0;i<equivalenceClassIBitsets->size();i++) {
		boost::dynamic_bitset<> * bitst = equivalenceClassIBitsets->at(i); 
		llSet * tidset = new llSet();

		for(int i = bitst->find_first(); i >= 0; i = bitst->find_next(i)) {
			tidset->addElement(i);
		}
		
		equivalenceClassITidsets->push_back(tidset);
	}
	return equivalenceClassITidsets;
}


llSet * algoFramework::formPrefixTidsetFromPrefixBitsets(boost::dynamic_bitset<> * prefixBitset) {
	
	llSet * prefixTidset = new llSet();

	for(int i = prefixBitset->find_first(); i >= 0; i = prefixBitset->find_next(i)) {
		prefixTidset->addElement(i);
	}

	return prefixTidset;
}


vector<llSet * > * algoFramework::convertBITSETStoDIFFSETS(boost::dynamic_bitset<> * prefixBitset, vector<boost::dynamic_bitset<> *> * equivalenceClassIBitsets) {
	
	vector<llSet * > * equivalenceClassIDiffsets = new vector<llSet * >();
	
	for(int i=0; i < equivalenceClassIBitsets->size(); i++) {
		boost::dynamic_bitset<> * bitset = equivalenceClassIBitsets->at(i);
		llSet * diffset = new llSet();
		for(int i = prefixBitset->find_first(); i >= 0; i = prefixBitset->find_next(i))
			if(!bitset->test(i))
				diffset->addElement(i);
		equivalenceClassIDiffsets->push_back(diffset);
	}

	return equivalenceClassIDiffsets;
}


llSet * algoFramework::formParentDiffsUnionFromPrefixBitset(boost::dynamic_bitset<> * prefixBitset) {

	llSet * parentDiffsUnion = new llSet();
	for(int i=0;i>N;i++) {
		if(!prefixBitset->test(i))
			parentDiffsUnion->addElement(i);
	}

	return parentDiffsUnion;
}


void algoFramework::processEquivalenceClassDEclat(llSet * parentDiffsUnion, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<llSet * > * equivalenceClassDiffsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> DECLAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

	cout << "Prefix : ";
	for(int i=0;i<prefixLength;i++)
		cout << prefix[i] << ", ";
	cout << endl;
	
	rec[3]++;
	
	int length = prefixLength+1;

	
	if(equivalenceClassItems->size() == 1) {
		
		int item = equivalenceClassItems->at(0);
		llSet * diffset = equivalenceClassDiffsets->at(0);
		int support = prefixSupport - diffset->size();
		save(prefix, prefixLength, item, support);

		delete parentDiffsUnion;
		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassDiffsets->size();d++) 
			delete equivalenceClassDiffsets->at(d);
		delete equivalenceClassDiffsets;

		return;
	}


	if(equivalenceClassItems->size() == 2) {
		
		int itemI = equivalenceClassItems->at(0);
		llSet * diffsetI = equivalenceClassDiffsets->at(0);
		int supportI = prefixSupport - diffsetI->size();
		save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		llSet * diffsetJ = equivalenceClassDiffsets->at(1);
		int supportJ = prefixSupport - diffsetJ->size();
		save(prefix, prefixLength, itemJ, supportJ);
		
		llSet * diffsetIJ = this->performDIFFERENCE(diffsetI, diffsetJ);
		int supportIJ = supportI - diffsetIJ->size();
		
		if(supportIJ >= minSupRelative) {
			int newPrefixLength = prefixLength+1;
			prefix[prefixLength] = itemI;
			save(prefix, newPrefixLength, itemJ, supportIJ);
		}

		delete diffsetIJ;
		delete parentDiffsUnion;
		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassDiffsets->size();d++) 
			delete equivalenceClassDiffsets->at(d);
		delete equivalenceClassDiffsets;

		return;
	}

	for(int i=0; i< equivalenceClassItems->size(); i++) {
		
		int ETotal = 0;
		int DTotal = 0;

		int suffixI = equivalenceClassItems->at(i);
		llSet * diffsetI = equivalenceClassDiffsets->at(i);
		int supportI = prefixSupport - diffsetI->size();
		save(prefix, prefixLength, suffixI, supportI);
		
		vector<int> * equivalenceClassISuffixItems = new vector<int>();
		vector<llSet * > * equivalenceClassIDiffsets = new vector<llSet * >();

		llSet * newParentDiffsUnion = new llSet();

		llSet_element * ptrA = parentDiffsUnion->getFirst();
		llSet_element * ptrB = diffsetI->getFirst();

		// cout << "parentDiffsUnion : " << endl;
		// parentDiffsUnion->print();
		// cout << "diffsetI : " << endl;
		// diffsetI->print();

		while(ptrA != NULL && ptrB != NULL) {
			if(ptrA->getValue() < ptrB->getValue()) {
				newParentDiffsUnion->addElement(ptrA->getValue());
				ptrA = ptrA->getNext();
			}
			else if(ptrA->getValue() > ptrB->getValue()) {
				newParentDiffsUnion->addElement(ptrB->getValue());
				ptrB = ptrB->getNext();
			}
			else {
				newParentDiffsUnion->addElement(ptrB->getValue());
				ptrB = ptrB->getNext();	
				ptrA = ptrA->getNext();	
			}
		}

		while(ptrA != NULL) {
			newParentDiffsUnion->addElement(ptrA->getValue());
			ptrA = ptrA->getNext();
		}

		while(ptrB != NULL) {
			newParentDiffsUnion->addElement(ptrB->getValue());
			ptrB = ptrB->getNext();
		}

		// cout << "newParentDiffsUnion : " << endl;
		// newParentDiffsUnion->print();

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {
			
			int suffixJ = equivalenceClassItems->at(j);
			llSet * diffsetJ = equivalenceClassDiffsets->at(j);
			int supportJ = prefixSupport - diffsetJ->size();
			
			llSet * diffsetIJ = this->performDIFFERENCE(diffsetI, diffsetJ);
			int supportIJ = supportI - diffsetIJ->size();
				
			if(supportIJ >= minSupRelative) {
				ETotal += supportIJ;
				DTotal += (supportI - supportIJ);
				equivalenceClassISuffixItems->push_back(suffixJ);
				equivalenceClassIDiffsets->push_back(diffsetIJ);
			}
			else {
				delete diffsetIJ;
			}
		}

		
		if(equivalenceClassISuffixItems->size() > 0) {

			prefix[prefixLength] = suffixI;
			int newPrefixLength = prefixLength+1;


			if(algo == 3) {

				treeEdges[2][2]++;

				this->processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);

			}
			else {

				// double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
				// double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
				// int ECLATthreshold = (int)N*(1.0/INTSIZE) ;
				// int ECLATstart = 0;
				// int DECLATthreshold  = supportI - ECLATthreshold;
				// int DECLATstart  = supportI;

				int Estore = INTSIZE*(ETotal + supportI);
				int Dstore = INTSIZE*(DTotal + N - supportI);
				int Vstore = (double)N*(equivalenceClassISuffixItems->size()+1.0)*BITSIZE;

				// if(DECLATthreshold <= ECLATthreshold) {

				// 	double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;

				// 	if(EAvg > PointOfDiff){
						
				// 		// cout << "DECLAT" << endl;
						
				// 		this->processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
				// 	}
				// 	else {
						
				// 		// cout << "ECLAT" << endl;

				// 		vector<unordered_set<int> * > * equivalenceClassITidsets = convertDIFFSETStoTIDSETS(newParentDiffsUnion, equivalenceClassIDiffsets);
				// 		unordered_set<int> * prefixTidset = formPrefixTidsetFromParentDiffsUnion(newParentDiffsUnion);
						
				// 		for(int i=0;i<equivalenceClassIDiffsets->size();i++)
				// 			delete equivalenceClassIDiffsets->at(i);
				// 		delete equivalenceClassIDiffsets;
				// 		delete newParentDiffsUnion;
						
				// 		this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				// 	}
				// }
				// else {

				cout << Estore << ", " << Vstore << ", " << Dstore << endl;

				if(Estore <= Dstore && Estore <= Vstore){
					
					// cout << "ECLAT" << endl;
					treeEdges[2][0]++;
					
					vector<llSet * > * equivalenceClassITidsets = convertDIFFSETStoTIDSETS(newParentDiffsUnion, equivalenceClassIDiffsets);
					llSet * prefixTidset = formPrefixTidsetFromParentDiffsUnion(newParentDiffsUnion);

					for(int i=0;i<equivalenceClassIDiffsets->size();i++)
						delete equivalenceClassIDiffsets->at(i);
					delete equivalenceClassIDiffsets;
					delete newParentDiffsUnion;
					
					this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				}
				else if(Vstore <= Estore && Vstore <= Dstore){
					
					// cout << "VIPER" << endl;
					treeEdges[2][1]++;
					
					vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets = convertDIFFSETStoBITSETS(newParentDiffsUnion, equivalenceClassIDiffsets);
					boost::dynamic_bitset<> * prefixBitset = formPrefixBitsetFromParentDiffsUnion(newParentDiffsUnion);

					for(int i=0;i<equivalenceClassIDiffsets->size();i++)
						delete equivalenceClassIDiffsets->at(i);
					delete equivalenceClassIDiffsets;
					delete newParentDiffsUnion;
					
					this->processEquivalenceClassViper(prefixBitset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
				}
				else{
					
					// cout << "DECLAT" << endl;
					treeEdges[2][2]++;

					this->processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
				}
				// }
			}
		}
		else {
			delete newParentDiffsUnion;
			delete equivalenceClassISuffixItems;
			delete equivalenceClassIDiffsets;
		}
	}	

	delete parentDiffsUnion;
	delete equivalenceClassItems;
	for(int d=0;d<equivalenceClassDiffsets->size();d++) 
		delete equivalenceClassDiffsets->at(d);
	delete equivalenceClassDiffsets;


}


// diff(PXY) = diff(PY) - DIFF(PX)	
llSet * algoFramework::performDIFFERENCE(llSet * diffsetI, llSet * diffsetJ) {
	
	llSet * diffsetIJ = new llSet();
	llSet_element * ptrA = diffsetI->getFirst();
	llSet_element * ptrB = diffsetJ->getFirst();
	
	// cout << "diffset i" << endl;
	// diffsetI->print();
	// cout << "diffset j" << endl;
	// diffsetJ->print();

	while(ptrB != NULL) {

		if(ptrA != NULL) {
			if(ptrA->getValue() < ptrB->getValue() ) {
				ptrA = ptrA->getNext();
			}
			else if(ptrA->getValue() > ptrB->getValue() ) {
				diffsetIJ->addElement(ptrB->getValue());
				ptrB = ptrB->getNext();
			}
			else {
				ptrB = ptrB->getNext();
				ptrA = ptrA->getNext();
			}
		}
		else {
			diffsetIJ->addElement(ptrB->getValue());
			ptrB = ptrB->getNext();		
		}
	
	}

	// cout << "diffset ij" << endl;
	// diffsetIJ->print();

	// for(unordered_set<int>::iterator i = diffsetJ->begin(); i != diffsetJ->end(); i++){ 
	// 	int tid = *i;
	// 	if( diffsetI->find(tid) == diffsetI->end() )
	// 		diffsetIJ->insert(tid);
	// }

	return diffsetIJ;

}


vector<llSet *> * algoFramework::convertDIFFSETStoTIDSETS(llSet * parentDiffsUnion, vector<llSet * > * equivalenceClassIDiffsets) {

	vector<llSet * > * equivalenceClassITidsets = new vector<llSet * >();

	for( int i=0;i<equivalenceClassIDiffsets->size();i++ ) {
		llSet * diffset = equivalenceClassIDiffsets->at(i);
		llSet * tidset = new llSet();
		llSet_element * ptrA = parentDiffsUnion->getFirst();
		llSet_element * ptrB = diffset->getFirst();
		
		for(int j=0; j<N; j++) {

			if(ptrA != NULL && ptrB != NULL) {
				if(ptrA->getValue() > j && ptrB->getValue() > j) 
					tidset->addElement(j);
				else {
					if(ptrA->getValue() == j)
						ptrA = ptrA->getNext();
					if(ptrB->getValue() == j)
						ptrB = ptrB->getNext();
				}
			}
			else if(ptrA == NULL && ptrB != NULL) {
				if(ptrB->getValue() > j) 
					tidset->addElement(j);
				else
					ptrB = ptrB->getNext();
			}
			else if(ptrB == NULL && ptrA != NULL) {
				if(ptrA->getValue() > j) 
					tidset->addElement(j);
				else
					ptrA = ptrA->getNext();
			}
			else {
				tidset->addElement(j);
			}


			// if(parentDiffsUnion->find(i) == parentDiffsUnion->end() && diffset->find(i) == diffset->end())
				// tidset->insert(i);
		}


		equivalenceClassITidsets->push_back(tidset);
	}

	return equivalenceClassITidsets;
}


llSet * algoFramework::formPrefixTidsetFromParentDiffsUnion(llSet * parentDiffsUnion) {

	llSet * prefixTidset = new llSet();
	llSet_element * ptr = parentDiffsUnion->getFirst();

	for(int i=0;i<N;i++) {
		if(ptr != NULL) {
			if(ptr->getValue() > i)
				prefixTidset->addElement(i);
			else
				ptr = ptr->getNext();
		}
		else
			prefixTidset->addElement(i);
	}

	return prefixTidset;
}


vector<boost::dynamic_bitset<> * > * algoFramework::convertDIFFSETStoBITSETS(llSet * parentDiffsUnion, vector<llSet * > * equivalenceClassIDiffsets) {

	vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets = new vector<boost::dynamic_bitset<> * >();

	for(int i=0;i<equivalenceClassIDiffsets->size();i++) {
		llSet * diffset = equivalenceClassIDiffsets->at(i);
		boost::dynamic_bitset<> * bs = new boost::dynamic_bitset<>(N);
		llSet_element * ptrA = parentDiffsUnion->getFirst();
		llSet_element * ptrB = diffset->getFirst();

		for(int j=0;j<N;j++) {
			
			if(ptrA != NULL && ptrB != NULL) {
				if(ptrA->getValue() > j && ptrB->getValue() > j) 
					bs->set(j);
				else {
					if(ptrA->getValue() == j)
						ptrA = ptrA->getNext();
					if(ptrB->getValue() == j)
						ptrB = ptrB->getNext();
				}
			}
			else if(ptrA == NULL && ptrB != NULL) {
				if(ptrB->getValue() > j) 
					bs->set(j);
				else
					ptrB = ptrB->getNext();
			}
			else if(ptrB == NULL && ptrA != NULL) {
				if(ptrA->getValue() > j) 
					bs->set(j);
				else
					ptrA = ptrA->getNext();
			}
			else {
				bs->set(j);
			}


			// if(parentDiffsUnion->find(i) == parentDiffsUnion->end() && diffset->find(i) == diffset->end()) {
			// 	bs->set(i);
			// }
		}
		equivalenceClassIBitsets->push_back(bs);
	}

	return equivalenceClassIBitsets;
}


boost::dynamic_bitset<> * algoFramework::formPrefixBitsetFromParentDiffsUnion(llSet * parentDiffsUnion) {

	boost::dynamic_bitset<> * prefixBitset = new boost::dynamic_bitset<>(N);
	llSet_element * ptr = parentDiffsUnion->getFirst();
	for(int i=0;i<N;i++) {
		
		if(ptr != NULL) {
			if(ptr->getValue() > i)
				prefixBitset->set(i);
			else
				ptr = ptr->getNext();
		}
		else
			prefixBitset->set(i);


	}

	return prefixBitset;
}


void algoFramework::printStats() {
	cout << "\n\n===================================================" << endl;
	cout << " Transactions count from database : " << N << endl;
	cout << " Frequent itemset count : " << itemsetCount << endl;
	cout << " Usage : ECLAT "  << rec[1] << " VIPER " << rec[2] << " DECLAT " << rec[3] << endl;
	cout << " E->E : " << treeEdges[0][0] << "\tE->V : " << treeEdges[0][1] << "\tE->D : " << treeEdges[0][2] << endl;
	cout << " V->E : " << treeEdges[1][0] << "\tV->V : " << treeEdges[1][1] << "\tV->D : " << treeEdges[1][2] << endl;
	cout << " D->E : " << treeEdges[2][0] << "\tD->V : " << treeEdges[2][1] << "\tD->D : " << treeEdges[2][2] << endl;
	cout << "===================================================" << endl;
}


void algoFramework::save(int * prefix, int prefixLength, int suffixItem, int support) {
	
	itemsetCount++;
	string s = "";

	for(int i=0; i < prefixLength; i++) 
		s = s + (to_string(prefix[i]) + " ") ;
	s = s + (to_string(suffixItem) + " #SUP: " + to_string(support) +  '\n' );

	writer << s;
}

