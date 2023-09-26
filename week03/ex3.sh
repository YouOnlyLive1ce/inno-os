#!/bin/sh

if gcc ex3.c -o ex3
then "./ex3"
else
    echo "ex3 did not compile successfully"
fi
