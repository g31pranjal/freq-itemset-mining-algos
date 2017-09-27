all:
	g++ -g -std=c++11 -c uTransactionDatabase.cpp uAlgoFramework.cpp uItem.cpp ullSet.cpp ullSet_element.cpp uBitset.cpp -w
	g++ -g -std=c++11 testDB.cpp uAlgoFramework.o uTransactionDatabase.o uItem.o ullSet.o ullSet_element.o uBitset.o -w -o switch


clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf switch
	rm -rf out.put



run:
	bash script.sh
