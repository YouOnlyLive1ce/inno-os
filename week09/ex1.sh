[In reply to Shamil]
#!/bin/bash
gcc pager.c -o pager  
gcc mmu.c -o mmu 

touch ex1.txt


echo "Aging hit rate: " > ex1.txt
./pager 4 2 aging
PAGER_PID=%1
./mmu 4 "R0 R1 W1 R0 R2 W2 R0 R3 W2" $PAGER_PID

sleep 3

echo "Random hit rate: " >> ex1.txt
./pager 4 2 random
PAGER_PID=%1
./mmu 4 "R0 R1 W1 R0 R2 W2 R0 R3 W2" $PAGER_PID

sleep 3

echo "NFU hit rate: " >> ex1.txt
./pager 4 2 nfu
PAGER_PID=%1
./mmu 4 "R0 R1 W1 R0 R2 W2 R0 R3 W2" $PAGER_PID

ex1.sh