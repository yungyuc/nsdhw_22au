# ! usr/bin/bash

if (($# < 1));then
  echo -e "missing file name\n"
  echo $0 $1
  echo $#
elif (($# > 1));then
  echo -e "only one argument is allowed\n"
else
  if [ -f $1 ]
  then
    num= wc -l < $1
    echo $num
  else
    echo -e"$1 not found\n"
  fi  
fi
