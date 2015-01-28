#!/bin/csh -f

#runstat.csh
#sort according the CPU time
#runstat.csh |  sort -n -k4 -r > stat.txt

#"#run   pDST      pDST     CPU time        CPU time      #used/#in   time/eve"
#"        #ev      #ev used   [s]             [h]          [%]         [ms]"

set sumin = 0
set suman = 0
set indir = $1
#foreach f (outputs/${indir}/logs/*.log)
foreach f (outputs/*/logs/*.log)
#foreach f (outputs/*/logs/000167813-s2000-*.log)
    set totin = `cat $f  | tail -3 | head -1 | awk '{print $9}'`
    set totan = `cat $f  | tail -2 | head -1 | awk '{print $9}'`
    @ sumin = $sumin + $totin
    @ suman = $suman + $totan
    set tims = `cat $f  | grep time | head -1 | awk '{print $6}'`
    set tim = `cat $f  | grep time | head -1 | awk '{print $3}'`
    set fname = `basename $f`
    set runid = `echo $fname|awk -F "_" '{print substr($1,4,6)}'`
    echo $runid"   "$totin"  "$totan"    "$tims"      "$tim  |\
        awk '{ if($2>0) {fraction = $3/$2*100; timeeve = $4/$2*1e3;} else {fraction=0; timeeve=0;} printf("%s %6d \t %6d %10.3f \t%s %10.2f %10.1f\n",$1,$2,$3,$4,$5, fraction, timeeve)}'
end
echo "--------------------------------------------------------------------------------"
echo "     " $sumin $suman

