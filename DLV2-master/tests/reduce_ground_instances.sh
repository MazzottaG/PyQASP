#!/bin/bash

FILE=short.20;
SIZE=`wc -l < $FILE | sed "s/ //g"`;
WASP=../build/debug/preasp
CLASP=clasp
DECREMENT=1

for i in {1..1000}
do
    perl -MList::Util -e 'print List::Util::shuffle <>' $FILE | head -n $SIZE > tmp;
    echo $i;
    cat tmp names |  $WASP > output1;
    exit_wasp=$?
    cat tmp names |  $CLASP --outf=1 > output2;
    exit_clasp=$?
    cat output1 | grep COST | tail -n1 | sed "s/@.*//g" > output1.clean
    cat output2 | grep COST | tail -n1  > output2.clean
    diff output1.clean output2.clean
    DIFF=$?
    if [ "$exit_wasp" -eq 134  ] || [ "$exit_wasp" -eq 20  -a "$exit_clasp" -ne 20  ] || [ "$DIFF" -ne 0 ]; then
        cp tmp short.$SIZE;
        FILE=short.$SIZE;
        SIZE=$((SIZE-DECREMENT))
        printf "ERROR $i - NEW SIZE $SIZE\n";
        if [ "$SIZE" -le 0 ]; then        
            exit 1;
        fi
    fi
done
