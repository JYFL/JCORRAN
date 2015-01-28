#!/bin/bash

if [ ! $1 ]
then
  echo "usage: $0 <dir>"
  exit
fi

unf=0

for i in `find $1 | grep log_o | grep -v log_o_master`
do
  lp=`cat $i | grep "%" | tail -n 1`
  dn=`cat $i | grep -c Good`
  if [ $dn -lt 1 ]
  then
    echo $i
    echo $lp $dn
    let unf=$unf+1
  fi
done
echo "Unfinished: $unf"

