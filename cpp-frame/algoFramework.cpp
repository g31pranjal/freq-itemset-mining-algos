#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
	
#include "algoFramework.h"
#include "transactionDatabase.h"

using namespace std;

static int INTSIZE = 32;


algoFramework::algoFramework() {
	this->algo = 0;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;
}


algoFramework::algoFramework(int algo) {
	this->algo = algo;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;
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
	
	for( map<int, set<int> * >::iterator i = verticalDB->begin() ; i != verticalDB->end();i++ ) {
		set<int> * tidset = i->second;
		int support = tidset->size();
		int item = i->first;
		if(support >= minSupRelative) {
			oneFrequentItems->push_back(item);
			totalInOneFrequent += tidset->size();
		}
	}


	if(oneFrequentItems->size() > 0) {

		int avgTidsetSize = (int)(totalInOneFrequent / oneFrequentItems->size());

		sort(oneFrequentItems->begin(), oneFrequentItems->end(), [this] (int arg0, int arg1) {
			return this->verticalDB->at(arg0)->size() < this->verticalDB->at(arg1)->size();
		});

		/* 
		 *	logic to decide the algorithm to begin with 
			we can work with some kind of probvectoric model to determine, with some success, the startting algorithm
		*/

		int ECLATthreshold  = (int)(N*(1.0/INTSIZE));
		int DECLATthreshold = (int)(N*((INTSIZE - 1.0)/INTSIZE));

		if(algo == 0) {
			if(avgTidsetSize <= ECLATthreshold ) {
				// cout << "ECLAT" << endl;
				this->constructTIDSETS(oneFrequentItems);
			}
			else if(avgTidsetSize <= DECLATthreshold){
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
		
	vector<set<int> * > * equivalenceClassTidsets = new vector<set<int> * >();

	// populate the vector of set ints corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {
		equivalenceClassTidsets->push_back(verticalDB->at(equivalenceClassItems->at(i)));
		verticalDB->at(equivalenceClassItems->at(i)) = NULL; 
	} 

	set<int> * enot = new set<int>();
	for(int i=0;i<N;i++) 
		enot->insert(i);

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
		set<int> * tidset = verticalDB->at(item);
		for(set<int>::iterator i = tidset->begin();i != tidset->end();i++) {
			bs->set(*i);
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
	
	vector<set<int> * > * equivalenceClassDiffsets = new vector<set<int> * >();

	// populate the vector of set ints corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {
		set<int> * tidset = verticalDB->at(equivalenceClassItems->at(i));
		set<int> * diffset = new set<int>();
		for(int i=0;i<N;i++) {
			if(tidset->find(i) == tidset->end()) {
				diffset->insert(i);
			}
		}
		equivalenceClassDiffsets->push_back(diffset); 
	} 
	
	delete this->database;

	int * prefixArray = new int[1000];

	this->processEquivalenceClassDEclat(new set<int>(), prefixArray, 0, N, equivalenceClassItems, equivalenceClassDiffsets);

	delete prefixArray;
}


void algoFramework::processEquivalenceClassEclat(set<int> * prefixTidset, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<set<int> * > * equivalenceClassTidsets) {

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
		set<int> * tidsetI = equivalenceClassTidsets->at(0);
		int supportI = tidsetI->size();
		this->save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		set<int> * tidsetJ = equivalenceClassTidsets->at(1);
		int supportJ = tidsetJ->size();
		this->save(prefix, prefixLength, itemJ, supportJ);
		
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

				double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
				double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
				int ECLATthreshold = (int)(N*(1.0/INTSIZE) );
				int ECLATstart = 0;
				int DECLATthreshold  = supportI - ECLATthreshold;
				int DECLATstart  = supportI;

				if(DECLATthreshold <= ECLATthreshold) {

					double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
			
					if(EAvg > PointOfDiff){
						
						// cout << "DECLAT" << endl;
						
						vector<set<int> * > * equivalenceClassIDiffsets = convertTIDSETStoDIFFSETS(tidsetI, equivalenceClassITidsets);
						set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixTidset(tidsetI);

						for(int i=0;i<equivalenceClassITidsets->size();i++)
							delete equivalenceClassITidsets->at(i);
						delete equivalenceClassITidsets;

						this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);	
					}
					else {

						// cout << "ECLAT" << endl;
				
						set<int> * tidsetIClone = new set<int>(tidsetI->begin(), tidsetI->end());
						this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
				}
				else {
					if(EAvg <= ECLATthreshold){
						
						// cout << "DECLAT" << endl;
					
						set<int> * tidsetIClone = new set<int>(tidsetI->begin(), tidsetI->end());
						this->processEquivalenceClassEclat(tidsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
					else if(EAvg <= DECLATthreshold){
						
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
				}
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


set<int> * algoFramework::performINTERSECTION(set<int> * tidsetI, set<int> * tidsetJ) {
	
	set<int> * tidsetIJ = new set<int>();
	if(tidsetJ->size() < tidsetI->size()){
		for(set<int>::iterator i = tidsetJ->begin();i != tidsetJ->end();i++) 
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


vector<set<int> * > * algoFramework::convertTIDSETStoDIFFSETS(set<int> * prefixTidset, vector<set<int> * > * equivalenceClassITidsets) {
	
	vector<set<int> * > * equivalenceClassIDiffsets = new vector<set<int> * >();
	
	for(int i=0;i<equivalenceClassITidsets->size();i++) {
		set<int> * tidset = equivalenceClassITidsets->at(i);
		set<int> * diffset = new set<int>();
		for( set<int>::iterator j = prefixTidset->begin(); j != prefixTidset->end() ; j++ ) {
			int tid = *j;
			if( tidset->find(tid) == tidset->end())
				diffset->insert(tid);
		}
		equivalenceClassIDiffsets->push_back(diffset);
	}

	return equivalenceClassIDiffsets;
}


set<int> * algoFramework::formParentDiffsUnionFromPrefixTidset(set<int> * prefixTidset) {

	set<int> * parentDiffsUnion = new set<int>();
	
	for(int i=0;i<N;i++) {
		if( prefixTidset->find(i) == prefixTidset->end() )
			parentDiffsUnion->insert(i);
	}

	return parentDiffsUnion;
}


vector<boost::dynamic_bitset<> *> * algoFramework::convertTIDSETStoBITSETS(vector<set<int> * > * equivalenceClassITidsets) {
	
	vector<boost::dynamic_bitset<> *> * equivalenceClassIBitsets = new vector<boost::dynamic_bitset<> * >();

	for(int i=0;i<equivalenceClassITidsets->size();i++) {
		set<int> * tidset = equivalenceClassITidsets->at(i);
		boost::dynamic_bitset<> * bs = new boost::dynamic_bitset<>(N);
		for(set<int>::iterator i = tidset->begin(); i != tidset->end() ; i++ ) {
			int tid = *i;
			bs->set(tid);
		}
		equivalenceClassIBitsets->push_back(bs);
	}

	return equivalenceClassIBitsets;
}


boost::dynamic_bitset<> * algoFramework::formPrefixBitsetFromPrefixTidset(set<int> * prefixTidset) {
	
	boost::dynamic_bitset<> * prefixBitset = new boost::dynamic_bitset<>(N);

	for(set<int>::iterator i = prefixTidset->begin(); i != prefixTidset->end() ; i++ ) {
		int tid = *i;
		prefixBitset->set(tid);
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

				boost::dynamic_bitset<> * bitsetIClone = new boost::dynamic_bitset<>(*bitsetI);
				this->processEquivalenceClassViper(bitsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);

			}
			else {

				double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
				double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
				int ECLATthreshold = (int)N*(1.0/INTSIZE);
				int ECLATstart = 0;
				int DECLATthreshold  = supportI - ECLATthreshold;
				int DECLATstart  = supportI;

				if(DECLATthreshold <= ECLATthreshold) {
	
					double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;
	
					if(EAvg > PointOfDiff){
						
						// cout << "DECLAT" << endl;
					
						vector<set<int> * > * equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
						set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
					
						for(int i=0;i<equivalenceClassIBitsets->size();i++)
							delete equivalenceClassIBitsets->at(i);
						delete equivalenceClassIBitsets;

						this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);

					}
					else {
						
						// cout << "ECLAT" << endl;
					
						vector<set<int> * > * equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
						set<int> * prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
					
						for(int i=0;i<equivalenceClassIBitsets->size();i++)
							delete equivalenceClassIBitsets->at(i);
						delete equivalenceClassIBitsets;

						this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
				}
				else {
					if(EAvg <= ECLATthreshold){
						
						// cout << "ECLAT" << endl;
						
						vector<set<int> * > * equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
						set<int> * prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);
					
						for(int i=0;i<equivalenceClassIBitsets->size();i++)
							delete equivalenceClassIBitsets->at(i);
						delete equivalenceClassIBitsets;

						this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
					else if(EAvg <= DECLATthreshold){
						
						// cout << "VIPER" << endl;
	
						boost::dynamic_bitset<> * bitsetIClone = new boost::dynamic_bitset<>(*bitsetI);
						this->processEquivalenceClassViper(bitsetIClone, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
					}
					else{
						
						// cout << "DECLAT" << endl;
						
						vector<set<int> * > * equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
						set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);
					
						for(int i=0;i<equivalenceClassIBitsets->size();i++)
							delete equivalenceClassIBitsets->at(i);
						delete equivalenceClassIBitsets;

						this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
						
					}
				}
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


vector<set<int> * > * algoFramework::convertBITSETStoTIDSETS(vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets) {
	
	vector<set<int> * > * equivalenceClassITidsets = new vector<set<int> * >();

	for(int i=0;i<equivalenceClassIBitsets->size();i++) {
		boost::dynamic_bitset<> * bitst = equivalenceClassIBitsets->at(i); 
		set<int> * tidset = new set<int>();

		for(int i = bitst->find_first(); i >= 0; i = bitst->find_next(i)) {
			tidset->insert(i);
		}
		
		equivalenceClassITidsets->push_back(tidset);
	}
	return equivalenceClassITidsets;
}


set<int> * algoFramework::formPrefixTidsetFromPrefixBitsets(boost::dynamic_bitset<> * prefixBitset) {
	
	set<int> * prefixTidset = new set<int>();

	for(int i = prefixBitset->find_first(); i >= 0; i = prefixBitset->find_next(i)) {
		prefixTidset->insert(i);
	}

	return prefixTidset;
}


vector<set<int> * > * algoFramework::convertBITSETStoDIFFSETS(boost::dynamic_bitset<> * prefixBitset, vector<boost::dynamic_bitset<> *> * equivalenceClassIBitsets) {
	
	vector<set<int> * > * equivalenceClassIDiffsets = new vector<set<int> * >();
	
	for(int i=0; i < equivalenceClassIBitsets->size(); i++) {
		boost::dynamic_bitset<> * bitset = equivalenceClassIBitsets->at(i);
		set<int> * diffset = new set<int>();
		for(int i = prefixBitset->find_first(); i >= 0; i = prefixBitset->find_next(i))
			if(!bitset->test(i))
				diffset->insert(i);
		equivalenceClassIDiffsets->push_back(diffset);
	}

	return equivalenceClassIDiffsets;
}


set<int> * algoFramework::formParentDiffsUnionFromPrefixBitset(boost::dynamic_bitset<> * prefixBitset) {

	set<int> * parentDiffsUnion = new set<int>();
	for(int i=0;i>N;i++) {
		if(!prefixBitset->test(i))
			parentDiffsUnion->insert(i);
	}

	return parentDiffsUnion;
}


void algoFramework::processEquivalenceClassDEclat(set<int> * parentDiffsUnion, int * prefix, int prefixLength, int prefixSupport, vector<int> * equivalenceClassItems, vector<set<int> * > * equivalenceClassDiffsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> DECLAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

	cout << "Prefix : ";
	for(int i=0;i<prefixLength;i++)
		cout << prefix[i] << ", ";
	cout << endl;
	
	rec[3]++;
	
	int length = prefixLength+1;

	
	if(equivalenceClassItems->size() == 1) {
		
		int item = equivalenceClassItems->at(0);
		set<int> * diffset = equivalenceClassDiffsets->at(0);
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
		set<int> * diffsetI = equivalenceClassDiffsets->at(0);
		int supportI = prefixSupport - diffsetI->size();
		save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		set<int> * diffsetJ = equivalenceClassDiffsets->at(1);
		int supportJ = prefixSupport - diffsetJ->size();
		save(prefix, prefixLength, itemJ, supportJ);
		
		set<int> * diffsetIJ = this->performDIFFERENCE(diffsetI, diffsetJ);
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
		set<int> * diffsetI = equivalenceClassDiffsets->at(i);
		int supportI = prefixSupport - diffsetI->size();
		save(prefix, prefixLength, suffixI, supportI);
		
		vector<int> * equivalenceClassISuffixItems = new vector<int>();
		vector<set<int> * > * equivalenceClassIDiffsets = new vector<set<int> * >();

		set<int> * newParentDiffsUnion = new set<int>(parentDiffsUnion->begin(), parentDiffsUnion->end());
		newParentDiffsUnion->insert(diffsetI->begin(), diffsetI->end());

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {
			
			int suffixJ = equivalenceClassItems->at(j);
			set<int> * diffsetJ = equivalenceClassDiffsets->at(j);
			int supportJ = prefixSupport - diffsetJ->size();
			
			set<int> * diffsetIJ = this->performDIFFERENCE(diffsetI, diffsetJ);
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

				this->processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);

			}
			else {

				double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
				double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
				int ECLATthreshold = (int)N*(1.0/INTSIZE) ;
				int ECLATstart = 0;
				int DECLATthreshold  = supportI - ECLATthreshold;
				int DECLATstart  = supportI;

				if(DECLATthreshold <= ECLATthreshold) {

					double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;

					if(EAvg > PointOfDiff){
						
						// cout << "DECLAT" << endl;
						
						this->processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
					else {
						
						// cout << "ECLAT" << endl;

						vector<set<int> * > * equivalenceClassITidsets = convertDIFFSETStoTIDSETS(newParentDiffsUnion, equivalenceClassIDiffsets);
						set<int> * prefixTidset = formPrefixTidsetFromParentDiffsUnion(newParentDiffsUnion);
						
						for(int i=0;i<equivalenceClassIDiffsets->size();i++)
							delete equivalenceClassIDiffsets->at(i);
						delete equivalenceClassIDiffsets;
						delete newParentDiffsUnion;
						
						this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
				}
				else {
					if(EAvg <= ECLATthreshold){
						
						// cout << "ECLAT" << endl;
						
						vector<set<int> * > * equivalenceClassITidsets = convertDIFFSETStoTIDSETS(newParentDiffsUnion, equivalenceClassIDiffsets);
						set<int> * prefixTidset = formPrefixTidsetFromParentDiffsUnion(newParentDiffsUnion);

						for(int i=0;i<equivalenceClassIDiffsets->size();i++)
							delete equivalenceClassIDiffsets->at(i);
						delete equivalenceClassIDiffsets;
						delete newParentDiffsUnion;
						
						this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
					}
					else if(EAvg <= DECLATthreshold){
						
						// cout << "VIPER" << endl;
						
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
						
						this->processEquivalenceClassDEclat(newParentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);
					}
				}
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
set<int> * algoFramework::performDIFFERENCE(set<int> * diffsetI, set<int> * diffsetJ) {
	
	set<int> * diffsetIJ = new set<int>();
	for(set<int>::iterator i = diffsetJ->begin(); i != diffsetJ->end(); i++){ 
		int tid = *i;
		if( diffsetI->find(tid) == diffsetI->end() )
			diffsetIJ->insert(tid);
	}

	return diffsetIJ;
}


vector<set<int> * > * algoFramework::convertDIFFSETStoTIDSETS(set<int> * parentDiffsUnion, vector<set<int> * > * equivalenceClassIDiffsets) {

	vector<set<int> * > * equivalenceClassITidsets = new vector<set<int> * >();

	for( int i=0;i<equivalenceClassIDiffsets->size();i++ ) {
		set<int> * diffset = equivalenceClassIDiffsets->at(i);
		set<int> * tidset = new set<int>();
		for(int j=0; j<N; j++) {
			if(parentDiffsUnion->find(i) == parentDiffsUnion->end() && diffset->find(i) == diffset->end())
				tidset->insert(i);
		}
		equivalenceClassITidsets->push_back(tidset);
	}

	return equivalenceClassITidsets;
}


set<int> * algoFramework::formPrefixTidsetFromParentDiffsUnion(set<int> * parentDiffsUnion) {

	set<int> * prefixTidset = new set<int>();
	for(int i=0;i<N;i++) {
		if( parentDiffsUnion->find(i) == parentDiffsUnion->end() )
			prefixTidset->insert(i);
	}

	return prefixTidset;
}


vector<boost::dynamic_bitset<> * > * algoFramework::convertDIFFSETStoBITSETS(set<int> * parentDiffsUnion, vector<set<int> * > * equivalenceClassIDiffsets) {

	vector<boost::dynamic_bitset<> * > * equivalenceClassIBitsets = new vector<boost::dynamic_bitset<> * >();

	for(int i=0;i<equivalenceClassIDiffsets->size();i++) {
		set<int> * diffset = equivalenceClassIDiffsets->at(i);
		boost::dynamic_bitset<> * bs = new boost::dynamic_bitset<>(N);
		for(int i=0;i>N;i++) {
			if(parentDiffsUnion->find(i) == parentDiffsUnion->end() && diffset->find(i) == diffset->end()) {
				bs->set(i);
			}
		}
		equivalenceClassIBitsets->push_back(bs);
	}

	return equivalenceClassIBitsets;
}


boost::dynamic_bitset<> * algoFramework::formPrefixBitsetFromParentDiffsUnion(set<int> * parentDiffsUnion) {

	boost::dynamic_bitset<> * prefixBitset = new boost::dynamic_bitset<>(N);
	for(int i=0;i>N;i++) {
		if(parentDiffsUnion->find(i) == parentDiffsUnion->end()) {
			prefixBitset->set(i);
		}
	}

	return prefixBitset;
}


void algoFramework::printStats() {
	cout << "\n\n===================================================" << endl;
	cout << " Transactions count from database : " << N << endl;
	cout << " Frequent itemset count : " << itemsetCount << endl;
	cout << " Usage : ECLAT "  << rec[1] << " VIPER " << rec[2] << " DECLAT " << rec[3] << endl;
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

