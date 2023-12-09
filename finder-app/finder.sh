#!/bin/sh
if [ $# -ne 2 ]
  then
    echo "Incorrect number of parameters specified. 2 were expected (filesdir searchstr)"
    exit 1
fi
filesdir=$1
searchstr=$2
if [ -d "$filesdir" ]
  then
    numMatches=$(grep -o "$searchstr" "$filesdir"/*| wc -l)
    numFiles=$(ls "$filesdir" | wc -l)
    echo "The number of files are $numFiles and the number of matching lines are $numMatches"
  else
    echo "Directory ${filesdir} does not exist."
    exit 1
fi