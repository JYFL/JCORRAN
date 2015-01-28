#!/bin/bash
 squeue | perl -a -nle'$n=sprintf "%s%20s",$F[3],$F[4];if($F[4] eq "R"){$h{$n}++}else{$h{$n}=$F[0]} }{ for ( sort keys %h ){ printf "%40s%20s\n", $_,$h{$_} }'

# use also ###
# slurm queue
##############
