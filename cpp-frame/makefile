
all:
	g++ -g -std=c++11 -c transactionDatabase.cpp algoFramework.cpp -w
	g++ -g -std=c++11 alterDB.cpp transactionDatabase.o algoFramework.o -w -o switch

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf switch
	rm -rf out*

run:
	# ./switch 0 0.9
	# ./switch 0 0.85
	# ./switch 0 0.8
	# ./switch 0 0.75
	# ./switch 0 0.7
	# ./switch 0 0.65
	# ./switch 0 0.6
	# ./switch 0 0.55
	# ./switch 0 0.5
	# ./switch 0 0.45
	# ./switch 0 0.4
	# ./switch 0 0.35
	# ./switch 0 0.3
	# ./switch 0 0.25
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.02 ./switch 0 0.2 > /massif/switch/output02.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.019 ./switch 0 0.19 > /massif/switch/output019.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.018 ./switch 0 0.18 > /massif/switch/output018.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.017 ./switch 0 0.17 > /massif/switch/output017.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.016 ./switch 0 0.16 > /massif/switch/output016.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.015 ./switch 0 0.15 > /massif/switch/output015.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.014 ./switch 0 0.14 > /massif/switch/output014.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.013 ./switch 0 0.13 > /massif/switch/output013.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.012 ./switch 0 0.12 > /massif/switch/output012.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.011 ./switch 0 0.11 > /massif/switch/output011.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.010 ./switch 0 0.1 > /massif/switch/output010.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.009 ./switch 0 0.09 > /massif/switch/output009.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.008 ./switch 0 0.08 > /massif/switch/output008.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.007 ./switch 0 0.07 > /massif/switch/output007.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.006 ./switch 0 0.06 > /massif/switch/output006.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.005 ./switch 0 0.05 > /massif/switch/output005.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.004 ./switch 0 0.04 > /massif/switch/output004.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.003 ./switch 0 0.03 > /massif/switch/output003.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.002 ./switch 0 0.02 > /massif/switch/output002.txt
	valgrind --tool=massif --massif-out-file=./massif/switch/massif.out.001 ./switch 0 0.01 > /massif/switch/output001.txt

