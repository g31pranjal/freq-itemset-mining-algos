a="0."
b="massif.out."
f="retail_10_100SPMF"
dir1=./massif/switch_$f
dir2=./massif/eclat_$f
dir3=./massif/viper_$f

#---------------------------
# mkdir if dir doesn't exist
#---------------------------

if [[ ! -e $dir1 ]]; then
    mkdir $dir1
elif [[ ! -d $dir1 ]]; then
    echo "$dir1 already exists but is not a directory" 1>&2
fi

if [[ ! -e $dir2 ]]; then
    mkdir $dir2
elif [[ ! -d $dir2 ]]; then
    echo "$dir2 already exists but is not a directory" 1>&2
fi

if [[ ! -e $dir3 ]]; then
    mkdir $dir3
elif [[ ! -d $dir3 ]]; then
    echo "$dir3 already exists but is not a directory" 1>&2
fi

#------------------
# do the massif job
#------------------

for number in  06 04 02
do
    for precision in 16
    do
	    c=$a$number
	    outputfile=$b$number
	    #valgrind --tool=massif --max-snapshots=1000 --alloc-fn='uAlgoFramework::constructExistentialProb(std::vector<int> *)' --alloc-fn='ullSet_element::getDeepCopy()' --massif-out-file=$dir1/$outputfile ./switch 0 $c $precision > $dir1/$number.txt
        valgrind --tool=massif --max-snapshots=1000 --alloc-fn='uAlgoFramework::constructExistentialProb(std::vector<int> *)' --alloc-fn='ullSet_element::getDeepCopy()' --massif-out-file=$dir2/$outputfile ./switch 1 $c $precision > $dir2/$number.txt
        #valgrind --tool=massif --max-snapshots=1000 --alloc-fn='uAlgoFramework::constructExistentialProb(std::vector<int> *)' --alloc-fn='ullSet_element::getDeepCopy()' --massif-out-file=$dir3/$outputfile ./switch 2 $c $precision > $dir3/$number.txt
        #./switch 0 $c $precision
        #./switch 1 $c $precision
        #./switch 2 $c $precision
    done
done

#for number in  05 04 03 02 01 009 008 007 006 005 0045 004 0035 003 0025 002
#do
#    for precision in 6 8 10 12 14 16 18 20 22 24
#    do
#	    c=$a$number
#	    outputfile=$b$number
#	    #valgrind --tool=massif --max-snapshots=1000 --alloc-fn='uAlgoFramework::constructExistentialProb(std::vector<int> *)' --alloc-fn='ullSet_element::getDeepCopy()' --massif-out-file=$dir1/$outputfile ./switch 0 $c $precision > $dir1/$number.txt
#        #valgrind --tool=massif --max-snapshots=1000 --alloc-fn='uAlgoFramework::constructExistentialProb(std::vector<int> *)' --alloc-fn='ullSet_element::getDeepCopy()' --massif-out-file=$dir2/$outputfile ./switch 1 $c $precision > $dir2/$number.txt
#        #valgrind --tool=massif --max-snapshots=1000 --alloc-fn='uAlgoFramework::constructExistentialProb(std::vector<int> *)' --alloc-fn='ullSet_element::getDeepCopy()' --massif-out-file=$dir3/$outputfile ./switch 2 $c $precision > $dir3/$number.txt
#        ./switch 0 $c $precision
#        ./switch 1 $c $precision
#        ./switch 2 $c $precision
#    done
#done
