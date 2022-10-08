m=4
n1=1801
i=1
programs=("lab3" "static1" "static3" "static4" "static5" "dynamic1" "dynamic3" "dynamic4" "dynamic5" "guided1" "guided3" "guided4" "guided5")

result_file="lab3_13.csv"
echo "program; n; t,ms" >$result_file
for program_name in "${programs[@]}"; do
  while [ $i -lt $n1 ]; do
    echo $i
    r=$(./$program_name $i $m)
    echo $program_name ";" $r >>$result_file
    ((i = $i + 5))
  done
  i=0
done
