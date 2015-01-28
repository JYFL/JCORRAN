#!/bin/bash
JOB=$1
#alien_submit TaskJC_BS_130408.jdl xml/LHC10d1/ Efficiency/pp7TeV/r1532/L0MBOR/ 115186  v5-04-61-AN v5-34-07 b/bschang
cat split/$JOB  | grep -v ^# | perl -anle'$ENV{USER}=~/((.).+)/;print "alien_submit TaskJC_BS_130408.jdl xml/$_ Efficiency/pp7TeV/r1532/L0MBOR/ $_ v5-04-61-AN v5-34-07 $2/$1"' |  sh  | tee logs/$JOB-$USER.log
