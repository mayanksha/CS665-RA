for VARIABLE in `seq 5000`
		do
			OUTPUT=`taskset 0x1 ./a.out 100 0`
			arr=($OUTPUT)
			if [[ ${arr[0]} -lt 36 ]]
			then
		    	
		    	break
			fi
done