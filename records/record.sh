#!/bin/bash
SEARCHQUERY="(python Main.py|opp_run.*IBLOCK)"
SLEEP=1
FILENAME="result"
FIELDS="pid,comm,stime,vsize,size,rss,etimes,cputimes"
PREVPIDS=()
while true
do
	PIDS=`ps -eo pid,args | grep -E "$SEARCHQUERY" | sed -e 's/^[[:space:]]*//' | grep -vE '^[0-9]+ +grep .*$' | cut -d' ' -f1`
	for PID in $PIDS
	do
		if [[ ${PREVPIDS[@]} =~ $PID ]]; then
			continue
		fi
		PREVPIDS+=($PID)
		if [ ! -f $FILENAME.$PID.csv ]; then
			echo "timestamp,$FIELDS" > $FILENAME.$PID.csv
			NOW=`date`
			echo -n "$NOW | New process: "
			ps -q $PID -o pid,args | tail -n1
		fi
	done
	for PID in $PIDS
	do
		(date +'%s.%N'; ps -q $PID -o $FIELDS | tail -n1 | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//' -e 's/  */ /g' -e 's/ /,/g') | paste -d, - - >> $FILENAME.$PID.csv
	done
	for PID in ${PREVPIDS[@]}
	do
		if [[ ! $PIDS =~ $PID ]]; then
			PREVPIDS=(${PREVPIDS[@]/$PID})
			NOW=`date`
			echo "$NOW | Process $PID terminated."
		fi
	done
	sleep $SLEEP
done
