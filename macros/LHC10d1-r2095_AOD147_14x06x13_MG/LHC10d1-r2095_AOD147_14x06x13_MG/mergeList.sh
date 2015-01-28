#!/bin/bash

function usage() {
echo "=================================================================================================================================";
echo "   Summary : $0 is a merging script of root with multiple process."
echo "             This can be easily extended to other prarallel jobs."
echo
echo "   USAGE : $0 <listfile> [Number of files in a process] [Number of processes] [output file]"
echo ""
echo "     <listfile>                     : list of root files"
echo "     [Number of files in a process] : defalult 1000. "
echo "                                      NOTE : This is not a number of jobs. Number of files per a job"
echo "                                      Number should be smaller 1000 and sqrt(number of all rootfiles) is usually best."
echo "     [Number of processes ]         : default 1. "
echo "                                      NOTE: This is not number of Cores. you can launch 1000 process with even 1 core."
echo "     [output file]                  : default <listfile>-merge.root"
echo 
echo "   EXAMPLE :  $0 rootlist.txt 400 4"
echo "=================================================================================================================================";
}

if [ $# -lt 1 ];then
    usage;
    exit 1 ;
fi

inputList=$1
NSPLIT=${2:-1000}
NCORES=${3:-1}
OutPutRoot=${4:-"$inputList-mreged.root"}

if [ ! -f $inputList ];then
    echo "ERROR : $inputList does not exists !!!!!!!" >&2
    usage;
    exit 1;
fi

MYTMP=/tmp/$USER/batch/PID$$-`date +%Y%m%d-%H%M%S`-merge-$inputList-$RANDOM
while [ -e $MYTMP ];do MYTMP=$MYTMP$RANDOM; done;
    echo $MYTMP

    {
        startTime=`perl -e'print time'`
        echo "INPUT : $inputList"
        echo "OUTPUT : $OutPutRoot"
        echo "NCores : $NCORES"
        echo $$
        WRKDIR=$PWD;
        date
        mkdir -p $MYTMP;

        cd $MYTMP;
        split -l$NSPLIT $WRKDIR/$inputList

        cd $WRKDIR;
        #ls $MYTMP/x* | while read x;do grep -vf $WRKDIR/exclude.list $x > $x-z1;done
        #ls $MYTMP/x*-z1 | xargs -I% -P$NCORES hadd -T -f9 %.root @%
        ls $MYTMP/x* | xargs -I% -P$NCORES hadd -T  %.root @%
        hadd -T  $OutPutRoot $MYTMP/x*.root
        #hadd -T -f9 $OutPutRoot $MYTMP/x*.root
        date
        endTime=`perl -e'print time'`
        echo "==== Time : "`expr $endTime - $startTime`
    } 2>&1 | tee $inputList-merging.log


