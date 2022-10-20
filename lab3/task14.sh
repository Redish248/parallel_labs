make clean
make task14

m=4
n1=629554
i=1801
programs=("lab3o1" "lab3o2" "lab3o0" "lab3o3")

result_file="lab3_14_"
for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+='.csv'
  echo "program; n; t,ms" >$full_result_file
  while [ $i -lt $n1 ]; do
    echo $i
    r=$(./$program_name $i $m)
    echo $program_name";"$r >>$full_result_file
    ((i = $i + 10000))
  done
  full_result_file=''
  i=1801
done
