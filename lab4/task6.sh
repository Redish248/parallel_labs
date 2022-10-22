make clean
make all

m=4
n1=1801
n2=629554
i=n1
programs=("lab4" "static1" "static3" "static4" "static5" "dynamic1" "dynamic3" "dynamic4" "dynamic5" "guided1" "guided3" "guided4" "guided5")

result_file="lab4_6_"
for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+='.csv'
  echo "program; n; t,ms" >$full_result_file
  while [ $i -lt $n2 ]; do
    echo $i
    r=$(./$program_name $i $m)
    echo $program_name";"$r >>$full_result_file
    ((i = $i + 10000))
  done
  full_result_file=''
  i=n1
done
