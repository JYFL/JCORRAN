#!/bin/bash

mergerdir="/home/jkral/alice/analysis/helpers"
mailaddr="jiri.kral@jyu.fi"

if [ ! $1 ]
then
  echo "usage: $0 <jobs dir>"
  exit
fi

uname=`whoami`

nrunning=0

#job list
if [ -f $1/jobtmp ]
then
  rm $1/jobtmp
fi

qstat | grep $uname | awk '{print $1}' >> $1/jobtmp

#count still running jobs
for i in `cat $1/jobcontrol`
do
  r=`cat $1/jobtmp | grep -c $i`
  let nrunning=$nrunning+$r
done

# merge if finished, schedule next check otherwise
if [ $nrunning -eq 0 ]
then

  echo "-------------" >> $1/jobcontrol.log
  ./checkprod.sh $1 | grep Unfinished >> $1/jobcontrol.log  

  localdir=`pwd`
  
  echo "`date` :: starting the merge" >> $1/jobcontrol.log
  cd $mergerdir

  if [ -f $1/merge.log ]
  then
    rm $1/merge.log
  fi

  ./gridadd.sh $1 output_ full.root >> $1/merge.log

  cd $localdir

  echo "`date` :: done." >> $1/jobcontrol.log

  #mail me
  jn=`echo $1 | sed "s,/$,,"`
  echo ${jn##*/} > $1/mailme.txt
  cat $1/jobcontrol.log | grep Unfinished >> $1/mailme.txt

  d1str=`cat $1/jobcontrol.log | grep -m 1 ::`
  # get the runtime
  d1=`date -d "${d1str%%::*}" +%s`
  d2=`date +%s`

  days=`echo $(((d2-d1)/86400))`
  hours=`echo $(((d2-d1-(days*86400))/3600))`
  minutes=`echo $(((d2-d1-(days*86400 + hours*3600))/60))`
  echo "Runtime: $days days $hours hours $minutes minutes" >> $1/mailme.txt

  `mail -s "jobdone - ${jn##*/}" $mailaddr < $1/mailme.txt`

else
  echo "`date` :: $nrunning jobs running." >> $1/jobcontrol.log
  at -M -f $1/atdesc now + 20 minutes
fi
