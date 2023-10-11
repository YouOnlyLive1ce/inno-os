#!/bin/bash
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

gnome-terminal -- ./publisher $1

for (( i = 1; i <= $1; ++i))
do
gnome-terminal -- bash -c "./subscriber $i; exec bash"
done
