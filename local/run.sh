#!/bin/bash

# get the startup parameters
chunktorunstr=`printf %04d $SGE_TASK_ID`

# dig the config from the job control
outdir=`cat $1 | grep XJCoutdir | awk '{print $2}'`
card=`cat $1 | grep XJCcard | awk '{print $2}'`
chunkstr=`cat $1 | grep XJCjob$chunktorunstr | awk '{print $2}'`
routdir=`cat $1 | grep XJCjob$chunktorunstr | awk '{print $3}'`

# stdout and stderr files
ofile=$routdir/log_o_$chunkstr.txt
efile=$routdir/log_e_$chunkstr.txt
filelist=$routdir/filelist_$chunkstr.txt
outfile=$routdir/output_$chunkstr.root

source /etc/profile >> $ofile 2>>$efile
source ~/.bash_profile >> $ofile 2>>$efile

pwd >> $ofile 2>>$efile

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jkral/alice/analysis/physics/JCORRAN/jemcal:/home/jkral/alice/analysis/physics/JCORRAN/jtrack:/home/jkral/alice/analysis/physics/JCORRAN/jpi0

echo "ldlib:"  >> $ofile 2>>$efile
echo $LD_LIBRARY_PATH  >> $ofile 2>>$efile

echo "shell:"  >> $ofile 2>>$efile
echo $SHELL  >> $ofile 2>>$efile

echo "parameters:"  >> $ofile 2>>$efile
echo $@  >> $ofile 2>>$efile
echo $SGE_TASK_ID >> $ofile 2>>$efile
echo $chunktorunstr >> $ofile 2>>$efile
echo $routdir >> $ofile 2>>$efile
echo $card >> $ofile 2>>$efile
echo $chunkstr >> $ofile 2>>$efile

echo "ls:"  >> $ofile 2>>$efile

ls  >> $ofile 2>>$efile

echo "---- starting ----"  >> $ofile 2>>$efile

# get the parameters to start up

./jtkt_alice $filelist $outfile $card pizero pizero >> $ofile 2>>$efile
