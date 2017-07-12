#ifndef UITEM_H

#define UITEM_H

using namespace std;

class uItem{
	private:
		int itemID;
		double probability;
	public:
		uItem(int itemID, double probability);
		~ uItem();
		int getItemID();
		double getProbability();
};
#endif