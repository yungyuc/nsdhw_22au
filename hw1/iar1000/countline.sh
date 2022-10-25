if [ "$#" -lt 1 ]; then
    echo "missing file name\n"
elif [ "$#" -gt 1 ]; then
    echo "only one argument is allowed\n"
else
    fname=$1
    if [ -f "$fname" ]; then
        lines=$(wc -l < "$fname")
	echo "$lines lines in $fname\n"
    else
        echo "$fname not found\n" 
    fi
fi
