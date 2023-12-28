#!/bin/bash

DATA_PATH="demo"

cd $DATA_PATH/RootData

file_list=""

for file in `ls`
do
	file_list="$file_list $file"
done

OUT_NAME=${file_list:0:9}_merged.root

hadd $OUT_NAME $file_list

for file in $file_list
do
	rm $file
done
