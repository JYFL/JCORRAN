#!/bin/bash -f

export lastjobid=$1
echo "submitting the jobID > ${lastjobid}"

#source /mnt/flustre/alice/common/alice-v5-04-02-AN/packages/env_aliroot.sh

aliensh -c "gbbox ps -AXS" | perl -anle'$F[3]=~/^(EV|ESV|Z|ER|EX|EE|EIB|EVT|ERROR_I|ESPLT)$/ and /\b(2\d{8})/ and $1>$ENV{$lastjobid} and print $1'| xargs  -n100 alien_resubmit;
