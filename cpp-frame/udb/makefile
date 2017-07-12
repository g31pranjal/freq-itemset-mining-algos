all:
	g++ -g -std=c++11 -c uTransactionDatabase.cpp uItem.cpp tidAndProb.cpp uAlgoFramework.cpp -w
	g++ -g -std=c++11 testDB.cpp uTransactionDatabase.o uItem.o tidAndProb.o uAlgoFramework.o -w -o switch

clean:
	rm -rf *.o
	rm -rf *.otu
	rm -rf switch