all:
	# g++ -g -std=c++11 -c uTransactionDatabase.cpp uItem.cpp tidAndProb.cpp uAlgoFramework.cpp uTidset.cpp -w
	# g++ -g -std=c++11 testDB.cpp uTransactionDatabase.o uItem.o tidAndProb.o uAlgoFramework.o -w -o switch
	# g++ -g -std=c++11 testDB.cpp uTransactionDatabase.o uItem.o tidAndProb.o uAlgoFramework.o uTidset.o -w -o testUTidset
	g++ -g -std=c++11 -c uTransactionDatabase.cpp uAlgoFramework.cpp uItem.cpp uTidset.cpp -w
	g++ -g -std=c++11 testDB.cpp uAlgoFramework.o uTransactionDatabase.o uItem.o uTidset.o -w -o switch
clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf switch
