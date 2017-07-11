#ifndef UITEM_H

#define UITEM_H

using namespace std;

class UItem{
	private:
		int itemID;
		double probability;
	public:
		UItem(int itemID, double probability);
		~ UItem();
		int getItemID();
		double getProbability();
};
#endif