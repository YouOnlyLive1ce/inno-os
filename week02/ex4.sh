#!/bin/sh

if gcc ex4.c -o ex4 -lm
then "./ex4"
else
    echo "ex4 did not compile successfully"
fi
