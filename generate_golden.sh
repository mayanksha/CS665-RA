filename="golden_file.txt"
declare -a indices
counter=0
for set in `seq 0 1 1024`
do
	for iter in `seq 80`
	do
		echo $iter
		for VARIABLE in `seq 5000`
		do
			OUTPUT=`taskset 0x1 ./a.out 65 $set`
			arr=($OUTPUT)
			if [[ ${arr[0]} -eq 1 ]]
			then
		    	for i in "${!arr[@]}"
		    	do
		        	if [[ $i -ne 0 ]]
		        	then
		            	indices[$counter]=${arr[$i]}
		            	counter=$((counter+1))
		        	fi
		    	done
		    	break
			fi
		done
	done
	printf "" > $filename
	for i in "${!indices[@]}"
	do
		echo ${indices[$i]} >> $filename
	done
	python plot_l2_evict.py golden_file.txt $set
done