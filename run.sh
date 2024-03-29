#!/bin/bash

# Data path
DATA_PATH="demo"
EXE_PATH="./CSV2root"

# Remove blank and bracket in file names. e.g. '20231221 (2)'-> '20231221_2'
rename 's/ /_/g' $DATA_PATH/*
rename 's/\(//g' $DATA_PATH/*
rename 's/\)//g' $DATA_PATH/*


if [ ! -d "${DATA_PATH}/RootData" ];then
	mkdir ${DATA_PATH}/RootData
else
	echo "--RootData folder already exists. Removing files..."
	rm ${DATA_PATH}/RootData/*
fi

idx=1
evtNum=0
for dir in `ls -tr ${DATA_PATH}`
do
	dir="${DATA_PATH}/$dir"
	if [[ "$dir" = "${DATA_PATH}/RootData" ]]; then continue; fi
	if [[ "$dir" == "${DATA_PATH}/*.log" ]]; then continue; fi
	if [[ "$dir" == "${DATA_PATH}/*.pssettings" ]]; then continue; fi
	echo -e "\n--Now processing: $dir"
#	echo "Renameing csv files..."
#	rename 's/ \([0-9]{1,5}\)//g' ${dir}/*.csv

	# Get the file name length
	str=`ls "${dir}" | head -1`

	wfm_num=`ls "${dir}" | wc -l`
#	echo "$EXE_PATH ${DATA_PATH}/${dir} $wfm_num $idx"
  echo "Start conversion..."
	$EXE_PATH "${dir}" $wfm_num $idx "${str}" $evtNum
	let idx++
	let evtNum+=wfm_num

#	echo "--Removing originial CSV file: $dir"
#	rm -rf $dir
done
