#include "tidAndProb.h"

using namespace std;

tidAndProb::tidAndProb(int tid, double probability){
	this->tid = tid;
	this->probability = probability;
}

tidAndProb::~tidAndProb(){

}

int tidAndProb::getTid(){
	return tid;
}

double tidAndProb::getProbability(){
	return probability;
}