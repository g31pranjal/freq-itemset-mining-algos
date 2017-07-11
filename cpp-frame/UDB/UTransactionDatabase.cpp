#include "UTransactionDatabase.h"
#include "TidAndProb.h"
#include "UItem.h"

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

//this is the constructor for UTransactionDatabase
UTransactionDatabase::UTransactionDatabase() {
	items = new set<int>();
	horizontalDB = new vector<vector<UItem *> *>();
	verticalDB = new map<int, set<TidAndProb *> * >();
}

UTransactionDatabase:: ~ UTransactionDatabase() {
	this->dismantleHorizontalDatabase();
	this->dismantleItems();
	this->dismantleVerticalDatabase();
}

//the function to load the file
void UTransactionDatabase::loadFile(string path){
	string thisLine;
	ifstream infile(path.c_str());

	while(getline(infile, thisLine)){
		if(thisLine.length() != 0 && thisLine[0] != '#' && thisLine[0] != '%' && thisLine[0] != '@') {
			UTransactionDatabase::addTransaction(stringSplit(thisLine, " "));
		}
	}

	N = this->horizontalDB->size();

	// convert to vertical data structure
	vector<UItem *> * currTransaction;
	set<TidAndProb *> * st;

	for (int i = 0; i < N; i++){
		currTransaction = horizontalDB->at(i);
		for (int j = 0; j < currTransaction->size(); j++){
			
			int itemID = currTransaction->at(j)->getItemID();
			int transactionID = i;
			double probability = currTransaction->at(j)->getProbability();

			if (verticalDB->find(itemID) == verticalDB->end()){
				st = new set<TidAndProb *>();
				verticalDB->insert(make_pair(itemID, st));
			} else
				st = verticalDB->at(itemID);

			TidAndProb * newPair = new TidAndProb(transactionID, probability);
			st->insert(newPair);
		}
	}

	M = this->items->size();

	this->dismantleHorizontalDatabase();
	this->dismantleItems();
}


void UTransactionDatabase::addTransaction(vector<string> itemsString){
	
	vector<UItem *> * itemset = new vector<UItem *>();
	
	for (int i = 0; i < itemsString.size(); ++i){
		vector<string> arr = stringSplit(itemsString.at(i), "(");
		int itemID = stoi(arr.at(0));
		double probability = stod( arr.at(1).substr(0,arr.at(1).size() -1) );

		UItem * curr = new UItem(itemID, probability);
		itemset->push_back(curr);
		items->insert(itemID); 
	}
	horizontalDB->push_back(itemset);
}

void UTransactionDatabase::printHorizontalDatabase() {
	
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

void UTransactionDatabase::printVerticalDatabase() {
	set<TidAndProb *> * st;
	cout << "\n... Transaction Database :: (vertical)\n";

	for(map<int, set<TidAndProb *> *>::iterator i = verticalDB->begin(); i != verticalDB->end(); i++){
		cout << i->first << " : ";
		st = i->second;
		for (set<TidAndProb *>::iterator j = st->begin(); j != st->end() ; j++){
			cout << (*j)->getTid() << "(" << (*j)->getProbability() << ") ";
		}
		cout << endl;
	}
}

int UTransactionDatabase::getN(){
	return this->N;
}

int UTransactionDatabase::getM() {
	return this->M;
}

void UTransactionDatabase::dismantleHorizontalDatabase(){
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

void UTransactionDatabase::dismantleItems() {
	if (items != NULL){
		delete items;
		items = NULL;
	}
}

void UTransactionDatabase::dismantleVerticalDatabase() {
	if (verticalDB != NULL){
		for (map<int, set<TidAndProb *> *>::iterator i = verticalDB->begin(); i != verticalDB->end();i++){
			if (i->second != NULL){
				for (set<TidAndProb *>::iterator j = i->second->begin(); j != i->second->end(); j++){
					if (*j != NULL){
						delete *j;
					}
				}
				delete i->second;
			}
		}
		delete verticalDB;
		verticalDB = NULL;
	}
}

map<int, set<TidAndProb *> *> * UTransactionDatabase::getVerticalDatabase() {
	return this->verticalDB;
}


