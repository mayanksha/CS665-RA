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
            	echo ${arr[$i]}
        	fi
    	done
    	break
	fi
done