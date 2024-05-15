#!/bin/bash
max_score=100
max_student_id=5
if [ $# = 2 ]; then
	max_score=$1
	max_student_id=$2
fi
#echo $max_score
#echo $max_student_id
declare -a score
i=1
while [ $i -le $max_student_id ]
do
path="./Submissions/180512${i}/180512${i}.sh"
if test -f "$path"; then
	bash $path > "180512${i}.txt"
	curScore=$max_score
	diff "--ignore-all-space" "--ignore-trailing-space" "--ignore-blank-line" "AcceptedOutput.txt" "180512${i}.txt" > "180512${i}dif.txt"
	while read -r line; do
		if [ ${line::1} == ">" ]; then
			((curScore=curScore-5))
		elif [ ${line::1} == "<" ]; then
			((curScore=curScore-5))
		fi
	done <"180512${i}dif.txt"
	rm "180512${i}dif.txt"
	score[$i]=$curScore
	j=1
	while [ $j -le $max_student_id ]
	do
		if [ $j -ne $i ]; then
			path2="./Submissions/180512${j}/180512${j}.sh"
			if test -f "$path2"; then
				#bash $path2 > "180512${j}.txt"
				diff "--ignore-trailing-space" "--ignore-blank-line" "./Submissions/180512${i}/180512${i}.sh" "./Submissions/180512${j}/180512${j}.sh" > "${i}_${j}_dif.txt"
				cnt=0
				while read -r line; do
					#echo $line;
					((cnt=cnt+1))
				done <"${i}_${j}_dif.txt"
				#rm "180512${j}.txt"
				rm "${i}_${j}_dif.txt"
				if [ $cnt -eq 0 ]; then
					score[$i]=-$max_score
					break
				fi
			fi
		fi
		((j=j+1))
	done
	rm "180512${i}.txt"
else
	score[$i]=0
fi
((i=i+1))
done
i=1
touch "output.csv"
printf "student_id,score\n" > output.csv
while [ $i -le $max_student_id ]
do
	printf "180512${i},${score[$i]}\n" >> output.csv
	((i=i+1))
done
