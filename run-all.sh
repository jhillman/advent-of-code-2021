#!/bin/bash
 
if [ "$1" != "" ]; then
    range=$1
     
    if [[ $range == [0-9]*..[0-9]* ]]; then
        startLength=`expr "$range" : '[0-9]*'`
         
        start=${range:0:$startLength}
         
        let endLength="${#range}-$startLength-2"
        let endLength="-$endLength"
         
        end=${range:$endLength}
    else
        start=$range
        end=$range
    fi
else
    start=1
 
    lastDir=`ls -d day* | tail -n 1`
 
    let endLength="${#lastDir}-3"
    let endLength="-$endLength"

    end=${lastDir:$endLength}

    if [[ $end == 0[1-9] ]]; then
        finalEndLength=endLength+1
    
        end=${lastDir:$finalEndLength}
    fi
fi
 
allCorrect="true"
 
if [ $start -le $end ]; then
    echo "Day/Part         Answer                                        Correct"
    echo "--------         ------                                        -------"
 
    for (( number=$start; number <= $end; number++ )); do
        directory="$(printf "day%02d" $number)"
 
        if [ -d $directory ]; then
            pushd . > /dev/null
            cd $directory
            
            for (( part=1; part <= 2; part++ )); do 
                answer=`./part${part}.o`
                 
                final=`grep -m 1 -o '= [^ *]*' part${part}.c | cut -d " " -f2`

                if [ "$final" == "$answer" ]; then
                    correct="true"
                else
                    if [ "$final" == "" ]; then
                        correct="N/A"
                    else
                        correct="false"
                        allCorrect="false"
                    fi
                fi
                 
                printf "Day %02d, part $part   %-48s %-4s\n" $number $answer $correct
            done

            echo "----------------------------------------------------------------------"
 
            popd > /dev/null
        fi
    done
 
    if [ $start -lt $end ]; then
        printf "All              ---                                              %s\n" $allCorrect
    fi
fi