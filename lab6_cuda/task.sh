make clean
make lab6

n1=1801
n2=629554
i=1801
programs=("lab6")

result_file="lab6_"
for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+='.csv'
  echo $program_name
  echo "program; n; t,ms" >$full_result_file
  while [ $i -lt $n2 ]; do
    echo $i
    r=$(./$program_name $i)
    echo $program_name";"$r >>$full_result_file
    ((i = $i + 62775))
  done
  full_result_file=''
  i=$n1
done
