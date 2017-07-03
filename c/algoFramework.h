#ifndef ALGO

#define ALGO

#include "transactionDatabase.h"

class algoFramework {

	private :
		int minSupRelative;
		transactionDatabase * database;
		map<int, set<int> * > * verticalDB;
		// BufferedWriter writer = null; 
		// Itemsets frequentItemsets;
		int itemsetCount;
		int algo;
		int rec[4];
		int getN;


		// int INTSIZE = 32; //bits
		//long startTime;
		//long endTime;
		
	public :
		algoFramework();
		algoFramework(int algo);
		void runAlgo(string outputFile, transactionDatabase * database, double minsupp);
		
		// void constructTIDSETS(vector<int> * equivalenceClassItems);
		// void constructBITSETS(List<Integer> equivalenceClassItems);
		// void constructDIFFSETS(List<Integer> equivalenceClassItems);
		// void processEquivalenceClassEclat(Set<Integer> prefixTidset, int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<Set<Integer>> equivalenceClassTidsets);
		// Set<Integer> performINTERSECTION(Set<Integer> tidsetI, Set<Integer> tidsetJ);
		// List<Set<Integer>> convertTIDSETStoDIFFSETS(Set<Integer> prefixTidset, List<Set<Integer>> equivalenceClassITidsets);
		// Set<Integer> formParentDiffsUnionFromPrefixTidset(Set<Integer> prefixTidset);
		// List<BitSetSupport> convertTIDSETStoBITSETS(List<Set<Integer>> equivalenceClassITidsets);
		// BitSetSupport formPrefixBitsetFromPrefixTidset(Set<Integer> prefixTidset);
		// void processEquivalenceClassViper(BitSetSupport prefixBitset, int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<BitSetSupport> equivalenceClassBitsets);
		// BitSetSupport performAND(BitSetSupport bitsetI, BitSetSupport bitsetJ)
		// List<Set<Integer>> convertBITSETStoTIDSETS(List<BitSetSupport> equivalenceClassIBitsets);
		// Set<Integer> formPrefixTidsetFromPrefixBitsets(BitSetSupport prefixBitset);
		// List<Set<Integer>> convertBITSETStoDIFFSETS(BitSetSupport prefixBitset, List<BitSetSupport> equivalenceClassIBitsets);
		// Set<Integer> formParentDiffsUnionFromPrefixBitset(BitSetSupport prefixBitset);
		// void processEquivalenceClassDEclat(Set<Integer> parentDiffsUnion, int[] prefix, int prefixLength, int prefixSupport, List<Integer> equivalenceClassItems, List<Set<Integer>> equivalenceClassDiffsets)
		// Set<Integer> performDIFFERENCE(Set<Integer> diffsetI, Set<Integer> diffsetJ);
		// List<Set<Integer>> convertDIFFSETStoTIDSETS(Set<Integer> parentDiffsUnion, List<Set<Integer>> equivalenceClassIDiffsets);
		// Set<Integer> formPrefixTidsetFromParentDiffsUnion(Set<Integer> parentDiffsUnion);
		// List<BitSetSupport> convertDIFFSETStoBITSETS(Set<Integer> parentDiffsUnion, List<Set<Integer>> equivalenceClassIDiffsets);
		// BitSetSupport formPrefixBitsetFromParentDiffsUnion(Set<Integer> parentDiffsUnion);
		// void printStats();
		// void save(int[] prefix, int prefixLength, int suffixItem, int support);
};


#endif