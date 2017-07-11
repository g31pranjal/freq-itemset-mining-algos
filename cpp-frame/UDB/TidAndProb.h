#ifndef TIDANDPROB_H
#define TIDANDPROB_H

class TidAndProb{
	private:
		int tid;
		double probability;
	public:
		TidAndProb(int tid, double probability);
		~ TidAndProb();
		int getTid();
		double getProbability();
};

#endif