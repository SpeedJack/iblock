#!/bin/bash
echo -n "VSIZE: "
sort -n -t, -k5 "$@" | tail -n1 | cut -d, -f5
echo -n "SIZE: "
sort -n -t, -k6 "$@" | tail -n1 | cut -d, -f6
echo -n "RSS: "
sort -n -t, -k7 "$@" | tail -n1 | cut -d, -f7
echo -n "TIME: "
stime=`head -n2 "$@" | tail -n1 | cut -d, -f1`
etime=`tail -n1 "$@" | cut -d, -f1`
python -c "print($etime - $stime)"
