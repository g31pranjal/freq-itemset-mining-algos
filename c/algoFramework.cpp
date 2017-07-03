#include "algoFramework.h"
#include "transactionDatabase.h"

#include 

using namespace std;

algoFramework::algoFramework() {
	this->algo = 0;
}

algoFramework::algoFramework(int algo) {
	this->algo = algo;
}

void algoFramework::runAlgo(string outputFile, transactionDatabase * database, double minsupp) {

	this->database = database;
	this->getN = database->getN();
	this->minSupRelative = (int) Math.ceil(minsupp * getN);
	this->verticalDB = database->getVerticalDB();

	// writer = new BufferedWriter(new FileWriter(outputFile));
	
	itemsetCount = 0;
	Long totalInOneFrequent = 0l;
	
	vector<int> * oneFrequentItems = new vector<int>();
	


}