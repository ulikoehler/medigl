#!/bin/sh
for arg in $@
do
	medcon -c png -f $arg
done