#!/bin/bash
#export alien_CLOSE_SE="ALICE::Prague::SE"
#source /mnt/flustre/bschang/common/alice/packages/env_aliroot.sh
#source ~bschang/common/alice/AliRoot_v4-21-17a-AN/packages/env_aliroot.sh
#export LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
#source /mnt/flustre/alice/common/alice2/packages/env_aliroot.sh

PAR_PACKAGES="PWGJCORRAN"
for pac in $PAR_PACKAGES
do
    cd $pac
    make clean
    cd ..
    rm $pac.par
    tar -zcvhf $pac.par --exclude="*/macros/*" $pac/
done

# echo $LD_LIBRARY_PATH
# which aliroot
#aliroot -b -q -l 'runGrid_LHC13b2.C' || exit
aliroot -b -q -l 'runGrid_PbPb_MC.C' || exit
#aliroot -b -q -l 'runGrid_LHC11b10a.C' || exit
#aliroot -b -q -l 'runGrid_LHC11a.C' || exit
#aliroot -b -q -l 'runGrid_LHC10h.C' || exit
#aliroot -b -q -l 'runGrid_LHC11a10a_bis.C' || exit
#aliroot -b -q -l 'runGrid_LHC13b.C' || exit
#aliroot -b -q -l 'runGrid_LHC13c.C' || exit

