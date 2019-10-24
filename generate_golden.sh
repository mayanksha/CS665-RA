declare -a indices
counter=0
for iter in `seq 80`
do
	for VARIABLE in `seq 5000`
	do
		#echo $VARIABLE
		OUTPUT=`taskset 0x1 ./a.out 65`
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
for i in "${!indices[@]}"
do
	echo ${indices[$i]}
done
