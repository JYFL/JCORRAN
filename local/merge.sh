#!/bin/bash

export DIR=$1
mkdir -p $DIR/merged
find $DIR/data/ -type f -name '*.root' | perl -ne'/000(\d{6})/ and $h{$1}.=$_}{for ( keys %h){ open $f,">$ENV{DIR}/merged/$_.txt";print $f $h{$_}}'
ls $DIR/merged/1*txt | while read x; do sbatch -v ./merge_one.sh $x;done
