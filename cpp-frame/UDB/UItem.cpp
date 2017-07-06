#include "UItem.h"

using namespace std;

//the constructor for UItem
UItem::UItem(int itemID, double probability){
	this->itemID = itemID;
	this->probability = probability;
}

//the destructor for UItem
UItem::~UItem(){
	//nothing since there is no heap vars
}

int UItem::getItemID(){
	return itemID;
}

double UItem::getProbability(){
	return probability;
}
