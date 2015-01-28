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

#save card
cp $3 $2/

chunkok=0
chunknok=0
chunktotal=0
echo "searching for input files ..."
for i in `cat $runlist`
do
  echo ""
  echo "run: $i ======================================================"
  
  #strip leading zeroes if any
  rnumnonz=`echo $i | sed 's/0*//'`
  rnum=`printf %09d $rnumnonz`
  
  echo $runsuperbase
  
  # get data dir
  runbase=`find -L $runsuperbase -mindepth 1 -maxdepth 2 -name *${rnum} -type d`

  # skip if does not exist
  if [ "$runbase" == "" ]
  then
    echo "run $i not found, skipping..."
    continue
  fi
  
  # find how many ESDs are in there
  nesd=`find -L ${runbase} -name jcorran.root | grep -c .`
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
      echo "  chunk $chunkstr OK."
      let chunkok=$chunkok+1
      let j=j+1
      continue
    else
      echo "  chunk $chunkstr NOT OK"
      let chunknok=$chunknok+1
    fi
  
    #test only
    if [ "$4" == "test" ]
    then
      if [ -f $ofile ]
      then
        echo "-----------------------------------------------"
  head -n 40 $ofile | grep node
        tail -n 7 $ofile
  echo ""
  tail -n 7 $efile
        echo ""
      fi
      let j=j+1
      continue
    fi

    echo "submitting... ---------------------------------"
  
    echo ""
    echo "rnum: $rnum"
    echo "runbase: $runbase"
    echo "runoutdir: $routdir"
  
    # clean the output dir for a new run
    for irm in `ls $routdir | grep $chunkstr`
    do
      rm $routdir/$irm
    done

    echo -n "building file list..."
    let jp1=$j+1

    # last chunk takes the rest
    if [ $jp1 -eq $nchunk ] 
    then
      let nlastesd=$nesd-$j*$nchunkesd
      let topesd=$j*$nchunkesd+$nlastesd
      echo "to file: $topesd / $nesd"
      `find -L ${runbase} -name jcorran.root | tail -n $nlastesd > $routdir/filelist_$chunkstr.txt`
    else
      let topesd=$jp1*$nchunkesd
      echo "to file: $topesd / $nesd"
      `find -L ${runbase} -name jcorran.root | grep -m $topesd . | tail -n $nchunkesd > $routdir/filelist_$chunkstr.txt`
    fi

    echo "submitting job..."

    jobnum=`qsub -S /bin/bash -cwd -o $routdir/log_o_$chunkstr.txt -e $routdir/log_e_$chunkstr.txt run_single.sh ./jtkt_alice $routdir/filelist_$chunkstr.txt $routdir/output_$chunkstr.root $3 pizero pizero | awk '{print $3}'`
    echo $jobnum >> $2/jobcontrol
    echo "Job $jobnum submitted."
    echo "-----------------------------------------------"
    echo ""
    
    let j=j+1
  done  # chunks
done # runs

echo "Summary"
echo "Total: $chunktotal  DONE: $chunkok  NOT DONE: $chunknok"
echo ""

echo "`date` :: jobs submitted" >> $2/jobcontrol.log

# prepare the at file
echo "`pwd`/jobcontrol_single.sh $2" > $2/atdesc

at -M -f $2/atdesc now + 20 minutes
echo "job control started"
atq

