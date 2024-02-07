#!/bin/bash

# Data path
DATA_PATH="demo"
EXE_PATH="./CSV2root"


for dir in `ls -tr ${DATA_PATH}`
do
	dir="${DATA_PATH}/$dir"
	if [[ "$dir" = "${DATA_PATH}/RootData" ]]; then continue; fi
	if [[ "$dir" == ${DATA_PATH}/*.log ]]; then continue; fi
	if [[ "$dir" == ${DATA_PATH}/*.pssettings ]]; then continue; fi

	echo "--Removing originial CSV file: $dir"
	rm -rf $dir
done
