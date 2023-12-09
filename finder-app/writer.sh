#!/bin/sh
if [ $# -ne 2 ]
  then
    echo "Incorrect number of parameters specified. 2 were expected (writefile writestr)"
    exit 1
fi
file=$1
mkdir -p "${file%/*}"
touch $file
echo "$2" > "$1"
if [ $? -eq 0 ]
  then
    exit 0
  else
    echo "Failed"
    exit 1
fi