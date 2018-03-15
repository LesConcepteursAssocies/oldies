#!/bin/sh
MODEM=/dev/ttyS2
T=/tmp/tmpfile.$$
TO_MATCH="NMBR =.*
"
while true; do
	serialwatch  $MODEM "$TO_MATCH" >$T # 2>/dev/null

	# get rid of ^M 's and select the number part only
	NUMBER=`sed -n '/NMBR =/{s@^M@@;s@^.*NMBR = @@;p;}' <$T`
	rm -f $T

	# log the event
	echo "$NUMBER" is calling...
	# lookup "$NUMBER" in phonebook and print caller's name (if known) &

	# and start all over
done
