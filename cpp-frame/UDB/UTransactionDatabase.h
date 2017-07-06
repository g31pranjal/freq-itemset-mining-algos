#ifndef UTRANS_DB

#define UTRANS_DB

#include "UItem.h"
#include "TidAndProb.h"

#include <set>
#include <vector>
#include <map>
#include <string>

using namespace std;

class UTransactionDatabase{
	private :
		set<int> * items;
		vector<vector<UItem*> *> * horizontalDB;
		map<int, set<TidAndProb *> *> *verticalDB;
		int N,M;

	public :
		UTransactionDatabase();
		~ UTransactionDatabase();
		void loadFile(string path);
		void addTransaction(vector<string> itemsString);
		void printHorizontalDatabase();
		void printVerticalDatabase();
		int getN();
		int getM();
		void dismantleHorizontalDatabase();
		void dismantleItems();
		void dismantleVerticalDatabase();


};

#endif