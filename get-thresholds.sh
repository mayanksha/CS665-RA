memory=(1 2 4 )

gcc -g -O0 util.c page-size-test.c
if [ $? -ne 0 ]
then
    echo "Compile failed"
    exit -1
fi

export LD_PRELOAD=libhugetlbfs.so
export HUGETLB_MORECORE=yes
export HUGETLB_VERBOSE=3

for i in "${memory[@]}"
do
    data_file="data/temp-${i}"
    ./a.out > "${data_file}"
    python ./plt-hist.py "${data_file}"
done
