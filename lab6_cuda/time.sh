make clean
make time

n1=1801
n2=629554
i=1801
programs=("lab6t")

result_file="timeN_"
for program_name in "${programs[@]}"; do
  full_result_file+=$result_file
  full_result_file+=$program_name
  full_result_file+=_
  echo $program_name
  while [ $i -lt $n2 ]; do
    echo $i
    ./$program_name $i >> $full_result_file$i.csv
    ((i = $i + 62775))
  done
  full_result_file=''
  i=$n1
done
