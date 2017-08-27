#ifndef UTRANS_DB

#define UTRANS_DB

#include "uItem.h"
#include "ullSet.h"

#include <set>
#include <vector>
#include <map>
#include <string>

using namespace std;

class uTransactionDatabase{
	private :
		set<int> * items;
		vector<vector<uItem*> *> * horizontalDB;
		map<int, ullSet * > *verticalDB;
		int N,M;

	public :
		uTransactionDatabase();
		~ uTransactionDatabase();
		void loadFile(string path);
		void addTransaction(vector<string> itemsString);
		void printHorizontalDatabase();
		void printVerticalDatabase();
		int getN();
		int getM();
		map<int, ullSet *> * getVerticalDatabase();
		void dismantleHorizontalDatabase();
		void dismantleItems();
		void dismantleVerticalDatabase();

};

#endif
