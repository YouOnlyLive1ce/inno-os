#!/bin/sh

gcc ex3.c -o ex3 

./ex3 3 & 
pstree
sleep 6
pstree
sleep 6 

./ex3 5 & 
sleep 6 
pstree 
sleep 11 
pstree
