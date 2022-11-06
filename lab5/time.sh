make clean
make static

m=1 # 2 3 4 5 6
n1=1801
n2=629554
i=1801
programs=("lab5s")

result_file="lab5_"
for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+=_$m
  full_result_file+='.csv'
  echo $program_name
  echo "program; n; gen t,ms; map t,ms; merge t,ms; sort t,ms" >$full_result_file
  while [ $i -lt $n2 ]; do
    echo $i
    r=$(./$program_name $i $m)
    echo $program_name";"$r >>$full_result_file
    ((i = $i + 62775))
  done
  full_result_file=''
  i=$n1
done
