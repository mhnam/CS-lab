if [ $# -eq 0 ]
then
	echo "Usage: phone searchfor [...searchfor]"
	echo " (You didn't tell me what you want to search for.)"
else
        if [ $# -eq 1 ]
        then
                egrep "$1" mydata | awk -f display.awk
        else
	        CNT=1
                for arg in $@
	        do
                        if [ $CNT -eq $# ]
                        then
                                egrep "$arg" temp | awk -f display.awk
                        else
                                egrep "$arg" mydata > temp
                        fi
                        CNT=$((CNT+1))
	        done
        fi
fi
