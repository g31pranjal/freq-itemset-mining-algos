#include "uItem.h"

using namespace std;

//the constructor for uItem
uItem::uItem(int itemID, double probability){
	this->itemID = itemID;
	this->probability = probability;
}

//the destructor for uItem
uItem::~uItem(){
	//nothing since there is no heap vars
}

int uItem::getItemID(){
	return itemID;
}

double uItem::getProbability(){
	return probability;
}
