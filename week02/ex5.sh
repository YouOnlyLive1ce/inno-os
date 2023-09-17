#!/bin/sh

if gcc ex5.c -o ex5 -lm
then "./ex5"
else
    echo "ex5 did not compile successfully"
fi
