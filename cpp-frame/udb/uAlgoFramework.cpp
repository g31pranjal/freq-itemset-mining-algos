#include <cmath>
#include <algorithm>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
	
#include "uAlgoFramework.h"
#include "uTransactionDatabase.h"
#include "ullSet.h"
#include "uBitset.h"

using namespace std;

static double ULLSET /*bytes*/ = 24; 
static double BITSIZE /*bytes*/ = 1.0/8.0;

uAlgoFramework::uAlgoFramework(){
	this->algo = 0;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;
}

uAlgoFramework::uAlgoFramework(int algo){
	this->algo = algo;
	rec[0] = rec[1] = rec[2] = rec[3] = 0;	
}

void uAlgoFramework::runAlgo(char * outputFile, uTransactionDatabase * database, double minsupp, int precision){

	this->precision = precision;
	this->database = database;
	this->N = database->getN();
	this->M = database->getM();
	this->minSupRelative = minsupp * (double)N;
	this->verticalDB = database->getVerticalDatabase();

	this->writer.open(outputFile, _Ios_Openmode::_S_app);
	itemsetCount = 0;
	double totalInOneFrequent = 0l;

	cout << "\t\tminsupp :: " << this->minSupRelative << endl;
	
	vector<int> * oneFrequentItems = new vector<int>();

	for( map<int, ullSet *>::iterator i = verticalDB->begin(); i != verticalDB->end(); i++){
		ullSet *tidset = i->second;
		double support = tidset->support();
		int item = i->first;
		if (support >= minSupRelative){
			oneFrequentItems->push_back(item);
			totalInOneFrequent += tidset->size();
		}
	}

	if (oneFrequentItems->size() > 0) {
		
		// double avgTidsetSize =  (totalInOneFrequent / oneFrequentItems->size());
		
		sort(oneFrequentItems->begin(), oneFrequentItems->end(), [this] (int arg0, int arg1) {
			return this->verticalDB->at(arg0)->support() < this->verticalDB->at(arg1)->support() ;
		});

		int Estore = ULLSET*(totalInOneFrequent);
		int Vstore = (double)N*(oneFrequentItems->size())*BITSIZE*(precision+1);

		// cout << Estore << ", " << Vstore << ", " << Dstore << endl;
		// cout << oneFrequentItems->size() << ", " << N << endl;

		constructExistentialProb(oneFrequentItems);

		if(algo == 0) {
			if(Estore <= Vstore) {
				// cout << "ECLAT" << endl;
				this->constructTIDSETS(oneFrequentItems);
			}
			else {
				// cout << "VIPER" << endl;
				this->constructBITSETS(oneFrequentItems);
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
		}
	}

	this->printStats();

	this->dismantleExistentialDBUllSet();
	this->dismantleExistentialDBUBitset();

	writer.close();

}

void uAlgoFramework::constructExistentialProb(std::vector<int> * equivalenceClassItems) {

	this->existentialDBUllSet = new map<int, ullSet * >();
	this->existentialDBUBitset = new map<int, uBitset *>();

	for(int i=0;i<equivalenceClassItems->size();i++) {
		this->existentialDBUllSet->insert(pair<int, ullSet *>(equivalenceClassItems->at(i), verticalDB->at(equivalenceClassItems->at(i))->getDeepCopy()));

		vector<ullSet *> * oneExistentialDBUllSetVectorTmp = new vector<ullSet *> ();
		oneExistentialDBUllSetVectorTmp->push_back((verticalDB->at(equivalenceClassItems->at(i))));
		vector<uBitset *> * oneExistentialDBUBitsetVectorTmp =  convertTIDSETStoBITSETS(oneExistentialDBUllSetVectorTmp);
		this->existentialDBUBitset->insert(pair<int, uBitset *>(equivalenceClassItems->at(i), oneExistentialDBUBitsetVectorTmp->at(0)));

		oneExistentialDBUBitsetVectorTmp->pop_back();
		delete oneExistentialDBUBitsetVectorTmp;

		oneExistentialDBUllSetVectorTmp->pop_back();
		delete oneExistentialDBUllSetVectorTmp;
	}
}

void uAlgoFramework::constructTIDSETS(vector<int> * equivalenceClassItems) {
	
	vector<ullSet * > * equivalenceClassTidsets = new vector<ullSet * >();

	// populate the vector of set ints corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {

		equivalenceClassTidsets->push_back((verticalDB->at(equivalenceClassItems->at(i))));
		verticalDB->at(equivalenceClassItems->at(i)) = NULL;
	} 

	delete this->database;

	int * prefixArray = new int[1000];
	
	this->processEquivalenceClassEclat(prefixArray, 0, (double)N, equivalenceClassItems, equivalenceClassTidsets);
	
	delete[] prefixArray;



}


