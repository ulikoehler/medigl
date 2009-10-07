#!/usr/bin/env python
from __future__ import with_statement
import sys

if __name__ == "__main__":
	with open(sys.argv[1]) as infile:
		print "const char* s ="
		for line in infile:
			print "\t\"" + line.replace("\"","\\\"").strip("\n") + "\""
		print "\t;"