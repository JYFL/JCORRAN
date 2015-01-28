#!/bin/bash
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
echo "========================================="
echo "############## PATH : ##############"
echo $PATH
echo "############## LD_LIBRARY_PATH : ##############"
echo $LD_LIBRARY_PATH
echo "############## ROOTSYS : ##############"
echo $ROOTSYS
echo "############## which root : ##############"
which root
echo "############## ALICE_ROOT : ##############"
echo $ALICE_ROOT
echo "############## which aliroot : ##############"
which aliroot
echo "############## system limits : ##############"
ulimit -a
echo "############## memory : ##############"
free -m
echo "========================================="

tar -zxf pythiaJetPackage.tgz
cd pythiaJetPackage
cd pythia
make clean && make
export PYTHIA8=$PWD
export PYTHIA8DATA=$PYTHIA8/xmldoc
cd ..

cd fastjet
./configure --prefix=$PWD/install
make clean && make && make install
export FASTJET=$PWD/install/bin
cd ..

make
./pythiaJet cardAlice_pp.input card_jet.config pythiaJet.root $1
cp pythiaJet.root ../

RET=$?
if [ "$RET" != "0" ];then
  echo "======== ERROR : TaskJC.C finished with NON zero code: $RET ========"
  if [ "$RET" -gt 128 ] && [ "$RET" -lt 160 ]; then
    let sig="$RET - 128"
    sigs='HUP INT QUIT ILL TRAP ABRT BUS FPE
    KILL USR1 SEGV USR2 PIPE ALRM TERM STKFLT
    CHLD CONT STOP TSTP TTIN TTOU URG XCPU
    XFSZ VTALRM PROF WINCH IO PWR SYS'
    sig=SIG`echo $sigs | awk '{ print $'"$sig"' }'`
    echo "======== it appears to have been killed with signal: $sig ========"
  fi
  exit $RET
fi

echo "======== TaskJC.C finished with exit code: $RET ========"
echo "############## memory after: ##############"
free -m