void uAlgoFramework::constructBITSETS(vector<int> * equivalenceClassItems) {
	
	vector<uBitset * > * equivalenceClassBitsets = new vector<uBitset * >();

	// populate the list of set integers corresponding to the sorted frequent 1-itemsets. 		
	for(int i=0;i<equivalenceClassItems->size();i++) {

		int item = equivalenceClassItems->at(i);
		uBitset * bs = new uBitset(N, precision);
		ullSet * tidset = verticalDB->at(item);

		for(ullSet_element * ptr = tidset->getFirst(); ptr != NULL; ptr = ptr->getNext()) {
			bs->insert(ptr->getValue().first, ptr->getValue().second);
		}

		equivalenceClassBitsets->push_back(bs);
	}

	delete this->database;

	int * prefixArray = new int[1000];

	this->processEquivalenceClassViper(prefixArray, 0, (double)N, equivalenceClassItems, equivalenceClassBitsets);
	
	delete[] prefixArray;

}



void uAlgoFramework::processEquivalenceClassEclat(int * prefix, int prefixLength, double prefixSupport, vector<int> * equivalenceClassItems, vector<ullSet * > * equivalenceClassTidsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> ECLAT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;

//	cout << "Prefix : " ;
//	for(int i=0;i<prefixLength;i++)
//		cout << prefix[i] << ", " ;
//	cout << endl;

	rec[1]++;

	int length = prefixLength+1;
	
	if(equivalenceClassItems->size() == 1) {
		
		int item = equivalenceClassItems->at(0);
		ullSet * tidset = equivalenceClassTidsets->at(0);
		double support = tidset->support();
		this->save(prefix, prefixLength, item, support);

		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassTidsets->size();d++) 
			delete equivalenceClassTidsets->at(d);
		delete equivalenceClassTidsets;

		return;
	}

	if(equivalenceClassItems->size() == 2) {
		
		int itemI = equivalenceClassItems->at(0);
		ullSet * tidsetI = equivalenceClassTidsets->at(0);
		double supportI = tidsetI->support();
		this->save(prefix, prefixLength, itemI, supportI);
		
		int itemJ = equivalenceClassItems->at(1);
		//ullSet * tidsetJ = equivalenceClassTidsets->at(1);

		ullSet * tidsetJ = this->existentialDBUllSet->at(itemJ);

		double supportJ = tidsetJ->support();
		this->save(prefix, prefixLength, itemJ, supportJ);

		ullSet * tidsetIJ = this->performINTERSECTION(tidsetI, tidsetJ);
		double supportIJ = tidsetIJ->support();
		
		if(supportIJ >= minSupRelative) {
			int newPrefixLength = prefixLength+1;
			prefix[prefixLength] = itemI;
			this->save(prefix, newPrefixLength, itemJ, supportIJ);
		}

		tidsetI = NULL;
		tidsetJ = NULL;
		delete tidsetIJ;
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
		ullSet * tidsetI = equivalenceClassTidsets->at(i);
		double supportI = tidsetI->support();
		this->save(prefix, prefixLength, suffixI, supportI);
		
		vector<int> * equivalenceClassISuffixItems = new vector<int>();
		vector<ullSet * > * equivalenceClassITidsets = new vector<ullSet * >();

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {

			int suffixJ = equivalenceClassItems->at(j);
			ullSet * tidsetJ = this->existentialDBUllSet->at(suffixJ);
			double supportJ = tidsetJ->support();
			
			ullSet * tidsetIJ = this->performINTERSECTION(tidsetI, tidsetJ);
			double supportIJ = tidsetIJ->support();
	
			if(supportIJ >= minSupRelative) {
				ETotal += tidsetIJ->size();
				DTotal += (tidsetI->size() - tidsetIJ->size());
				equivalenceClassISuffixItems->push_back(suffixJ);
				equivalenceClassITidsets->push_back(tidsetIJ);
				tidsetIJ == NULL;
			}
			else {
				delete tidsetIJ;
			}
		}
		
		if(equivalenceClassISuffixItems->size() > 0) {

//			cout << "equivalenceClassItems = " << equivalenceClassItems << endl;
//
//			for(int k=0; k < equivalenceClassItems->size(); k++) {
//				cout << equivalenceClassItems->at(k) << ", ";
//			}
//
//			cout << "equivalenceClassTidsets =  " << equivalenceClassTidsets << endl;
//
//			for(int k=0; k < equivalenceClassTidsets->size(); k++){
//				cout << equivalenceClassTidsets->at(k) << ", ";
//			}

			prefix[prefixLength] = suffixI;
			int newPrefixLength = prefixLength+1;
			
			if(algo == 1) {
				this->processEquivalenceClassEclat(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
			}
			else {

				// double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
				// double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();
				
				// int ECLATthreshold = (int)(N*(1.0/INTSIZE) );
				// int ECLATstart = 0;
				// int DECLATthreshold  = supportI - ECLATthreshold;
				// int DECLATstart  = supportI;

				int Estore = ULLSET*(ETotal);
				int Vstore = (double)N * (equivalenceClassISuffixItems->size()) * BITSIZE * (precision+1);

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

				cout << Estore << ", " << Vstore << endl;

				if(Estore <= Vstore){

					this->processEquivalenceClassEclat(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				
				}
				else {
					
//					// cout << "VIPER" << endl;
					vector<uBitset * > * equivalenceClassIBitsets = convertTIDSETStoBITSETS(equivalenceClassITidsets);

//                    cout << "1. test convertTIDSETStoBITSETS: prefixSupportSet before convert" << endl;
//                    ullSet* testUllSet = new ullSet();
//                    testUllSet->addElement(make_pair<int, double>(1,0.3));
//                    testUllSet->addElement(make_pair<int, double>(2,0.5));
//                    testUllSet->print();
//
//                    vector<ullSet * > * testUllSetVector = new vector<ullSet *>();
//                    testUllSetVector->push_back(testUllSet);
//                    vector<uBitset * > * testUllSetBitsetsVector = convertTIDSETStoBITSETS(testUllSetVector);
//                    uBitset * testUllSetBitsets = testUllSetBitsetsVector->at(0);
//
//                    cout << "2. test convertTIDSETStoBITSETS: prefixSupportSet after convert" << endl;
//                    for(int i=0; i < 4; i++){
//                        cout << "(" << i <<"," << testUllSetBitsets->getProbability(i) << "), ";
//                    }
//                    cout << endl;



//                    vector<ullSet * > * tidsetIVector = new vector<ullSet *>();
//                    tidsetIVector->push_back(tidsetI->getDeepCopy());
//                    vector<uBitset * > * tidsetIBitsetsVector = convertTIDSETStoBITSETS(tidsetIVector);
//
//                    uBitset * tidsetIBitsets = tidsetIBitsetsVector->at(0);
//                    uBitset * tidsetIBitsetsCopy = tidsetIBitsets->getDeepCopy();
//
//                    delete tidsetIVector->at(0);
//                    delete tidsetIVector;
//
//                    delete tidsetIBitsetsVector->at(0);
//                    delete tidsetIBitsetsVector;

//                    cout << "2. test convertTIDSETStoBITSETS: prefixSupportSet after convert" << endl;
//                    for(int i=0; i < prefixSupportBitsets->size(); i++){
//                        cout << "(" << i <<"," << prefixSupportBitsets->getProbability(i) << "), ";
//                    }
//                    cout << endl;

					for(int i=0;i<equivalenceClassITidsets->size();i++)
						delete equivalenceClassITidsets->at(i);
					delete equivalenceClassITidsets;

					this->processEquivalenceClassViper(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);
				}
				// }
			}			
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


ullSet * uAlgoFramework::performINTERSECTION(ullSet * tidsetI, ullSet * tidsetJ) {
	
	ullSet * tidsetIJ = new ullSet();
	ullSet_element * ptrA = tidsetI->getFirst();
	ullSet_element * ptrB = tidsetJ->getFirst();

	// if( JMap->size() < IMap->size() ){
	// 	for(unordered_map<int, double>::iterator i = JMap->begin();i != JMap->end();i++) 
	// 		if(IMap->find(i->first) != IMap->end())
	// 			tidsetIJ->insert( i->first ,(i->second)*(IMap->at(i->first)) );
	// }
	// else {
	// 	for(unordered_map<int, double>::iterator i = IMap->begin();i != IMap->end();i++) 
	// 		if(JMap->find(i->first) != JMap->end())
	// 			tidsetIJ->insert( i->first ,(i->second)*(JMap->at(i->first)));
	// }

	while(ptrA != NULL && ptrB != NULL) {
		if(ptrA->getValue().first < ptrB->getValue().first) {
			ptrA = ptrA->getNext();
		}
		else if(ptrA->getValue().first > ptrB->getValue().first) {
			ptrB = ptrB->getNext();
		}
		else {

			tidsetIJ->addElement(make_pair(ptrA->getValue().first, (ptrA->getValue().second*ptrB->getValue().second) / 1.0));

			ptrA = ptrA->getNext();
			ptrB = ptrB->getNext();
		}
	}

	return tidsetIJ;
}


vector<uBitset *> * uAlgoFramework::convertTIDSETStoBITSETS(vector<ullSet * > * equivalenceClassITidsets) {
	
	vector<uBitset *> * equivalenceClassIBitsets = new vector<uBitset * >();

	for(int i=0;i<equivalenceClassITidsets->size();i++) {

		ullSet * tidset = equivalenceClassITidsets->at(i);
		uBitset * bs = new uBitset(N, precision);
		
		for(ullSet_element * ptr = tidset->getFirst(); ptr != NULL; ptr = ptr->getNext()) {
			bs->insert(ptr->getValue().first, ptr->getValue().second);
		}
		equivalenceClassIBitsets->push_back(bs);
	}
	return equivalenceClassIBitsets;
}


void uAlgoFramework::processEquivalenceClassViper(int * prefix, int prefixLength, double prefixSupport, vector<int> * equivalenceClassItems, vector<uBitset * > * equivalenceClassBitsets) {

	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>> VIPER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
//
//	cout << "Prefix : " ;
//	for(int i=0;i<prefixLength;i++)
//		cout << prefix[i] << ", ";
//	cout << endl;

	rec[2]++;

	int length = prefixLength+1;

	if(equivalenceClassItems->size() == 1) {

		int item = equivalenceClassItems->at(0);
		uBitset * bitset = equivalenceClassBitsets->at(0);
		save(prefix, prefixLength, item, bitset->getSupport());

		delete equivalenceClassItems;
		for(int d=0;d<equivalenceClassBitsets->size();d++)
			delete equivalenceClassBitsets->at(d);
		delete equivalenceClassBitsets;

		return;
	}

	if(equivalenceClassItems->size() == 2) {

		int itemI = equivalenceClassItems->at(0);
		uBitset * bitsetI = equivalenceClassBitsets->at(0);
		double supportI = bitsetI->getSupport();
		save(prefix, prefixLength, itemI, supportI);

		int itemJ = equivalenceClassItems->at(1);
		uBitset * bitsetJ = this->existentialDBUBitset->at(itemJ);
		double supportJ = bitsetJ->getSupport();
		save(prefix, prefixLength, itemJ, supportJ);

		uBitset * bitsetIJ = this->performAND(bitsetI, bitsetJ);
		double supportIJ = bitsetIJ->getSupport();

		if(supportIJ >= minSupRelative) {
			int newPrefixLength = prefixLength+1;
			prefix[prefixLength] = itemI;
			save(prefix, newPrefixLength, itemJ, supportIJ);
		}

		bitsetI = NULL;
		bitsetJ = NULL;

		delete bitsetIJ;
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
		uBitset * bitsetI = equivalenceClassBitsets->at(i);
		double supportI = bitsetI->getSupport();
		this->save(prefix, prefixLength, suffixI, supportI);

		vector<int> * equivalenceClassISuffixItems= new vector<int>();
		vector<uBitset * > * equivalenceClassIBitsets = new vector<uBitset * >();

		for(int j=i+1; j < equivalenceClassItems->size(); j++) {

			int suffixJ = equivalenceClassItems->at(j);
			uBitset * bitsetJ = this->existentialDBUBitset->at(suffixJ);
			int supportJ = bitsetJ->getSupport();

			uBitset * bitsetIJ = this->performAND(bitsetI, bitsetJ);
			double supportIJ = bitsetIJ->getSupport();

			if(supportIJ >= minSupRelative) {
				ETotal += bitsetIJ->size();
				DTotal += (bitsetI->size() - bitsetIJ->size());
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
			else {

				// double DAvg = DTotal / (double)equivalenceClassISuffixItems->size();
				// double EAvg = ETotal / (double)equivalenceClassISuffixItems->size();

				// int ECLATthreshold = (int)N*(1.0/INTSIZE);
				// int ECLATstart = 0;
				// int DECLATthreshold  = supportI - ECLATthreshold;
				// int DECLATstart  = supportI;

				int Estore = ULLSET*(ETotal);
				int Vstore = (double)N*(equivalenceClassISuffixItems->size())*BITSIZE*(precision+1);

				// if(DECLATthreshold <= ECLATthreshold) {

				// 	double PointOfDiff = DECLATthreshold + (ECLATthreshold - DECLATthreshold)/2.0;

				// 	if(EAvg > PointOfDiff){

				// 		// cout << "DECLAT" << endl;

				// 		vector<set<int> * > * equivalenceClassIDiffsets = convertBITSETStoDIFFSETS(bitsetI, equivalenceClassIBitsets);
				// 		set<int> * parentDiffsUnion = formParentDiffsUnionFromPrefixBitset(bitsetI);

				// 		for(int i=0;i<equivalenceClassIBitsets->size();i++)
				// 			delete equivalenceClassIBitsets->at(i);
				// 		delete equivalenceClassIBitsets;

				// 		this->processEquivalenceClassDEclat(parentDiffsUnion, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIDiffsets);

				// 	}
				// 	else {

				// 		// cout << "ECLAT" << endl;

				// 		vector<set<int> * > * equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);
				// 		set<int> * prefixTidset = formPrefixTidsetFromPrefixBitsets(bitsetI);

				// 		for(int i=0;i<equivalenceClassIBitsets->size();i++)
				// 			delete equivalenceClassIBitsets->at(i);
				// 		delete equivalenceClassIBitsets;

				// 		this->processEquivalenceClassEclat(prefixTidset, prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);
				// 	}
				// }
				// else {

				cout << Estore << ", " << Vstore << endl;

				if(Estore <= Vstore){

					// cout << "ECLAT" << endl;

					vector<ullSet * > * equivalenceClassITidsets = convertBITSETStoTIDSETS(equivalenceClassIBitsets);


//                    uBitset* testBitset = new uBitset(4);
//                    testBitset->insert(2, 0.3);
//                    testBitset->insert(1, 0.5);
//
//                    vector<uBitset * > * testBitsetVector = new vector<uBitset *>();
//                    testBitsetVector->push_back(testBitset);
//                    vector<ullSet * > * testTidsetVector = convertBITSETStoTIDSETS(testBitsetVector);
//                    ullSet * testTidset = testTidsetVector->at(0);
//
//                    cout << "3. test convertBITSETStoTIDSETS: prefixTidsets before convert" << endl;
//                    for(int i=0; i < 4; i++){
//                        cout << "(" << i <<"," << testBitset->getProbability(i) << "), ";
//                    }
//                    cout << endl;
//
//                    cout << "4. test convertBITSETStoTIDSETS: prefixTidsets after convert" << endl;
//                    testTidset->print();



//                    vector<uBitset * > * bitSetIVector = new vector<uBitset *>();
//                    bitSetIVector->push_back(bitsetI->getDeepCopy());
//                    vector<ullSet * > * ullSetITidsetsVector = convertBITSETStoTIDSETS(bitSetIVector);
//
//                    ullSet * ullSetITidsets = ullSetITidsetsVector->at(0);
//                    ullSet * ullSetITidsetsCopy = ullSetITidsets->getDeepCopy();
//
//                    delete bitSetIVector->at(0);
//                    delete bitSetIVector;
//
//                    delete ullSetITidsetsVector->at(0);
//                    delete ullSetITidsetsVector;


					for(int i=0;i<equivalenceClassIBitsets->size();i++)
						delete equivalenceClassIBitsets->at(i);
					delete equivalenceClassIBitsets;


					this->processEquivalenceClassEclat(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassITidsets);

				}
				else {
					// cout << "VIPER" << endl;s
					this->processEquivalenceClassViper(prefix, newPrefixLength, supportI, equivalenceClassISuffixItems, equivalenceClassIBitsets);

				}
				// }
			}
		}
		else {
			delete equivalenceClassISuffixItems;
			delete equivalenceClassIBitsets;
		}
	}

	delete equivalenceClassItems;
	for(int d=0;d<equivalenceClassBitsets->size();d++)
		delete equivalenceClassBitsets->at(d);
	delete equivalenceClassBitsets;

	return;

}

uBitset * uAlgoFramework::performAND(uBitset * bitsetI, uBitset * bitsetJ) {

	
	uBitset * bitsetIJ = new uBitset(this->N, precision);
	
	boost::dynamic_bitset<> * newEligible = new boost::dynamic_bitset<>( (*bitsetI->getEligible())&(*bitsetJ->getEligible()));

	for(int i = newEligible->find_first(); i >= 0; i = newEligible->find_next(i)) {
		bitsetIJ->insert(i, bitsetI->getProbability(i)*bitsetJ->getProbability(i));
	}

	delete newEligible;

	return bitsetIJ;
}

vector<ullSet *> * uAlgoFramework::convertBITSETStoTIDSETS(vector<uBitset * > * equivalenceClassIBitsets) {

	vector<ullSet * > * equivalenceClassITidsets = new vector<ullSet * >();

	for(int i=0;i<equivalenceClassIBitsets->size();i++) {
		
		uBitset * bitst = equivalenceClassIBitsets->at(i); 
		boost::dynamic_bitset<> * eligible = bitst->getEligible();
		
		ullSet * tidset = new ullSet();

		for(int i = eligible->find_first(); i >= 0; i = eligible->find_next(i)) {
			tidset->addElement( make_pair(i, bitst->getProbability(i) ) );
		}
		equivalenceClassITidsets->push_back(tidset);
	}
	return equivalenceClassITidsets;
}


void uAlgoFramework::printStats() {

	string s = "";
	s = s + "===================================================\n" ;
	s = s + " Transactions count from database : " + to_string(N) + "\n" ;
	s = s +  " Frequent itemset count : " + to_string(itemsetCount) + "\n";
	s = s + " Usage : ECLAT "  + to_string(rec[1]) + " VIPER " + to_string(rec[2]) + " DECLAT " + to_string(rec[3]) + "\n";
	s = s + "===================================================" "\n";

	writer << s;

	cout << s <<endl;
}

void uAlgoFramework::save(int * prefix, int prefixLength, int suffixItem, double support) {
	
	itemsetCount++;
	string s = "";

	for(int i=0; i < prefixLength; i++) 
		s = s + (to_string(prefix[i]) + " ") ;
	s = s + (to_string(suffixItem) + " #SUP: " + to_string(support) +  '\n' );

}

void uAlgoFramework::dismantleExistentialDBUllSet() {
	if (existentialDBUllSet != NULL){
		for (map<int, ullSet *>::iterator it = existentialDBUllSet->begin(); it != existentialDBUllSet->end(); it ++){
			if (it->second != NULL){
				delete it->second; // because this variable points to the object in verticalDB, which has already been disposed
			}
		}
		delete existentialDBUllSet;
		existentialDBUllSet = NULL;
	}
}

void uAlgoFramework::dismantleExistentialDBUBitset() {
	if (existentialDBUBitset != NULL){
		for (map<int, uBitset *>::iterator it = existentialDBUBitset->begin(); it != existentialDBUBitset->end(); it ++){
			if (it->second != NULL)
				delete it->second;
		}
		delete existentialDBUBitset;
		existentialDBUBitset = NULL;
	}
}



