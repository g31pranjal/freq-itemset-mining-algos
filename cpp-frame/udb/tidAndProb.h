#ifndef TIDANDPROB_H
#define TIDANDPROB_H

class tidAndProb{
	private:
		int tid;
		double probability;
	public:
		tidAndProb(int tid, double probability);
		~ tidAndProb();
		int getTid();
		double getProbability();
};

#endif