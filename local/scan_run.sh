#!/bin/bash

dataset=/mnt/flustre/alice/taxi_jcorran/13_sets/all
outdir=/mnt/flustre/jkral/scan

nset=0
esf=1

for i in `cat scan_cut_ranges.txt | grep minEnergy | awk '{$1=""; print $0}'`
do
  for j in `cat scan_cut_ranges.txt | grep pi0AsymRange | awk '{$1=""; print $0}'`
  do
#     for k in `cat scan_cut_ranges.txt | grep emcalLambda02Cut | awk '{$1=""; print $0}'`
#     do
      for l in `cat scan_cut_ranges.txt | grep pi0DoSMCut | awk '{$1=""; print $0}'`
      do

        # make the list
        setstr=`printf %03d $nset`
        setcard="`pwd`/scan_cards/cardAlice_pPb_photon_scan_$setstr.input"
        cp ./analysis_cards/cardAlice_pPb_photon_master.input $setcard

        echo "" >> $setcard
        echo "minEnergy $i" >> $setcard
        echo "pi0AsymRange 0.0 $j" >> $setcard
#         echo "emcalLambda02Cut -1 $k" >> $setcard
        echo "pi0DoSMCut $l" >> $setcard
        echo "" >> $setcard

        # run the production
        ./rerunproduction.sh $dataset $outdir/$setstr $setcard start
        
        cp $setcard $outdir/$setstr/

        let nset=nset+1
      
      done
#     done
  done

done