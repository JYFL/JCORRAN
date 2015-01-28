#!/bin/bash

if [ ! $4 ]
then 
  echo "usage: $0 <data dir> <output dir> <card file> <start/test> [runlist]"
  echo "  specifying test does not resubmit the jobs"
  exit
fi

# define variables
chunkesds=100

if [ ! $5 ]
then
  runlist=$1/runlist.txt
else
  runlist=$5
fi

runsuperbase=$1

if [ ! -d $2 ]
then
  mkdir -p $2
fi

# erase job control file if exists
if [ -f $2/jobcontrol ]
then
  rm $2/jobcontrol
fi
if [ -f $2/jobcontrol.log ]
then
  rm $2/jobcontrol.log
fi

# dump the card file into the job control
echo "XJCoutdir: $2" >> $2/jobcontrol
echo "XJCcard: $3" >> $2/jobcontrol

#copy the card
cp $3 $2/

chunkok=0
chunknok=0
chunktotal=0
chunktorun=1  #bloody qsub indexing starts with 1

echo "checking for existing production and preparing a new one ..."
echo $runsuperbase
echo "searching for input files ..."

for i in `cat $runlist`
do
  echo ""
  echo "run: $i ------------------------------------------------------"
  
  #strip leading zeroes if any
#  rnumnonz=`echo $i | sed 's/0*//'`
#  rnum=`printf %09d $rnumnonz`
  rnum=$i
 
  # get data dir
  runbase=`find -L $runsuperbase -mindepth 1 -maxdepth 1 -name *${rnum} -type d`

  # skip if does not exist
  if [ "$runbase" == "" ]
  then
    echo "run $i not found, skipping..."
    continue
  fi
  
  # find how many file are in there
  # dumpt the run directory content
  find -L ${runbase} -name jcorran.root > $2/runtmpf

  #nesd=`find -L ${runbase} -name jcorran.root | grep -c .`
  nesd=`cat $2/runtmpf | grep -c .`

  let nchunk=$nesd/$chunkesds
  if [ $nchunk -eq 0 ]
  then
    nchunk=1
  fi
  
  #equilibrate a bit
  let nchunkesd=$nesd/$nchunk
  
  echo "jcorran files: $nesd ; chunks: $nchunk ; files per chunk: $nchunkesd"
  
  # ------------------------ submit algo here --------------------
  j=0
  
  let chunktotal=$chunktotal+$nchunk
  
  routdir=$2/$rnum
  
  if [ ! -d $routdir ]
  then
    mkdir -p $routdir
  fi
  
  #if [ $routdir ]
  #then
  #  rm -rf $routdir/*
  #fi

  while [ $j -lt $nchunk ]  # chunks
  do
    chunkstr=`printf %04d $j`
    chunktorunstr=`printf %04d $chunktorun`

    if [ -d $routdir ]
    then
      ofile=$routdir/log_o_$chunkstr.txt
      efile=$routdir/log_e_$chunkstr.txt
    fi
  
    isdone=0
  
    if [ -f $ofile ]
    then
      isdone=`cat $ofile | grep -c "Good Bye!"`
    fi
    
    if [ $isdone -gt 0 ]
    then
      echo "  chunk $chunkstr DONE."
      let chunkok=$chunkok+1
      let j=j+1
      continue
    else
      echo "  chunk $chunkstr NOT DONE"
      let chunknok=$chunknok+1
    fi
  
    #test only
    if [ "$4" == "test" ]
    then
      continue
    fi

    # clean the output dir for a new run
    for irm in `ls $routdir | grep $chunkstr`
    do
      rm $routdir/$irm
    done

    # build the file list
    let jp1=$j+1

    # last chunk takes the rest
    if [ $jp1 -eq $nchunk ] 
    then
      let nlastesd=$nesd-$j*$nchunkesd
      let topesd=$j*$nchunkesd+$nlastesd
      echo "to file: $topesd / $nesd"
      #`find -L ${runbase} -name jcorran.root | tail -n $nlastesd > $routdir/filelist_$chunkstr.txt`
      `cat $2/runtmpf | tail -n $nlastesd > $routdir/filelist_$chunkstr.txt`
    else
      let topesd=$jp1*$nchunkesd
      echo "to file: $topesd / $nesd"
      #`find -L ${runbase} -name jcorran.root | grep -m $topesd . | tail -n $nchunkesd > $routdir/filelist_$chunkstr.txt`
      `cat $2/runtmpf | grep -m $topesd . | tail -n $nchunkesd > $routdir/filelist_$chunkstr.txt`
    fi

    echo "XJCjob$chunktorunstr: $chunkstr $routdir" >> $2/jobcontrol
    
    let j=j+1
    let chunktorun=chunktorun+1

  done  # chunks
done # runs

echo ""
echo "Summary ---------------------------------------------------------"
echo "Total: $chunktotal  DONE: $chunkok  NOT DONE: $chunknok"
echo ""

#submint the array
if [ $chunktorun -gt 1 ]
then
  let chunktorun=chunktorun-1

  # the bloody qsub indexing starts from 1
  echo -n "Submitting the array... "
  jobnumstr=`qsub -S /bin/bash -cwd -t 1-$chunktorun -o $2/log_o_master.txt -e $2/log_e_master.txt run.sh $2/jobcontrol | awk '{print $3}'`

  jobnum=`echo ${jobnumstr%%.*}`
  echo "XJCmasterjob: $jobnum" >> $2/jobcontrol

  echo "masterjob: $jobnum"

	echo "`date` :: master job $jobnum submitted" >> $2/jobcontrol.log

  # prepare the at file
  echo "`pwd`/jobcontrol.sh $2" > $2/atdesc

  echo ""
  echo -n "job control starting... "
  at -M -f $2/atdesc now + 1 minutes
  atq

  echo ""
  echo "Launched."
else
  echo "Nothing to run!"
fi

