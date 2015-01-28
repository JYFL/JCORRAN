#!/bin/bash

echo "## Make jcorranEff list with RunByRun"
ls output/ | while read x; do echo $x;find output/$x -name jcorranEff.root > list/jcorranEff_$x.list;done

ls list | while read x; do ./mergeList.sh list/$x 50 4 merged/$x.root;done

