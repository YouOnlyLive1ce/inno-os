#!/bin/sh

if gcc ex1.c -o ex1
then "./ex1"
else
    echo "ex1 did not compile successfully"
fi
