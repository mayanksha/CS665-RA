filename="golden_slice.txt"
echo "Getting Responses ...."
declare -a indices
counter=0
for set in `seq 1 1 1`
do
	for VARIABLE in `seq 500`
	do
		OUTPUT=`taskset 0x1 ./a.out 0`
		arr=($OUTPUT)
		for i in "${!arr[@]}"
	    do
	    	if [[ $i%2 -eq 0 ]]
	    	then
	    		if [[ ${arr[$i]} -eq 1 ]]
				then
			    	indices[$counter]=${arr[$i+1]}
			     	counter=$((counter+1))
				fi
	    	fi
    	done
		
	done
	printf "" > $filename
	for i in "${!indices[@]}"
	do
		echo ${indices[$i]} >> $filename
	done
	python plot_l2_evict.py golden_slice.txt $set
done