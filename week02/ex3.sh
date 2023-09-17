#!/bin/sh

if gcc ex3.c -o ex3 -lm
then "./ex3"
else
    echo "ex3 did not compile successfully"
fi
