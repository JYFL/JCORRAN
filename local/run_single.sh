#!/bin/bash

source /etc/profile
source ~/.bash_profile

pwd

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/jkral/alice/analysis/physics/JCORRAN/jemcal:/home/jkral/alice/analysis/physics/JCORRAN/jtrack:/home/jkral/alice/analysis/physics/JCORRAN/jpi0

echo "ldlib:"
echo $LD_LIBRARY_PATH

echo "shell:"
echo $SHELL

echo "parameters:"
echo $@

echo "ls:"

ls

echo "---- starting ----"

$@
