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
totalTime=0.0
 
if [ $start -le $end ]; then
    echo "Day/Part         Answer          Correct   Time"
    echo "--------         ------          -------   ----"
 
    for (( number=$start; number <= $end; number++ )); do
        directory="$(printf "day%02d" $number)"
 
        if [ -d $directory ]; then
            pushd . > /dev/null
            cd $directory
            
            for (( part=1; part <= 2; part++ )); do 
                startTime="$(gdate +'%s.%N')"
                answer=`./part${part}.o`
                time="$(gdate +"%s.%N - ${startTime}" | bc)"
                 
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

                totalTime="$( bc <<<"$totalTime + $time" )"
                 
                printf "Day %02d, part $part   %-18s %-4s   %f\n" $number $answer $correct $time
            done

            echo "---------------------------------------------------"
 
            popd > /dev/null
        fi
    done
 
    if [ $start -lt $end ]; then
        printf "All              ---                %s   %f\n" $allCorrect $totalTime
    fi
fi