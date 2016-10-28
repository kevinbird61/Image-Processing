#!/bin/bash
INPUT=""
OUTPUT=""
EXECTIME=""
THREAD_NUM=""
# dealing with those arguments
# $1 : execution file , $2 : input file , $3 : output file , $4 : execution time , $5 : thread number
read -p "[Prepared to execute...]If you want to change input filename(Press Enter to get default)?[y/n] " ANS
if [[ $ANS == "y" || $ANS == "Y" ]]; then
  read -p "Write your path here: " PATH
  INPUT=$PATH
else
  INPUT="img/input.bmp"
fi

read -p "[Prepared to execute...]If you want to change output filename(Press Enter to get default)?[y/n] " ANS
if [[ $ANS == "y" || $ANS == "Y" ]]; then
  read -p "Write your path here: " PATH
  OUTPUT=$PATH
else
  OUTPUT="output.bmp"
fi

read -p "[Prepared to execute...]Enter the execution times your want to apply on image:[Press Enter to default=1] " ET
if [ -z $ET ]; then
  EXECTIME="1"
else
  EXECTIME=$ET
fi

read -p "[Prepared to execute...]Enter the thread numbers your want to apply on image:[Press Enter to default=2] " TN
if [ -z $TN ]; then
  THREAD_NUM="2"
else
  THREAD_NUM=$TN
fi

echo "Start to execute !"

./$1 $INPUT $OUTPUT $EXECTIME $THREAD_NUM
