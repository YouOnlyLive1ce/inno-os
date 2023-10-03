#!/bin/sh

if gcc ex2.c -o ex2 
	then "./ex2"
else
    echo "ex2 did not compile successfully"
fi
