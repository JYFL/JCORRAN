#!/bin/bash

#aliensh -c "gbbox ps -AXS" | perl -anle'$F[3]=~/^(EV|ESV|Z|ER|EX|EE|EIB|EVT|ERROR_I|ESPLT)$/ and /\b(2\d{8})/ and $1>=277382296 and print $1'| xargs  -n100 alien_resubmit;
aliensh -c "gbbox ps -AXS" | perl -anle'$F[3]=~/^(ESPLT)$/ and /\b(2\d{8})/ and $1>=296499015 and print $1'| xargs  -n100 alien_resubmit;
