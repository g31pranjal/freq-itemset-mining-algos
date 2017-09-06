a="0."
b="massif.out."
f="mushroom_10_100SPMF"
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

for number in 03 0275 025 0225 020 018 016 014 012 010 009 008 007 006 005 004 003
do
	c=$a$number
	outputfile=$b$number
    valgrind --tool=massif --max-snapshots=500 --massif-out-file=$dir2/$outputfile ./switch 1 $c > $dir2/$number.txt
    valgrind --tool=massif --max-snapshots=500 --massif-out-file=$dir1/$outputfile ./switch 0 $c > $dir1/$number.txt
    valgrind --tool=massif --max-snapshots=500 --massif-out-file=$dir3/$outputfile ./switch 2 $c > $dir3/$number.txt
done
