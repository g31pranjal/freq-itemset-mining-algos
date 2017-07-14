
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
	valgrind --tool=massif --massif-out-file=./massif/massif.out.02 ./switch 0 0.2 &>test.txt
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.019 ./switch 0 0.19
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.018 ./switch 0 0.18
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.017 ./switch 0 0.17
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.016 ./switch 0 0.16
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.015 ./switch 0 0.15
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.014 ./switch 0 0.14
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.013 ./switch 0 0.13
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.012 ./switch 0 0.12
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.011 ./switch 0 0.11
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.010 ./switch 0 0.1
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.009 ./switch 0 0.09
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.008 ./switch 0 0.08
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.007 ./switch 0 0.07
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.006 ./switch 0 0.06 > output006.txt
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.005 ./switch 0 0.05 &
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.004 ./switch 0 0.04 &
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.003 ./switch 0 0.03 &
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.002 ./switch 0 0.02 &
	#valgrind --tool=massif --massif-out-file=./massif/massif.out.001 ./switch 0 0.01 &

