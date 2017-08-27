#include "uTransactionDatabase.h"
#include "uItem.h"
#include "ullSet.h"

#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

//to split the string
vector<string> stringSplit(string str, string sep){
	char * cstr = const_cast<char *>(str.c_str());
	char * current;
	vector<string> arr;
	current = strtok(cstr, sep.c_str());
	while(current != NULL){
		arr.push_back(current);
		current = strtok(NULL,sep.c_str());
	}
	return arr;
}

//this is the constructor for uTransactionDatabase
uTransactionDatabase::uTransactionDatabase() {
	items = new set<int>();
	horizontalDB = new vector<vector<uItem *> *>();
	verticalDB = new map<int, ullSet * >();
}

uTransactionDatabase:: ~ uTransactionDatabase() {
	this->dismantleHorizontalDatabase();
	this->dismantleItems();
	this->dismantleVerticalDatabase();
}

//the function to load the file
void uTransactionDatabase::loadFile(string path){
	string thisLine;
	ifstream infile(path.c_str());

	while(getline(infile, thisLine)){
		if(thisLine.length() != 0 && thisLine[0] != '#' && thisLine[0] != '%' && thisLine[0] != '@') {
			uTransactionDatabase::addTransaction(stringSplit(thisLine, " "));
		}
	}

	N = this->horizontalDB->size();

	// convert to vertical data structure
	vector<uItem *> * currTransaction;
	//set<tidAndProb *> * st;
	ullSet * st;	

	for (int i = 0; i < N; i++){
		currTransaction = horizontalDB->at(i);
		for (int j = 0; j < currTransaction->size(); j++){
			int itemID = currTransaction->at(j)->getItemID();
			int transactionID = i;
			double probability = currTransaction->at(j)->getProbability();
			if (verticalDB->find(itemID) == verticalDB->end()){
				st = new ullSet();
				verticalDB->insert(pair<int, ullSet *>(itemID, st));
			} else
				st = verticalDB->at(itemID);

			st->addElement(make_pair(transactionID, probability));
		}
	}

	M = this->items->size();

	this->dismantleHorizontalDatabase();
	this->dismantleItems();
}


void uTransactionDatabase::addTransaction(vector<string> itemsString){
	
	vector<uItem *> * itemset = new vector<uItem *>();
	
	for (int i = 0; i < itemsString.size(); ++i){
		vector<string> arr = stringSplit(itemsString.at(i), "(");
		int itemID = stoi(arr.at(0));
		double probability = stod( arr.at(1).substr(0,arr.at(1).size() -1) );

		uItem * curr = new uItem(itemID, probability);
		itemset->push_back(curr);
		items->insert(itemID); 
	}
	horizontalDB->push_back(itemset);
}

void uTransactionDatabase::printHorizontalDatabase() {
	
	cout << "\n... Transaction Database :: (horizontal)\n";
	int count = 0;

	for (int i = 0; i < horizontalDB->size(); i++){
		cout << (count++) << " : ";
		for(int j=0;j<horizontalDB->at(i)->size();j++) {
			cout << horizontalDB->at(i)->at(j)->getItemID() << "(" << horizontalDB->at(i)->at(j)->getProbability() << ") ";	
		}
		cout << endl;
	}
}

void uTransactionDatabase::printVerticalDatabase() {
	ullSet * st;
	cout << "\n... Transaction Database :: (vertical)\n";

	for(map<int, ullSet *>::iterator i = verticalDB->begin(); i != verticalDB->end(); i++){
		cout << i->first << " : ";
		st = i->second;
		st->print();
	}
}

int uTransactionDatabase::getN(){
	return this->N;
}

int uTransactionDatabase::getM() {
	return this->M;
}

void uTransactionDatabase::dismantleHorizontalDatabase(){
	if (horizontalDB != NULL){
		for (int i = 0; i < horizontalDB->size(); i++){
			if (horizontalDB->at(i) != NULL){
				for (int j = 0; j < horizontalDB->at(i)->size(); j++){
					if (horizontalDB->at(i)->at(j) != NULL){
						delete horizontalDB->at(i)->at(j);
					}
				}
				delete horizontalDB->at(i);
			}
		}
		delete horizontalDB;
		horizontalDB = NULL;
	}
}

void uTransactionDatabase::dismantleItems() {
	if (items != NULL){
		delete items;
		items = NULL;
	}
}

void uTransactionDatabase::dismantleVerticalDatabase() {
	if (verticalDB != NULL){
		for (map<int, ullSet *>::iterator it = verticalDB->begin(); it != verticalDB->end(); it ++){
			if (it->second != NULL)
				delete it->second;
		}
		delete verticalDB;
		verticalDB = NULL;
	}
}

map<int, ullSet *> * uTransactionDatabase::getVerticalDatabase() {
	return this->verticalDB;
}

