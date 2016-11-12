#!/bin/bash -x
getopt -T
gversion=$?

# Color set
RED=`tput setaf 1`
GRE=`tput setaf 2`
RESET=`tput sgr0`

# Check version of getopt
if [ ${gversion} -eq "4" ]; then
  echo "${GRE}Your getopt version is OK!${RESET}"
else
  echo "${RED}Your getopt isn't over 4! Please upgrade your getopt!${RESET}"
  exit 1
fi

# Define helper function and usage
usage()
{
  echo "Usage: bash `basename $0` [-a -v -t -m -h -s | -g NUMBER]"
  echo "  -g NUMBER : define with the function we want to test, if don't set "
  echo "    just compile with all type"
  echo "  [--clean --help | --perf NUMBER]"
  echo "  --perf NUMBER : define the time we want to apply perf on this program"
  exit 1
}

format()
{
  scripts/install-git-hooks
  astyle --style=kr --indent=spaces=4 --indent-switches --suffix=none *.[ch]
}
# =========== defined Objects here ===========
OBJS=(gaussian mirror hsv)
TARGET=image_process
# =========== defined Objects here ===========
# Get string from command line
ARGS=`getopt -o aevtmhsg: --long help,clean,perf: -n 'execute.sh' -- "$@"`
if [ $? != 0 ]; then
    usage
fi

# Parsing ARGS
eval set -- "${ARGS}"
# defined filter
GAU_TYPE=1
ENVI=0
VALG=n
TEST=n
MIRROR=0
HSV=0
STRICT=n
PLOT=n
PERF=0

# Test argument from user stdin
while true
do
  case "$1" in
    -a)
      echo "compile with gau_all"
      GAU_TYPE=2047
      shift
      ;;
    -e)
      echo "compile with arm environment"
      ENVI=1
      ;;
    -v)
      echo "compile with valgrind"
      VALG=y
      shift
      ;;
    -t)
      echo "compile with test module"
      TEST=y
      shift
      ;;
    -m)
      echo "compile with mirror_all"
      MIRROR=1
      shift
      ;;
    -h)
      echo "compile with hsv"
      HSV=1
      shift
      ;;
    -s)
      echo "compile with strictly condition"
      STRICT=y
      shift
      ;;
    -g)
      echo "compile with target gaussian: $2"
      if [[ "$2" =~ ^[0-9]+$ ]]; then
        GAU_TYPE=$2
        shift 2
      else
        echo "${RED}You can't assign non-value to -g !${RESET}"
        usage
      fi
      ;;
    --perf)
      echo "compile + run and plot execution times: $2"
      PERF=$2
      # And must set gau_type to 2047
      GAU_TYPE=2047
      shift 2
      ;;
    --clean)
      echo "${RED}clean all object files${RESET}"
      # clean all the object file and useless files
      rm *output*.bmp *.png $TARGET *.log *.o 2> /dev/null
      exit 1
      ;;
    --help)
      usage
      ;;
    --)
      # use for end note
      shift
      break
      ;;
  esac
done

# Total we get
echo "The program will be compile with:"
#echo "  gaussian selection:$GAU_TYPE, mirror selection:${MIRROR}, hsv selection:${HSV},"
#echo "  valgrind usage:${VALG}, test module?${TEST}, strictly compile?${STRICT},"
#echo "  plot the figure?${PLOT}, plot with perf analysis?${PERF}"
CC="gcc"
CFLAGS=
if [ $STRICT = "y" ]; then
  CFLAGS="-msse -msse2 -msse3 -msse4 --std gnu99 -Wall -pedantic -fopenmp -O0"
else
  CFLAGS="-msse -msse2 -msse3 -msse4 --std gnu99 -fopenmp -O0"
fi

# generate those object file
format
# edit with object file
for item in ${OBJS[*]}
do
  $CC -c $CFLAGS -o ${item}.o ${item}.c
done

INPUT=
OUTPUT=
TIMES=
THREADS=
# Define value
read -p "[Prepared to execute...]Enter the execution times your want to apply on image:[Press Enter to default=1] " ET
if [ -z $ET ]; then
  TIMES="1"
else
  TIMES=$ET
fi
read -p "[Prepared to execute...]Enter the thread numbers your want to apply on image:[Press Enter to default=2] " TN
if [ -z $TN ]; then
  THREADS="2"
else
  THREADS=$TN
fi
read -p "[Prepared to execute...]If you want to change input filename(Press Enter to get default)?[y/n] " ANS
if [[ $ANS == "y" || $ANS == "Y" ]]; then
  read -p "Write your path here: " PATH
  INPUT=$PATH
else
  INPUT="img/input.bmp"
fi
read -p "[Prepared to execute...]If you want to change output filename(Press Enter to get default)?[y/n] " ANS
if [[ $ANS == "y" || $ANS == "Y" ]]; then
  read -p "Write your name here: " PATH
  OUTPUT=$PATH
else
  OUTPUT="output"
fi
# compile and run
if [ $PERF -eq "0" ]; then
  if [ $VALG = "n" ]; then
    $CC -std=gnu99 -c -DGAUSSIAN=$GAU_TYPE -DMIRROR=$MIRROR -DHSV=$HSV -o main.o main.c
    $CC $CFLAGS gaussian.o mirror.o hsv.o main.o -o $TARGET -lpthread
    ./$TARGET ${INPUT} ${OUTPUT}_${GAU_TYPE}_${MIRROR}_${HSV}.bmp ${TIMES} ${THREADS}
  else
    $CC -std=gnu99 -c -DGAUSSIAN=$GAU_TYPE -DMIRROR=$MIRROR -DHSV=$HSV -g -o main.o main.c
    $CC $CFLAGS gaussian.o mirror.o hsv.o main.o -o $TARGET -lpthread
    valgrind --leak-check=full ./$TARGET img/input.bmp output_${GAU_TYPE}_${MIRROR}_${HSV}.bmp 1 4
  fi
else
  if [ $VALG = "n" ]; then
    $CC -std=gnu99 -c -DPERF=1 -DGAUSSIAN=$GAU_TYPE -DMIRROR=$MIRROR -DHSV=$HSV -o main.o main.c
    $CC $CFLAGS gaussian.o mirror.o hsv.o main.o -o $TARGET -lpthread
    echo "Start to perf !"
    perf stat -r $PERF -e cache-misses,cache-references \
  	./$TARGET img/input.bmp output.bmp 1 4 > exec_time.log
    gnuplot scripts/plot_time.gp
  	gnuplot scripts/plot_time_2.gp
  else
    #$CC -std=gnu99 -c -DPERF=1 -DGAUSSIAN=$GAU_TYPE -DMIRROR=$MIRROR -DHSV=$HSV -g -o main.o main.c
    #$CC $CFLAGS gaussian.o mirror.o hsv.o main.o -o $TARGET -lpthread
    echo "No avaliable! Terminating ... "
    exit 1
  fi
fi
