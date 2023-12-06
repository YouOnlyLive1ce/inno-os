#!/bin/sh

if gcc ex1.c -o ex1
then "./ex1 3 3"
else
    echo "ex1 did not compile successfully"
fi
