make clean
make task14

m=3 #add best correct data
n1=50000
i=1
programs=("lab3o1" "lab3o2" "lab3o4")

for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+='.csv'
  echo "program; n; t,ms" >$full_result_file
  while [ $i -lt $n1 ]; do
    echo $i
    r=$(./$program_name $i $m)
    echo $program_name";"$r >>$full_result_file
    ((i = $i + 5))
  done
  full_result_file=''
  i=0
done
