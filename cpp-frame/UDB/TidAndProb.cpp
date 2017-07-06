#include "TidAndProb.h"

using namespace std;

TidAndProb::TidAndProb(int tid, double probability){
	this->tid = tid;
	this->probability = probability;
}

TidAndProb::~TidAndProb(){

}

int TidAndProb::getTid(){
	return tid;
}

double TidAndProb::getProbability(){
	return probability;
}