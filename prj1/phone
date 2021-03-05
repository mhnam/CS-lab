if [ $# -eq 0 ]
then
	echo "Usage: phone searchfor [...searchfor]"
	echo " (You didn't tell me what you want to search for.)"
else
	CNT=0
	TMP="("
	for arg in $@
	do
		TMP=$TMP"$arg"
		CNT=$((CNT+1))
		if [ $CNT -lt $# ]
		then
			TMP=$TMP"|"
		else
			TMP=$TMP")"
		fi
	done
	egrep "$TMP" mydata | awk -f display.awk
fi
